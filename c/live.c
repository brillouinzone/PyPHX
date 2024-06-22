/****************************************************************************
 *
 * Active Silicon
 *
 * Project     : SDK
 * Description : Simple acquisition and display application
 * Updated     : 21-Jun-2017
 *
 * Copyright (c) 2019 Active Silicon
 ****************************************************************************
 * Comments:
 * --------
 * This example shows how to initialise the frame grabber and use the Display
 * library to run live double buffered (also known as ping-pong) acquisition,
 * using a callback function.
 * It also shows how to use the image conversion function. It captures into
 * a direct buffer, and then converts the image data into a format suitable
 * for display. This reduces the amount of PCI bandwidth used.
 *
 ****************************************************************************
 */

 /* Define to use the PHX Display library */
#if defined _PHX_WIN32
#define _PHX_DISPLAY
#endif

/* Application & library headers */
#include "common/common.h"
#include "live.h"
#include <stdbool.h>
#include <stdint.h>


///*Globals*/
/*Globals*/
volatile bool stop_loop = false;
volatile bool prevent_overwrite = false;
volatile bool read_buffer = false;
volatile bool save_config = false;
uint16_t* globalBuffer = NULL;
/*this size used when transferring the capture buffer*/
/*multiply by 2 if 14 or 16 bit image*/
uint32_t globalBufferWidth = 1280;
uint32_t globalBufferHeight = 1020;

/* Function to get the buffer address */
_declspec(dllexport) void lock_buffer(volatile bool value) {
    prevent_overwrite = value;
    printf("set mem lock to %s \n", prevent_overwrite ? "true" : "false");
}
/* Function to get the buffer address */
__declspec(dllexport) uint16_t* get_buffer_address() {
    return globalBuffer;
}

/* Function to get the buffer width */
__declspec(dllexport) uint32_t get_buffer_width() {
    return globalBufferWidth;
}

/* Function to get the buffer height */
__declspec(dllexport) uint32_t get_buffer_height() {
    return globalBufferHeight;
}
__declspec(dllexport) void stop_looping() {
    stop_loop = true;
}

__declspec(dllexport) void access_buffer() {
    read_buffer = true;
}

__declspec(dllexport) void write_config() {
    save_config = true;
}
/*
phxlive_callback()
 * This is the callback function which handles the interrupt events.
 */
static void phxlive_callback(
    tHandle  hCamera,          /* Camera handle */
    ui32     dwInterruptMask,  /* Interrupt mask */
    void* pvParams          /* Pointer to user supplied context */
)
{
    tPhxLive* psPhxLive = (tPhxLive*)pvParams;

    (void)hCamera;

    /* Handle the Buffer Ready event */
    if (PHX_INTRPT_BUFFER_READY & dwInterruptMask) {
        /* Increment the Display Buffer Ready Count */
        psPhxLive->dwBufferReadyCount++;
        psPhxLive->fBufferReady = TRUE;
    }

    /* Fifo Overflow */
    if (PHX_INTRPT_FIFO_OVERFLOW & dwInterruptMask)
        psPhxLive->fFifoOverflow = TRUE;

    /* Note:
     * The callback routine may be called with more than 1 event flag set.
     * Therefore all possible events must be handled here.
     */
}


/*
phxlive()
 * Simple live capture application code, using image conversion in order to reduce
 * the amount of PCI bandwidth used.
 */
