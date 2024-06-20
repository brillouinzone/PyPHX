#include "phxgrabberinterface.h"
#include "common/common.h"
#if defined _PHX_WIN32
#define _PHX_DISPLAY
#endif
#include "pdl_api.h"
#include "phx.cpp"
#include "pbl_api.h"
#include "phx_api.h"
#include "phx_os.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "pil_api.h"

#define MAX_SZ_CHARS 256   /* Maximum string length */


static void handleInterruptEvents(tHandle hCamera, ui32 interruptMask, void* userParameter) {
    EventContext* eventContext = static_cast<EventContext*>(userParameter);

    (void)hCamera;

    if (PHX_INTRPT_BUFFER_READY & interruptMask)
        eventContext->nBufferReadyCount++;

    if (PHX_INTRPT_FIFO_OVERFLOW & interruptMask)
        eventContext->fFifoOverFlow = TRUE;

    // The callback routine may be called with more than 1 event flag set.
    // Therefore all possible events must be handled here.

    if (PHX_INTRPT_FRAME_END & interruptMask) {
    }
}

PHXGrabberInterface::PHXGrabberInterface() {
    _isOpened = false;
    _useEventCounter = false;
}

PHXGrabberInterface::~PHXGrabberInterface() {
    close();
}

void PHXGrabberInterface::open(const std::string& _cameraConfigFile) {
    if (isOpened()) {
        messageOutput("Connection already Opened!");
        return;
    }
    printf("opening grabber interface \n");

    cameraConfigFile = _cameraConfigFile;

    bool oState = configureGrabber();

    setState(oState);

    if (isOpened())
        oState = livePicture();

    if (!oState)
        messageOutput(_lastError + "\nAborting...\n");

    releaseGrabber();
}

void PHXGrabberInterface::close() {
    setState(false);
}

void PHXGrabberInterface::setState(bool isOpened) {
    _isOpened = isOpened;
    // Implement grabberStatusChanged if needed
}

void PHXGrabberInterface::setEventCounterUsage(bool useEventCounter) {
    _useEventCounter = useEventCounter;
}

std::string PHXGrabberInterface::lastError() const {
    return _lastError;
}

bool PHXGrabberInterface::isOpened() const {
    return _isOpened;
}

void grabberErrorHandler(const char* pszFnName, etStat eErrCode, const char* pszDescString) {
    std::string error = "Phx Error: " +
        std::string(pszFnName) + " : " +
        std::to_string(eErrCode) + " : " +
        std::string(pszDescString);

    std::cerr << error << std::endl;
    //messageOutput(_lastError + "\nAborting...\n");
}

