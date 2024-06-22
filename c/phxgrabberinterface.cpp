#include "phxgrabberinterface.h"
#include "common/common.h"
#include "live.h"
#if defined _PHX_WIN32
#define _PHX_DISPLAY
#endif
#include "pdl_api.h"
#include "pbl_api.h"
#include "phx_api.h"
#include "phx_os.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "pil_api.h"

#define MAX_SZ_CHARS 256   /* Maximum string length */




///* Function to get the buffer address */

/* Function to get the buffer address */
//uint16_t* PHXGrabberInterface::get_buffer_address() {
//    return globalBuffer;
//}
//
///* Function to get the buffer width */
//uint32_t PHXGrabberInterface::get_buffer_width() {
//    return globalBufferWidth;
//}
//
///* Function to get the buffer height */
//uint32_t PHXGrabberInterface::get_buffer_height() {
//    return globalBufferHeight;
//}

//void PHXGrabberInterface::setStopLoop(volatile bool* stop) {
//    stop_loop = stop;
//    printf("DLL received stop loop command from external code\n");
//}
//
//// Add the method declaration to the class
//void setStopLoop(volatile bool* stop);

/* Create a channel handle */
static void phxlive_callback(tHandle hCamera, ui32 dwInterruptMask, void* pvParams) {
    tPhxLive* psPhxLive = (tPhxLive*)pvParams;
    (void)hCamera;
    /* Handle the Buffer Ready event */
    if (PHX_INTRPT_BUFFER_READY & dwInterruptMask) {
        /* Increment the Display Buffer Ready Count */
        psPhxLive->dwBufferReadyCount++;
        psPhxLive->fBufferReady = TRUE;
    }
    // printf("Buffer ready, count=%u\n", psPhxLive->dwBufferReadyCount);
    if (PHX_INTRPT_FIFO_OVERFLOW & dwInterruptMask) {
        psPhxLive->fFifoOverflow = TRUE;
        printf("FIFO overflow detected\n");
    }
}

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

    bool oState = configureGrabberLegacy();

    setState(oState);

 //   if (isOpened())
   //     oState = livePicture();

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
int PHXGrabberInterface::live(char* file) {
    return ::live(file);
}
bool PHXGrabberInterface::configureGrabberLegacy() {

    char* cCameraConfigFile = new char[cameraConfigFile.length() + 1];
    std::strcpy(cCameraConfigFile, cameraConfigFile.c_str());
    int status = live(
        cCameraConfigFile
    );
    delete[] cCameraConfigFile;
    return status == 0;
    return 0;
}
bool PHXGrabberInterface::configureGrabber() {
    etStat         eStatus = PHX_OK;
    etParamValue   eParamValue;          // Parameter for use with PHX_ParameterSet/Get calls
    etStat         eStat = PHX_OK;      /* Status variable */
    etParamValue   eCaptureFormat = PHX_BUS_FORMAT_MONO16;
    ui32           dwCamSrcDepth = 16; /*make sure this value matches to the param number*/
    etParamValue   eCamSrcCol, eCamFormat, eAcqType, eBayCol;
    stImageBuff* psImageBuffers = NULL;        /* Capture buffer array */
    tPhxLive       sPhxLive;                                 /* User defined event context */
    tHandle        hCamera = 0;           /* Camera handle */
    tPHX           hDisplay = 0;           /* Display handle */
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

    hCamera = 0;
    hDisplay = 0;
    hBuffer1 = 0;
    hBuffer2 = 0;

   /* char* argv[1] = { nullptr };

    PhxCommonParseCmd(0, argv, &phxGrabberInfo);*/
    PhxCommonKbInit();

    memset(&eventContext, 0, sizeof(eventContext));

    //phxGrabberInfo.eCamConfigLoad = (etCamConfigLoad) ( PHX_DIGITAL | PHX_ACQ_ONLY | PHX_CHANNEL_AUTO );
    etCamConfigLoad configOptions = static_cast<etCamConfigLoad>(PHX_BOARD_NUMBER_1 | PHX_CHANNEL_NUMBER_1| PHX_DIGITAL | PHX_ACQ_ONLY | PHX_CHANNEL_AUTO);
    phxGrabberInfo.eBoardNumber = eBoardNumber;
    phxGrabberInfo.eChannelNumber = eChannelNumber;
    // Allocate the board with the config file
    char* cstr = new char[cameraConfigFile.length() + 1]; // +1 for the null terminator
    // Copy the contents of the string to the character array
    std::strcpy(cstr, cameraConfigFile.c_str());
    eStatus = PHX_CameraConfigLoad(&hCamera,
        cstr,
        configOptions,
        &grabberErrorHandler);
    if (PHX_OK != eStatus) {
        _lastError = "camera configuaration error";
        return false;
    }

    // Create display with a NULL hWnd, this will automatically create an image window.
    eStatus = PDL_DisplayCreate(&hDisplay,
        NULL,
        hCamera,
        &grabberErrorHandler);
    if (PHX_OK != eStatus) {
        _lastError = "display creation error";
        return false;
    }

    // Create two display buffers for double buffering
    eStatus = PDL_BufferCreate(&hBuffer1,
        hDisplay,
        (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT);
    if (PHX_OK != eStatus) {
        _lastError = "buffer allocation error: buffer 1";
        return false;
    }

    eStatus = PDL_BufferCreate(&hBuffer2,
        hDisplay,
        (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT);
    if (PHX_OK != eStatus) {
        _lastError = "buffer allocation error: buffer 2";
        return false;
    }

    // Initialise the display, this associates the display buffers with the display
    eStatus = PDL_DisplayInit(hDisplay);
    if (PHX_OK != eStatus) {
        _lastError = "display initialization error";
        return false;
    }

    int AcquireNr = 1;
    eStatus = PHX_ParameterSet(hCamera, PHX_ACQ_NUM_IMAGES, &AcquireNr);
    if (PHX_OK != eStatus) {
        _lastError = "Parameter error: Acquired number of images";
        return false;
    }

    // Enable FIFO Overflow events
    eParamValue = PHX_INTRPT_FIFO_OVERFLOW;
    eStatus = PHX_ParameterSet(hCamera, PHX_INTRPT_SET, &eParamValue);
    if (PHX_OK != eStatus) {
        _lastError = "Parameter error: FIFO Overflow interpretor";
        return false;
    }

    // Setup an event context
    eStatus = PHX_ParameterSet(hCamera, PHX_EVENT_CONTEXT, (void*)&eventContext);
    if (PHX_OK != eStatus) {
        _lastError = "Parameter error: Custom Event Context";
        return false;
    }

    // Now start capture, using the callback method
    eStatus = PHX_Acquire(hCamera, PHX_START, (void*)handleInterruptEvents);
    if (PHX_OK != eStatus) {
        _lastError = "starting capture error";
        return false;
    }
    //printf("final capture eCaptureFormat is %u \n", eCaptureFormat);
    //printf("PHX_CAM_SRC_DEPTH %u \n", dwCamSrcDepth);
    //printf("PHX_ACQ_NUM_BUFFERS=%u\n", dwBufferCount);  // Use %u for unsigned integers
    //printf("initial capture eCaptureFormat is %u \n", eCaptureFormat);
    //printf("roi_width=%u\n", dwImageWidth);  // Use %u for unsigned integers
    //printf("roi_height=%u\n", dwImageHeight);  // Use %u for unsigned integers
    //printf("active_width=%u\n", w);  // Use %u for unsigned integers
    //printf("roi_height=%u\n", h);  // Use %u for unsigned integers
    //printf("phx_buf_height=%u\n", dwBufferHeight);  // Use %u for unsigned integers
    //printf("phx_buf_width=%u\n", dwBufferWidth);  // Use %u for unsigned integers

    return true;
}

bool PHXGrabberInterface::livePicture() {
    etStat         eStatus = PHX_OK;
    ui32           nBufferReadyLast = 0; // Previous BufferReady count value

    messageOutput("Starting Live Picture!...\n");
    updateEventCounter(0);

    // Continue processing data until the user presses "Stop" or Phoenix detects a FIFO overflow
    while (isOpened() && !eventContext.fFifoOverFlow)
    {
        _PHX_SleepMs(10);

        // If there are any buffers waiting to display, then process them
        if (nBufferReadyLast != eventContext.nBufferReadyCount) {
            stImageBuff stBuffer;
            int nStaleBufferCount;

            /* If the processing is too slow to keep up with acquisition,
             * then there may be more than 1 buffer ready to process.
             * The application can either be designed to process all buffers
             * knowing that it will catch up, or as here, throw away all but the
             * latest
             */
            nStaleBufferCount = eventContext.nBufferReadyCount - nBufferReadyLast;
            nBufferReadyLast += nStaleBufferCount;

            // Throw away all but the last image
            if (nStaleBufferCount > 1)
            {
                do
                {
                    eStatus = PHX_Acquire(hCamera, PHX_BUFFER_RELEASE, NULL);
                    if (PHX_OK != eStatus) {
                        _lastError = "butter releasing error";
                        return false;
                    }
                    nStaleBufferCount--;
                } while (nStaleBufferCount > 1);
            }

            // Get info for the last acquired buffer
            eStatus = PHX_Acquire(hCamera, PHX_BUFFER_GET, &stBuffer);
            if (PHX_OK != eStatus) {
                _lastError = "information error: last acquired buffer";
                return false;
            }

            // stBuffer.pvContext --> member variable to pass a display buffer handle
            // stBuffer.pvAddress --> access to the video data

            // Paint the image on the display
            PDL_BufferPaint((tPHX)stBuffer.pvContext);

            if (_useEventCounter)
                updateEventCounter((int)eventContext.nBufferReadyCount);

            // Release the buffer ready for further image data
            eStatus = PHX_Acquire(hCamera, PHX_BUFFER_RELEASE, NULL);
            if (PHX_OK != eStatus) {
                _lastError = "butter releasing error";
                return false;
            }
        }
    }

    /* In this simple example we abort the processing loop on an error condition (FIFO overflow).
     * However handling of this condition is application specific, and generally would involve
     * aborting the current acquisition, and then restarting.
     */
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

// External functions
PHXGrabberInterface* create_phxgrabber() {
    return new PHXGrabberInterface();
}

void destroy_phxgrabber(PHXGrabberInterface* handle) {
    delete handle;
}

void phxgrabber_open(PHXGrabberInterface* handle, const char* cameraConfigFile) {
    handle->open(cameraConfigFile);
}

void phxgrabber_close(PHXGrabberInterface* handle) {
    handle->close();
}

int phxgrabber_is_opened(PHXGrabberInterface* handle) {
    return handle->isOpened();
}

const char* phxgrabber_last_error(PHXGrabberInterface* handle) {
    return handle->lastError().c_str();
}

void phxgrabber_set_event_counter_usage(PHXGrabberInterface* handle, int useEventCounter) {
    handle->setEventCounterUsage(useEventCounter != 0);
}

//uint16_t* get_buffer_address(PHXGrabberInterface* handle) {
//    return handle->get_buffer_address();
//}
//
//uint32_t get_buffer_width(PHXGrabberInterface* handle) {
//    return handle->get_buffer_width();
//}
//
//uint32_t get_buffer_height(PHXGrabberInterface* handle) {
//    return handle->get_buffer_height();
//}