int phxlive(
    etParamValue   eBoardNumber,        /* Board number, i.e. 1, 2, or 0 for next available */
    etParamValue   eChannelNumber,      /* Channel number */
    char* pszConfigFileName,   /* Name of config file */
    tCxpRegisters  sCameraRegs,         /* Camera CXP registers */
    etParamValue   eConfigMode          /* PHX Configuration mode */
)
{
    etStat         eStat = PHX_OK;      /* Status variable */
    etParamValue   eParamValue;                              /* Parameter for use with PHX_ParameterSet/Get calls */
    etParamValue   eCaptureFormat = PHX_BUS_FORMAT_MONO8;
    etParamValue   eCamSrcCol, eCamFormat, eAcqType, eBayCol;
    stImageBuff* psImageBuffers = NULL;        /* Capture buffer array */
    tPhxLive       sPhxLive;                                 /* User defined event context */
    tHandle        hCamera = 0;           /* Camera handle */
    tPHX           hDisplay = 0;           /* Display handle */
    tPHX* phCaptureBuffers = NULL;        /* Capture buffer handle array */
    //tPHX           hDisplayBuffer = 0;           /* Display buffer handle */
    tPHX           hBuffHandle;
    tFlag          fDebayer = FALSE;
    ui32           dwBufferReadyLast = 0;           /* Previous BufferReady count value */
    ui32           dwImageWidth, dwImageHeight;
    ui32           dwBufferWidth, dwBufferHeight;
    ui32           dwCamSrcDepth;
    ui32           dwBufferCount = 0;
    tFlag          fCameraIsCxp = FALSE;
    tFlag          fIsCxpCameraDiscovered = FALSE;
    int            i;

    /* Initialise the user defined Event context structure */
    memset(&sPhxLive, 0, sizeof(tPhxLive));

    /* Create a channel handle */
    eStat = PHX_Create(&hCamera, PHX_ErrHandlerDefault);
    if (PHX_OK != eStat) goto Error;

    /* Set the configuration file name */
    eStat = PHX_ParameterSet(hCamera, PHX_CONFIG_FILE, &pszConfigFileName);
    if (PHX_OK != eStat) goto Error;

    /* Set the board number */
    eStat = PHX_ParameterSet(hCamera, PHX_BOARD_NUMBER, &eBoardNumber);
    if (PHX_OK != eStat) goto Error;

    /* Set the channel number */
    eStat = PHX_ParameterSet(hCamera, PHX_CHANNEL_NUMBER, &eChannelNumber);
    if (PHX_OK != eStat) goto Error;

    /* Set the configuration mode */
    eStat = PHX_ParameterSet(hCamera, PHX_CONFIG_MODE, &eConfigMode);
    if (PHX_OK != eStat) goto Error;

    /* Open the board using the above configuration file */
    eStat = PHX_Open(hCamera);
    if (PHX_OK != eStat) goto Error;

    eStat = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_YLENGTH, &dwImageHeight);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_COL, &eCamSrcCol);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ACQ_TYPE, &eAcqType);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    if (PHX_OK != eStat) goto Error;

    /* Interlaced Camera in Field Mode */
    if (PHX_CAM_INTERLACED == eCamFormat &&
        (PHX_ACQ_FIELD_12 == eAcqType || PHX_ACQ_FIELD_21 == eAcqType ||
            PHX_ACQ_FIELD_NEXT == eAcqType ||
            PHX_ACQ_FIELD_1 == eAcqType || PHX_ACQ_FIELD_2 == eAcqType)) {
        dwImageHeight /= 2;
    }

    /* Determine PHX_BUS_FORMAT based on the camera format */
    eStat = PhxCommonGetBusFormat(eCamSrcCol, dwCamSrcDepth, eCaptureFormat, &eCaptureFormat);
    if (PHX_OK != eStat) goto Error;

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
    eStat = PHX_ParameterSet(hCamera, (etParam)(PHX_BUS_FORMAT | PHX_CACHE_FLUSH), &eCaptureFormat);
    if (PHX_OK != eStat) goto Error;

    /* Read back the Destination Buffer Width and Height */
    eStat = PHX_ParameterGet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStat) goto Error;

    /* Malloc space to hold the array of image pointers */
    psImageBuffers = (stImageBuff*)malloc((dwBufferCount + 1) * sizeof(stImageBuff));
    if (NULL == psImageBuffers) {
        printf("Failed to allocate memory for the array of stImage structures");
        goto Error;
    }

    phCaptureBuffers = (tPHX*)malloc(dwBufferCount * sizeof(tPHX));
    if (NULL == phCaptureBuffers) {
        printf("Failed to allocate memory for the array of tPHX handles");
        goto Error;
    }

    /* Create and initialise our capture buffers (not associated with display) */
    for (i = 0; i < (int)dwBufferCount; i++) {
        /* We create a capture buffer for our double buffering */
        eStat = PBL_BufferCreate(&phCaptureBuffers[i], (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT,
            0, hCamera, PHX_ErrHandlerDefault);
        if (PHX_OK != eStat) goto Error;

        /* Initialise our capture buffer */
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_WIDTH, &dwImageWidth);
        if (PHX_OK != eStat) goto Error;
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_HEIGHT, &dwBufferHeight);  /* Use dwBufferHeight to take PHX_ACQ_IMAGES_PER_BUFFER count into account */
        if (PHX_OK != eStat) goto Error;
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_STRIDE, &dwBufferWidth);
        if (PHX_OK != eStat) goto Error;
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_DST_FORMAT, &eCaptureFormat);
        if (PHX_OK != eStat) goto Error;
        eStat = PBL_BufferInit(phCaptureBuffers[i]);
        if (PHX_OK != eStat) goto Error;

        /* Set up the type of Bayer conversion required */
        if (fDebayer) {
            ui32 dwParamValue = PBL_BAY_DEC_DUP;
            eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_BAYDEC, &dwParamValue);
            if (PHX_OK != eStat) goto Error;
            eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_BAYCOL, &eBayCol);
            if (PHX_OK != eStat) goto Error;
        }

        /* Build up our array of capture buffers */
        PBL_BufferParameterGet(phCaptureBuffers[i], PBL_BUFF_ADDRESS, &psImageBuffers[i].pvAddress);
        psImageBuffers[i].pvContext = (void*)phCaptureBuffers[i];
    }
    /* Terminate the array */
    psImageBuffers[dwBufferCount].pvAddress = NULL;
    psImageBuffers[dwBufferCount].pvContext = NULL;

    /* Using virtual buffers */
    eStat = PHX_ParameterSet(hCamera, PHX_DST_PTRS_VIRT, psImageBuffers);
    if (PHX_OK != eStat) goto Error;
    eParamValue = PHX_DST_PTR_USER_VIRT;
    eStat = PHX_ParameterSet(hCamera, (etParam)(PHX_DST_PTR_TYPE | PHX_CACHE_FLUSH | PHX_FORCE_REWRITE), &eParamValue);
    if (PHX_OK != eStat) goto Error;