bool PHXGrabberInterface::configureGrabber() {
    etStat eStatus = PHX_OK;

    hCamera = 0;
    hDisplay = 0;
    hBuffer1 = 0;
    hBuffer2 = 0;
    etStat         eStat = PHX_OK;      /* Status variable */
    etParamValue   eParamValue;                              /* Parameter for use with PHX_ParameterSet/Get calls */
    etParamValue   eCaptureFormat = PHX_BUS_FORMAT_MONO16;
    ui32           dwCamSrcDepth = 16; /*make sure this value matches to the param number*/
    etParamValue   eCamSrcCol, eCamFormat, eAcqType;
    etPblParamValue eBayCol;
    stImageBuff* psImageBuffers = NULL;        /* Capture buffer array */
                                 /* User defined event context */

    tPHX* phCaptureBuffers = NULL;        /* Capture buffer handle array */
    tPHX           hDisplayBuffer = 0;           /* Display buffer handle */
    tPHX           hBuffHandle;
    tFlag          fDebayer = FALSE;
    ui32           dwBufferReadyLast = 0;           /* Previous BufferReady count value */
    ui32           dwImageWidth = 1280;
    ui32           dwImageHeight = 1020;
    ui32           camWidth = 1280;
    ui32           camHeight = 1020;
    ui32           dwBufferWidth = 2 * 1280;
    ui32 dwBufferStride = 1 * dwBufferWidth;
    ui32           imperbuf = 1;
    ui32           dwBufferHeight = imperbuf * 1020;
    ui32           dwBufferCount = 1;
    tFlag          fCameraIsCxp = FALSE;
    tFlag          fIsCxpCameraDiscovered = FALSE;
    int i;

    PhxCommonKbInit();

    memset(&sPhxLive, 0, sizeof(sPhxLive));

    /* Create a channel handle */
    eStatus = PHX_Create(&hCamera, PHX_ErrHandlerDefault);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    printf("created camera channel handle\n");
    /* Set the configuration file name */
    eStatus = PHX_ParameterSet(hCamera, PHX_CONFIG_FILE, &cameraConfigFile);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /* Set the board number */
    eStatus = PHX_ParameterSet(hCamera, PHX_BOARD_NUMBER, &eBoardNumber);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /* Set the channel number */
    eStatus = PHX_ParameterSet(hCamera, PHX_CHANNEL_NUMBER, &eChannelNumber);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    ///* Set the configuration mode */
    //eStatus = PHX_ParameterSet(hCamera, PHX_CONFIG_MODE, &eConfigMode);
    //if (PHX_OK != eStatus) {
    //    _lastError = "camera configuration error";
    //    return false;
    //}


    // Allocate the board with the config file
    std::vector<char> writable(cameraConfigFile.begin(), cameraConfigFile.end());
    writable.push_back('\0'); // Add null terminator

    etCamConfigLoad configOptions = static_cast<etCamConfigLoad>(PHX_BOARD_AUTO | PHX_DIGITAL | eChannelNumber | PHX_NO_RECONFIGURE | 1);

    // Allocate the board with the config file
    eStatus = PHX_CameraConfigLoad(&hCamera, writable.data(), configOptions, &grabberErrorHandler);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /*manually load values*/
    eStatus = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    printf("initial phxROI_XLENGTH %u \n", dwImageWidth);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterSet(hCamera, PHX_ROI_XLENGTH, &camWidth);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    printf("initial phxROI_XLENGTH %u \n", dwImageWidth);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterSet(hCamera, PHX_ROI_YLENGTH, &camHeight);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_ROI_YLENGTH, &dwImageHeight);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    eStatus = PHX_ParameterSet(hCamera, PHX_CAM_ACTIVE_XLENGTH, &camWidth);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    ui32 w;
    eStatus = PHX_ParameterGet(hCamera, PHX_CAM_ACTIVE_XLENGTH, &w);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterSet(hCamera, PHX_CAM_ACTIVE_YLENGTH, &camHeight);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    ui32 h;
    eStatus = PHX_ParameterGet(hCamera, PHX_CAM_ACTIVE_YLENGTH, &h);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    //eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
    //if (PHX_OK != eStat) goto Error;
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_CAM_SRC_COL, &eCamSrcCol);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterSet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    eStatus = PHX_ParameterGet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eCamFormat = eCaptureFormat;
    eStatus = PHX_ParameterSet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_ACQ_TYPE, &eAcqType);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    //
    PHX_ParameterSet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    eStatus = PHX_ParameterGet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    //
        /* Determine PHX_BUS_FORMAT based on the camera format */
    eStatus = PhxCommonGetBusFormat(eCamSrcCol, dwCamSrcDepth, eCaptureFormat, &eCaptureFormat);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /* Determine bayer format */
    fDebayer = TRUE;
    if (PHX_CAM_SRC_BAYER_BG == eCamSrcCol) {
        eBayCol = PBL_BAY_COL_BLU;
    }
    else if (PHX_CAM_SRC_BAYER_GB == eCamSrcCol) {
        eBayCol = PBL_BAY_COL_GRNB;
    }
    else if (PHX_CAM_SRC_BAYER_GR == eCamSrcCol) {
        eBayCol = PBL_BAY_COL_GRNR;
    }
    else if (PHX_CAM_SRC_BAYER_RG == eCamSrcCol) {
        eBayCol = PBL_BAY_COL_RED;
    }
    else {
        fDebayer = FALSE;
    }


    /* Update the PHX_BUS_FORMAT, as it may have changed (above) */
    eStatus = PHX_ParameterSet(hCamera, (etParam)(PHX_BUS_FORMAT | PHX_CACHE_FLUSH), &eCaptureFormat);
    eStatus = PHX_ParameterSet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
    printf("final capture eCaptureFormat is %u \n", eCaptureFormat);
    printf("PHX_CAM_SRC_DEPTH %u \n", dwCamSrcDepth);
    printf("PHX_ACQ_NUM_BUFFERS=%u\n", dwBufferCount);  // Use %u for unsigned integers
    printf("initial capture eCaptureFormat is %u \n", eCaptureFormat);
    printf("roi_width=%u\n", dwImageWidth);  // Use %u for unsigned integers
    printf("roi_height=%u\n", dwImageHeight);  // Use %u for unsigned integers
    printf("active_width=%u\n", w);  // Use %u for unsigned integers
    printf("roi_height=%u\n", h);  // Use %u for unsigned integers
    printf("phx_buf_height=%u\n", dwBufferHeight);  // Use %u for unsigned integers
    printf("phx_buf_width=%u\n", dwBufferWidth);  // Use %u for unsigned integers


    /* Read back the Destination Buffer Width and Height */
    eStatus = PHX_ParameterSet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterSet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eStatus = PHX_ParameterGet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    /* Malloc space to hold the array of image pointers */
    psImageBuffers = (stImageBuff*)malloc((dwBufferCount + 1) * sizeof(stImageBuff));
    if (NULL == psImageBuffers) {
        printf("Failed to allocate memory for the array of stImage structures");
        
    }

    phCaptureBuffers = (tPHX*)malloc(dwBufferCount * sizeof(tPHX));
    if (NULL == phCaptureBuffers) {
        printf("Failed to allocate memory for the array of tPHX handles");
        
    }

    /* Create and initialise our capture buffers (not associated with display) */
    for (i = 0; i < (int)dwBufferCount; i++) {
        printf("adding buffer %i", i);
        /* We create a capture buffer for our double buffering */
        eStat = PBL_BufferCreate(&phCaptureBuffers[i], (etPblBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT,
            0, hCamera, PHX_ErrHandlerDefault);
        if (PHX_OK != eStatus) {
            _lastError = "camera configuration error";
            return false;
        }
        eStatus = PHX_ParameterSet(hCamera, PHX_ACQ_IMAGES_PER_BUFFER, &imperbuf);
        printf("images per buffer = %u \n", imperbuf);
        eStatus = PHX_ParameterSet(hCamera, PHX_ACQ_IMAGES_PER_BUFFER, &imperbuf);
        printf("images per buffer = %u \n", imperbuf);
        /* Initialise our capture buffer */
        eStatus = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_WIDTH, &dwImageWidth);
        if (PHX_OK != eStatus) {
            _lastError = "camera configuration error";
            return false;
        }
        eStatus = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_HEIGHT, &dwBufferHeight);  /* Use dwBufferHeight to take PHX_ACQ_IMAGES_PER_BUFFER count into account */
        if (PHX_OK != eStatus) {
            _lastError = "camera configuration error";
            return false;
        }

        eStatus = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_STRIDE, &dwBufferStride);
        if (PHX_OK != eStatus) {
            _lastError = "camera configuration error";
            return false;
        }
        eStatus = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_DST_FORMAT, &eCaptureFormat);
        if (PHX_OK != eStatus) {
            _lastError = "camera configuration error";
            return false;
        }
        eStatus = PBL_BufferInit(phCaptureBuffers[i]);
        if (PHX_OK != eStatus) {
            _lastError = "camera configuration error";
            return false;
        }

        /* Set up the type of Bayer conversion required */
        if (fDebayer) {
            ui32 dwParamValue = PBL_BAY_DEC_DUP;
            eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_BAYDEC, &dwParamValue);
            if (PHX_OK != eStatus) {
                _lastError = "camera configuration error";
                return false;
            }
            eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_BAYCOL, &eBayCol);
            if (PHX_OK != eStatus) {
                _lastError = "camera configuration error";
                return false;
            }
        }

        /* Build up our array of capture buffers */
        PBL_BufferParameterGet(phCaptureBuffers[i], PBL_BUFF_ADDRESS, &psImageBuffers[i].pvAddress);
        psImageBuffers[i].pvContext = (void*)phCaptureBuffers[i];
    }
    /* Terminate the array */
    psImageBuffers[dwBufferCount].pvAddress = NULL;
    psImageBuffers[dwBufferCount].pvContext = NULL;

    /* Using virtual buffers */
    eStatus = PHX_ParameterSet(hCamera, PHX_DST_PTRS_VIRT, psImageBuffers);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }
    eParamValue = PHX_DST_PTR_USER_VIRT;
    eStatus = PHX_ParameterSet(hCamera, (etParam)(PHX_DST_PTR_TYPE | PHX_CACHE_FLUSH | PHX_FORCE_REWRITE), &eParamValue);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }


#if defined _PHX_DISPLAY
    /* We create our display with a NULL hWnd, this will automatically create an image window. */
    eStatus = PDL_DisplayCreate(&hDisplay, NULL, hCamera, PhxCommonDisplayErrorHandler);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /* We create a display buffer (indirect) */
    eStatus = PDL_BufferCreate(&hDisplayBuffer, hDisplay, (etBufferMode)PDL_BUFF_SYSTEM_MEM_INDIRECT);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /* Initialise the display, this associates the display buffer with the display */
    eStatus = PDL_DisplayInit(hDisplay);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

#endif

    /* Enable FIFO Overflow events */
    eParamValue = PHX_INTRPT_FIFO_OVERFLOW;
    eStatus = PHX_ParameterSet(hCamera, PHX_INTRPT_SET, &eParamValue);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }

    /* Setup our own event context */
    eStatus = PHX_ParameterSet(hCamera, PHX_EVENT_CONTEXT, &sPhxLive);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }


    /* Now start our capture, using the callback method */
    eStatus = PHX_StreamRead(hCamera, PHX_START, phxlive_callback);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuration error";
        return false;
    }


    /* Allocate global buffer */
    globalBuffer = (uint16_t*)malloc(globalBufferWidth * globalBufferHeight * sizeof(uint16_t));
    if (globalBuffer == NULL) {
        printf("Failed to allocate global buffer\n");
        
    }
    //// Create display with a NULL hWnd, this will automatically create an image window.
    //eStatus = PDL_DisplayCreate(&hDisplay, NULL, hCamera, &grabberErrorHandler);
    //if (PHX_OK != eStatus) {
    //    _lastError = "display creation error";
    //    return false;
    //}

    //// Create two display buffers for double buffering
    //eStatus = PDL_BufferCreate(&hBuffer1, hDisplay, (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT);
    //if (PHX_OK != eStatus) {
    //    _lastError = "buffer allocation error: buffer 1";
    //    return false;
    //}

    //eStatus = PDL_BufferCreate(&hBuffer2, hDisplay, (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT);
    //if (PHX_OK != eStatus) {
    //    _lastError = "buffer allocation error: buffer 2";
    //    return false;
    //}

    //// Initialize the display, this associates the display buffers with the display
    //eStatus = PDL_DisplayInit(hDisplay);
    //if (PHX_OK != eStatus) {
    //    _lastError = "display initialization error";
    //    return false;
    //}

    //int AcquireNr = 2;
    //eStatus = PHX_ParameterSet(hCamera, PHX_ACQ_NUM_IMAGES, &AcquireNr);
    //if (PHX_OK != eStatus) {
    //    _lastError = "Parameter error: Acquired number of images";
    //    return false;
    //}

    //// Enable FIFO Overflow events
    //eParamValue = PHX_INTRPT_FIFO_OVERFLOW;
    //eStatus = PHX_ParameterSet(hCamera, PHX_INTRPT_SET, &eParamValue);
    //if (PHX_OK != eStatus) {
    //    _lastError = "Parameter error: FIFO Overflow interpreter";
    //    return false;
    //}

    //// Setup an event context
    //eStatus = PHX_ParameterSet(hCamera, PHX_EVENT_CONTEXT, (void*)&eventContext);
    //if (PHX_OK != eStatus) {
    //    _lastError = "Parameter error: Custom Event Context";
    //    return false;
    //}

    //// Now start capture, using the callback method
    //eStatus = PHX_Acquire(hCamera, PHX_START, (void*)handleInterruptEvents);
    //if (PHX_OK != eStatus) {
    //    _lastError = "starting capture error";
    //    return false;
    //}

    return true;
}