#if defined _PHX_DISPLAY
    /* We create our display with a NULL hWnd, this will automatically create an image window. */
    //eStat = PDL_DisplayCreate(&hDisplay, NULL, hCamera, PhxCommonDisplayErrorHandler);
    //if (PHX_OK != eStat) goto Error;

    ///* We create a display buffer (indirect) */
    //eStat = PDL_BufferCreate(&hDisplayBuffer, hDisplay, (etBufferMode)PDL_BUFF_SYSTEM_MEM_INDIRECT);
    //if (PHX_OK != eStat) goto Error;

    ///* Initialise the display, this associates the display buffer with the display */
    //eStat = PDL_DisplayInit(hDisplay);
    //if (PHX_OK != eStat) goto Error;

#endif

    /* Enable FIFO Overflow events */
    eParamValue = PHX_INTRPT_FIFO_OVERFLOW;
    eStat = PHX_ParameterSet(hCamera, PHX_INTRPT_SET, &eParamValue);
    if (PHX_OK != eStat) goto Error;

    /* Setup our own event context */
    eStat = PHX_ParameterSet(hCamera, PHX_EVENT_CONTEXT, &sPhxLive);
    if (PHX_OK != eStat) goto Error;

    /* Check if camera is CXP */
    eStat = PhxCommonIsCxp(hCamera, &fCameraIsCxp);
    if (PHX_OK != eStat) goto Error;

    /* Check that camera is discovered (only applies to CXP) */
    if (fCameraIsCxp) {
        PhxCommonGetCxpDiscoveryStatus(hCamera, 10, &fIsCxpCameraDiscovered);
        if (!fIsCxpCameraDiscovered) {
            goto Error;
        }
        eStat = PhxCommonUpdateCxpRegs(hCamera, &sCameraRegs);
        if (PHX_OK != eStat) goto Error;
    }

    /* Now start our capture, using the callback method */
    eStat = PHX_StreamRead(hCamera, PHX_START, phxlive_callback);
    if (PHX_OK != eStat) goto Error;

    /* Now start camera */
    if (fCameraIsCxp && sCameraRegs.dwAcqStartAddress) {
        eStat = PhxCommonWriteCxpReg(hCamera, sCameraRegs.dwAcqStartAddress, sCameraRegs.dwAcqStartValue, 800);
        if (PHX_OK != eStat) goto Error;
    }

    /* Continue processing data until the user presses a key in the console window
     * or a FIFO overflow is detected
     */


     /* Allocate global buffer */
    globalBuffer = (uint16_t*)malloc(globalBufferWidth * globalBufferHeight * sizeof(uint16_t));
    if (globalBuffer == NULL) {
        printf("Failed to allocate global buffer\n");
        goto Error;
    }

    int c = 0;

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
                    printf(("throwing away stale buffer %d \n", dwStaleBufferCount));
                    eStat = PHX_StreamRead(hCamera, PHX_BUFFER_RELEASE, NULL);
                    if (PHX_OK != eStat) goto Error;
                    dwStaleBufferCount--;
                } while (dwStaleBufferCount > 1);
            }
        }

        sPhxLive.fBufferReady = FALSE;

        /* Get the info for the last acquired buffer */
        eStat = PHX_StreamRead(hCamera, PHX_BUFFER_GET, &stBuffer);
        if (PHX_OK != eStat)
            printf("\nFailed to StreamRead last aquired buffer\n");

        /* Process the newly acquired buffer,
         * which in this simple example is a call to display the data.
         * For our display function we use the pvContext member variable to
         * pass a display buffer handle.
         * Alternatively the actual video data can be accessed at stBuffer.pvAddress
         *
         */
        hBuffHandle = (tPHX)stBuffer.pvContext;

        ///* This copies/converts data from the direct capture buffer to the indirect display buffer */
        //eStat = PIL_Convert(hBuffHandle, hDisplayBuffer);
        //if (PHX_OK != eStat)
        //    printf("\nFailed to convert buffers\n");
//
//#if defined _PHX_DISPLAY
//        if (PHX_OK == eStat)
//            PDL_BufferPaint(hDisplayBuffer);
//#else
//        printf("EventCount = %5d\r", sPhxLive.dwBufferReadyCount);
//#endif
        if (!prevent_overwrite) {
            if (read_buffer) {
                memcpy(globalBuffer, stBuffer.pvAddress, globalBufferWidth * globalBufferHeight * sizeof(uint16_t));
                //memcpy(globalBuffer, psImageBuffers[0].pvAddress, globalBufferWidth*globalBufferHeight);
                read_buffer = false;
            }
        }

        /* Having processed the data, release the buffer ready for further image data */
        eStat = PHX_StreamRead(hCamera, PHX_BUFFER_RELEASE, NULL);
        if (PHX_OK != eStat)
            printf("\nFailed to release buffer\n");

        if (save_config) {
            PHX_Action(hCamera, PHX_CONFIG_SAVE, PHX_SAVE_ALL, "NIT.pcf");
        }


        // printf("\nFinished looping 409\n");
        c++;
    }

    if (stop_loop) {
        //printf("loop canceled externally,..Aborting\n");
        goto Error;
    }
    //printf("\n");

    /* In this simple example we abort the processing loop on an error condition (FIFO overflow).
     * However handling of this condition is application specific, and generally would involve
     * aborting the current acquisition, and then restarting.
     */
    if (sPhxLive.fFifoOverflow)
        printf("FIFO OverFlow detected..Aborting\n");

Error:
    /* Now cease all captures */
    if (hCamera) {
        /* Stop camera */
        if (fIsCxpCameraDiscovered && sCameraRegs.dwAcqStopAddress) {
            PhxCommonWriteCxpReg(hCamera, sCameraRegs.dwAcqStopAddress, sCameraRegs.dwAcqStopValue, 800);
        }
        /* Stop frame grabber */
        PHX_StreamRead(hCamera, PHX_ABORT, NULL);
    }

    for (i = 0; i < (int)dwBufferCount; i++)
        if (phCaptureBuffers[i]) PBL_BufferDestroy(&phCaptureBuffers[i]);

#if defined _PHX_DISPLAY
    /* Free our display resources */
    //if (hDisplayBuffer) PDL_BufferDestroy(&hDisplayBuffer);

    /* Destroy our display */
    if (hDisplay) PDL_DisplayDestroy(&hDisplay);
#endif

    /* Free remaining resources */
    if (psImageBuffers) free(psImageBuffers);
    if (phCaptureBuffers) free(phCaptureBuffers);

    /* Release the board */
    if (hCamera) {
        /* Close the board */
        PHX_Close(&hCamera);

        /* Destroy the handle */
        PHX_Destroy(&hCamera);
    }

    printf("Exiting\n");
    return 0;
}