bool PHXGrabberInterface::livePicture() {
    etStat eStatus = PHX_OK;
    ui32 dwBufferReadyLast = 0; // Previous BufferReady count value

    messageOutput("Starting Live Picture!...\n");
    updateEventCounter(0);
    /* Continue processing data until the user presses a stops the acquisition
     * or a FIFO overflow is detected
     */
    int c = 0;

    /*save pcf to file*/
    PHX_Action(hCamera, PHX_CONFIG_SAVE, PHX_SAVE_ALL, "Sens128016bit_grabber.pcf");

    while (!sPhxLive.fFifoOverflow && !stop_loop) {
        stImageBuff    stBuffer;
        /* Wait here until either:
         * (a) The user aborts the wait bychanging the bool
         * (b) The BufferReady event occurs indicating that the image is complete
         * (c) The FIFO overflow event occurs indicating that the image is corrupt.
         * Keep calling the sleep function to avoid burning CPU cycles
         */

        while (!sPhxLive.fBufferReady && !sPhxLive.fFifoOverflow) {
            _PHX_SleepMs(10);
            /*printf("fBufferReady: %d, fFifoOverflow: %d\n",
                sPhxLive.fBufferReady, sPhxLive.fFifoOverflow);*/
                //    _PHX_SleepMs(10);
                //    printf("fBufferReady: %d, fFifoOverflow: %d, stop_loop: %d\n",
                //        sPhxLive.fBufferReady, sPhxLive.fFifoOverflow, stop_loop);
        }
        //printf("\n buffer ready \n");
        if (dwBufferReadyLast != sPhxLive.dwBufferReadyCount) {
            ui32 dwStaleBufferCount;
            /* If the processing is too slow to keep up with acquisition,
             * then there may be more than 1 buffer ready to process.
             * The application can either be designed to process all buffers
             * knowing that it will catch up, or as here, throw away all but the
             * latest
             */
            dwStaleBufferCount = sPhxLive.dwBufferReadyCount - dwBufferReadyLast;
            dwBufferReadyLast += dwStaleBufferCount;

            /* Throw away all but the last image */
            if (1 < dwStaleBufferCount) {
                do {
                    eStatus = PHX_StreamRead(hCamera, PHX_BUFFER_RELEASE, NULL);
                    if (PHX_OK != eStatus) {
                        _lastError = "stream read error";
                        return false;
                    }
                    dwStaleBufferCount--;
                } while (dwStaleBufferCount > 1);
            }
        }

        sPhxLive.fBufferReady = FALSE;

        /* Get the info for the last acquired buffer */
        eStatus = PHX_StreamRead(hCamera, PHX_BUFFER_GET, &stBuffer);
        if (PHX_OK != eStatus) {
            _lastError = "camera get buffer error";
            return false;
        }

        /* Process the newly acquired buffer,
         * which in this simple example is a call to display the data.
         * For our display function we use the pvContext member variable to
         * pass a display buffer handle.
         * Alternatively the actual video data can be accessed at stBuffer.pvAddress
         *
         */
        hBuffHandle = (tPHX)stBuffer.pvContext;

        /* This copies/converts data from the direct capture buffer to the indirect display buffer */
        eStatus = PIL_Convert(hBuffHandle, hDisplayBuffer);
        if (PHX_OK != eStatus)
            printf("\nFailed to convert buffers\n");

#if defined _PHX_DISPLAY
        if (PHX_OK == eStatus)
            PDL_BufferPaint(hDisplayBuffer);
#else
        printf("EventCount = %5d\r", sPhxLive.dwBufferReadyCount);
#endif

        if (read_buffer) {
            printf("buffer accessed\n");
            memcpy(globalBuffer, stBuffer.pvAddress, globalBufferWidth * globalBufferHeight * sizeof(uint16_t));
            //memcpy(globalBuffer, psImageBuffers[0].pvAddress, globalBufferWidth*globalBufferHeight);
            read_buffer = false;
        }
        /* Having processed the data, release the buffer ready for further image data */
        eStatus = PHX_StreamRead(hCamera, PHX_BUFFER_RELEASE, NULL);
        if (PHX_OK != eStatus)
            printf("\nFailed to release buffer\n");

        if (save_config) {
            PHX_Action(hCamera, PHX_CONFIG_SAVE, PHX_SAVE_ALL, "NIT.pcf");
        }


        // printf("\nFinished looping 409\n");
        c++;
    }
    if (eventContext.fFifoOverFlow) {
        _lastError = "FIFO OverFlow detected";
        return false;
    }

    return true;
}

void PHXGrabberInterface::releaseGrabber() {
    updateEventCounter((int)eventContext.nBufferReadyCount);

    // stop all captures
    if (hCamera) PHX_Acquire(hCamera, PHX_ABORT, NULL);

    // free buffers
    if (hBuffer1) PDL_BufferDestroy((tPHX*)&hBuffer1);
    if (hBuffer2) PDL_BufferDestroy((tPHX*)&hBuffer2);

    // free display
    if (hDisplay) PDL_DisplayDestroy((tPHX*)&hDisplay);

    // release grabber board
    if (hCamera) PHX_CameraRelease(&hCamera);

    PhxCommonKbClose();
    close();
    messageOutput("Grabber released\n\n - - - - - - - - - - - - - - - \n");
}

void PHXGrabberInterface::messageOutput(const std::string& message) {
    std::cout << message << std::endl;
}

void PHXGrabberInterface::updateEventCounter(int count) {
    // Implement the event counter update if needed
}