/* VxWorks requires a unique function name for each application */
//#if !defined _PHX_VXWORKS
//int main()
//{
//   //tCxpRegisters  sCameraRegs;
//   //tPhxCmd        sPhxCmd;
//   int            nStatus = -1;
//
//   //PhxCommonParseCmd( argc, argv, &sPhxCmd );
//    tPhxCmd sPhxCmd;
//    tCxpRegisters sCameraRegs;
//
//    // Set your saved variables
//    int boardNumber = PHX_BOARD_NUMBER_AUTO;
//    int channelNumber = PHX_CHANNEL_NUMBER_AUTO;
//    const char* configFileName = "C:/Users/User/PycharmProjects/PyPHX/tests/Sens128016bit.pcf";
//    const char* outputFileName = "as_file";
//    const char* recordingFormat = "avi";
//    int recordingFrameRate = 25;
//    int recordingQualityFactor = 30;
//    int recordingNumBuffers = 100;
//    int cxpBitRate = 0;
//    int cxpLinks = 0;
//    int configMode = 0;
//    int waitForKill = 0;
//
//    // Set the PhxCmd structure
//    nStatus = set_phx_cmd(&sPhxCmd,
//        boardNumber,
//        channelNumber,
//        configFileName,
//        outputFileName,
//        recordingFormat,
//        recordingFrameRate,
//        recordingQualityFactor,
//        recordingNumBuffers,
//        cxpBitRate,
//        cxpLinks,
//        configMode,
//        waitForKill);
//    printf("phx command status = %u \n", nStatus);
//   memset( &sCameraRegs, 0, sizeof( sCameraRegs ) );
//   if ( NULL != sPhxCmd.pszConfigFileName ) {
//      if ( PHX_OK != PhxCommonParseCxpRegs( sPhxCmd.pszConfigFileName, &sCameraRegs ) ) {
//         return nStatus;
//      }
//   }
//
//   printf("set phxcmd ok, sarting live acquisition \n");
//   PhxCommonKbInit();
//   nStatus = phxlive( sPhxCmd.eBoardNumber, sPhxCmd.eChannelNumber, sPhxCmd.pszConfigFileName, sCameraRegs, sPhxCmd.eConfigMode );
//   PhxCommonKbClose();
//   return nStatus;
//}
//#endif

//int live(char * file, uint16_t* globalBuffer, uint32_t globalBufferWidth, uint32_t globalBufferHeight, bool stop_loop, bool read_buffer, bool save_config)
//{
//    tCxpRegisters  sCameraRegs;
//    tPhxCmd        sPhxCmd;
//    int            nStatus = -1;
//
//    PhxCommonSkipParseCmd(&sPhxCmd);
//
//    memset(&sCameraRegs, 0, sizeof(sCameraRegs));
//    if (NULL != sPhxCmd.pszConfigFileName) {
//        if (PHX_OK != PhxCommonParseCxpRegs(sPhxCmd.pszConfigFileName, &sCameraRegs)) {
//            return nStatus;
//        }
//    }
//
//    printf("set phxcmd ok, sarting live acquisition \n");
//    PhxCommonKbInit();
//    nStatus = phxlive(sPhxCmd.eBoardNumber, sPhxCmd.eChannelNumber, file, sCameraRegs, sPhxCmd.eConfigMode, uint16_t * globalBuffer, uint32_t globalBufferWidth, uint32_t globalBufferHeight, bool stop_loop, bool read_buffer, bool save_config);
//    PhxCommonKbClose();
//    return nStatus;
//}

int live(char* file) {
    tCxpRegisters sCameraRegs;
    tPhxCmd sPhxCmd;
    int nStatus = -1;

    PhxCommonSkipParseCmd(&sPhxCmd);

    memset(&sCameraRegs, 0, sizeof(sCameraRegs));
    if (NULL != sPhxCmd.pszConfigFileName) {
        if (PHX_OK != PhxCommonParseCxpRegs(sPhxCmd.pszConfigFileName, &sCameraRegs)) {
            return nStatus;
        }
    }

    printf("set phxcmd ok, starting live acquisition \n");
    PhxCommonKbInit();
    nStatus = phxlive(
        sPhxCmd.eBoardNumber,
        sPhxCmd.eChannelNumber,
        file,
        sCameraRegs,
        sPhxCmd.eConfigMode);
    PhxCommonKbClose();
    return nStatus;
}