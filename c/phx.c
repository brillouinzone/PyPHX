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
#ifndef _PHX_USEDLLS
#define _PHX_USEDLLS
#endif

#include "common/common.h"
#include "ActiveSDK/include/phx_api.h"
#include "ActiveSDK/include/phx_os.h"
#include "ActiveSDK/include/Platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <windows.h>



/*Globals*/
volatile bool stop_loop = false;
volatile bool read_buffer = false;
volatile bool save_config = false;

tCxpRegisters sCameraRegs;
tPhxLive       sPhxLive;                                 /* User defined event context */
tHandle        hCamera = 0;           /* Camera handle */
tPHX           hDisplay = 0;           /* Display handle */
tPHX* phCaptureBuffers = NULL;        /* Capture buffer handle array */
tPHX           hDisplayBuffer = 0;           /* Display buffer handle */
tPHX           hBuffHandle;
etStat         eStat = PHX_OK;      /* Status variable */
etParamValue   eParamValue;                              /* Parameter for use with PHX_ParameterSet/Get calls */
etParamValue   eCaptureFormat = PHX_BUS_FORMAT_MONO16;
ui32           dwCamSrcDepth = 16; /*make sure this value matches to the param number*/
etParamValue   eCamSrcCol, eCamFormat, eAcqType, eBayCol;
stImageBuff* psImageBuffers = NULL;        /* Capture buffer array */
tFlag          fDebayer = FALSE;
ui32           dwBufferReadyLast = 0;           /* Previous BufferReady count value */
ui32           dwImageWidth = 1280;
ui32           dwImageHeight = 1020;
ui32           camWidth = 1280;
ui32           camHeight = 1020;
ui32           dwBufferWidth=2*1280;
ui32 dwBufferStride = 1*dwBufferWidth;
ui32           imperbuf = 1;
ui32           dwBufferHeight=imperbuf*1020;
ui32           dwBufferCount = 1;
tFlag          fCameraIsCxp = FALSE;
tFlag          fIsCxpCameraDiscovered = FALSE;

uint16_t* globalBuffer = NULL;
/*this size used when transferring the capture buffer*/
/*multiply by 2 if 14 or 16 bit image*/
uint32_t globalBufferWidth = 1280;
uint32_t globalBufferHeight = 1020;
/* Function to get the buffer address */

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
/* Create a channel handle */

__declspec(dllexport) etStat Create(tHandle* hCamera, etFctErrorHandler etfc) {
    etStat         eStat = PHX_OK;
    eStat = PHX_Create(hCamera, etfc);
    return eStat;
}

__declspec(dllexport) etStat Action(tHandle hCamera, etAction eta, etActionParam etap, void* variable) {
    etStat         eStat = PHX_OK;
    eStat = PHX_Action(hCamera, eta, etap, &variable);
    return eStat;
}

__declspec(dllexport) etStat Open(tHandle hCamera) {
    etStat         eStat = PHX_OK;
    eStat = PHX_Open(hCamera);
    return eStat;
}

__declspec(dllexport) etStat Close(tHandle* hCamera) {
    etStat         eStat = PHX_OK;
    eStat = PHX_Close(hCamera);
    return eStat;
}

__declspec(dllexport) etStat Destroy(tHandle* hCamera) {
    etStat         eStat = PHX_OK;
    eStat = PHX_Destroy(hCamera);
    return eStat;
}

__declspec(dllexport) void InitLive(tPhxLive * sPhxLive) {

    /* Initialise the user defined Event context structure */
    memset(&sPhxLive, 0, sizeof(tPhxLive));

}


__declspec(dllexport) int ParameterGet(tHandle hCamera, etParam p, ui32 * data) {
    etStat eStat = PHX_OK;
    eStat = PHX_ParameterGet(hCamera, p, data);
    return eStat;
}


__declspec(dllexport) int ParameterSet(tHandle hCamera, etParam p, ui32* data) {
    etStat eStat = PHX_OK;
    eStat = PHX_ParameterSet(hCamera, p, data);
    return eStat;
}


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
int phxsave (char * filename){
/*save pcf to file*/
    PHX_Action(hCamera, PHX_CONFIG_SAVE, PHX_SAVE_ALL, filename);
    return 0
}
int phxcreatebuffers (){
    /* Malloc space to hold the array of image pointers */
    psImageBuffers = (stImageBuff*)malloc((dwBufferCount+1) * sizeof(stImageBuff));
    if (NULL == psImageBuffers) {
        printf("Failed to allocate memory for the array of stImage structures");
        phxerror();
    }

    phCaptureBuffers = (tPHX*)malloc(dwBufferCount * sizeof(tPHX));
    if (NULL == phCaptureBuffers) {
        printf("Failed to allocate memory for the array of tPHX handles");
        phxerror();
    }

    /* Create and initialise our capture buffers (not associated with display) */
    for (i = 0; i < (int)dwBufferCount; i++) {
        printf("adding buffer %i", i);
        /* We create a capture buffer for our double buffering */
        eStat = PBL_BufferCreate(&phCaptureBuffers[i], (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT,
            0, hCamera, PHX_ErrHandlerDefault);
        if (PHX_OK != eStat) phxerror();
        eStat = PHX_ParameterSet(hCamera, PHX_ACQ_IMAGES_PER_BUFFER, &imperbuf);
        printf("images per buffer = %u \n", imperbuf);
        eStat = PHX_ParameterSet(hCamera, PHX_ACQ_IMAGES_PER_BUFFER, &imperbuf);
        printf("images per buffer = %u \n", imperbuf);
        /* Initialise our capture buffer */
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_WIDTH, &dwImageWidth);
        if (PHX_OK != eStat) phxerror();
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_HEIGHT, &dwBufferHeight);  /* Use dwBufferHeight to take PHX_ACQ_IMAGES_PER_BUFFER count into account */
        if (PHX_OK != eStat) phxerror();

        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_STRIDE, &dwBufferStride);
        if (PHX_OK != eStat) phxerror();
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_DST_FORMAT, &eCaptureFormat);
        if (PHX_OK != eStat) phxerror();
        eStat = PBL_BufferInit(phCaptureBuffers[i]);
        if (PHX_OK != eStat) phxerror();

        /* Set up the type of Bayer conversion required */
        if (fDebayer) {
            ui32 dwParamValue = PBL_BAY_DEC_DUP;
            eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_BAYDEC, &dwParamValue);
            if (PHX_OK != eStat) phxerror();
            eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_BAYCOL, &eBayCol);
            if (PHX_OK != eStat) phxerror();
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
    if (PHX_OK != eStat) phxerror();
    eParamValue = PHX_DST_PTR_USER_VIRT;
    eStat = PHX_ParameterSet(hCamera, (etParam)(PHX_DST_PTR_TYPE | PHX_CACHE_FLUSH | PHX_FORCE_REWRITE), &eParamValue);
    if (PHX_OK != eStat) phxerror();

    /* Enable FIFO Overflow events */
    eParamValue = PHX_INTRPT_FIFO_OVERFLOW;
    eStat = PHX_ParameterSet(hCamera, PHX_INTRPT_SET, &eParamValue);
    if (PHX_OK != eStat) phxerror();

    /* Setup our own event context */
    eStat = PHX_ParameterSet(hCamera, PHX_EVENT_CONTEXT, &sPhxLive);
    if (PHX_OK != eStat) phxerror();


    /* Now start our capture, using the callback method */
    eStat = PHX_StreamRead(hCamera, PHX_START, phxlive_callback);
    if (PHX_OK != eStat) phxerror();


    /* Allocate global buffer */
    globalBuffer = (uint16_t*)malloc(globalBufferWidth * globalBufferHeight * sizeof(uint16_t));
    if (globalBuffer == NULL) {
        printf("Failed to allocate global buffer\n");
        phxerror();
    }
   return 0;
}
int phxerror(){

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
    if (hDisplayBuffer) PDL_BufferDestroy(&hDisplayBuffer);

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
int phxdestroybuffers(){

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
    if (hDisplayBuffer) PDL_BufferDestroy(&hDisplayBuffer);

    /* Destroy our display */
    if (hDisplay) PDL_DisplayDestroy(&hDisplay);
#endif

    /* Free remaining resources */
    if (psImageBuffers) free(psImageBuffers);
    if (phCaptureBuffers) free(phCaptureBuffers);

    printf("Exiting\n");
    return 0;

}
int phxclose(){
    /* Release the board */
    if (hCamera) {
        /* Close the board */
        PHX_Close(&hCamera);

        /* Destroy the handle */
        PHX_Destroy(&hCamera);
    }
    return 0;
}
int phxstreamlive(){

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

        /* This copies/converts data from the direct capture buffer to the indirect display buffer */
        eStat = PIL_Convert(hBuffHandle, hDisplayBuffer);
        if (PHX_OK != eStat)
            printf("\nFailed to convert buffers\n");

#if defined _PHX_DISPLAY
        if (PHX_OK == eStat)
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
        eStat = PHX_StreamRead(hCamera, PHX_BUFFER_RELEASE, NULL);
        if (PHX_OK != eStat)
            printf("\nFailed to release buffer\n");

        if (save_config) {
            PHX_Action(hCamera, PHX_CONFIG_SAVE, PHX_SAVE_ALL, "NIT.pcf");
        }


       // printf("\nFinished looping 409\n");
        c++;
    }
    return 0;
}
int phxconfig(
    etParamValue   eBoardNumber,        /* Board number, i.e. 1, 2, or 0 for next available */
    etParamValue   eChannelNumber,      /* Channel number */
    char* pszConfigFileName,   /* Name of config file */
    tCxpRegisters  sCameraRegs,         /* Camera CXP registers */
    etParamValue   eConfigMode          /* PHX Configuration mode */
)
{
    int            i;

    int c = 0;

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
    printf("initial board number = %u \n", eBoardNumber);
    if (PHX_OK != eStat) goto Error;

    /* Set the channel number */
    eStat = PHX_ParameterSet(hCamera, PHX_CHANNEL_NUMBER, &eChannelNumber);
    printf("initial channel number = %u \n", eChannelNumber);
    if (PHX_OK != eStat) goto Error;

    /* Set the configuration mode */
    eStat = PHX_ParameterSet(hCamera, PHX_CONFIG_MODE, &eConfigMode);
    if (PHX_OK != eStat) goto Error;

    /* Open the board using the above configuration file */
    eStat = PHX_Open(hCamera);
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    printf("initial phxROI_XLENGTH %u \n", dwImageWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_ROI_XLENGTH, &camWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    printf("initial phxROI_XLENGTH %u \n", dwImageWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_ROI_YLENGTH, &camHeight);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_YLENGTH, &dwImageHeight);
    if (PHX_OK != eStat) goto Error;

    eStat = PHX_ParameterSet(hCamera, PHX_CAM_ACTIVE_XLENGTH, &camWidth);
    if (PHX_OK != eStat) goto Error;
    ui32 w;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_ACTIVE_XLENGTH, &w);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_CAM_ACTIVE_YLENGTH, &camHeight);
    if (PHX_OK != eStat) goto Error;
    ui32 h;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_ACTIVE_YLENGTH, &h);
    if (PHX_OK != eStat) goto Error;
    //eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
    //if (PHX_OK != eStat) goto Error;
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_COL, &eCamSrcCol);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    eStat = PHX_ParameterGet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    if (PHX_OK != eStat) goto Error;
    eCamFormat = eCaptureFormat;
    eStat = PHX_ParameterSet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ACQ_TYPE, &eAcqType);
    if (PHX_OK != eStat) goto Error;
//
    PHX_ParameterSet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    eStat = PHX_ParameterGet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    if (PHX_OK != eStat) goto Error;

//
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
    eStat = PHX_ParameterSet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
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
    if (PHX_OK != eStat) goto Error;


    /* Read back the Destination Buffer Width and Height */
    eStat = PHX_ParameterSet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStat) goto Error;

    /* Continue processing data until the user presses a stops the acquisition
     * or a FIFO overflow is detected
     */

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
            printf("\nFailed to StreamRead last acquired buffer\n");

        /* Process the newly acquired buffer,
         * which in this simple example is a call to display the data.
         * For our display function we use the pvContext member variable to
         * pass a display buffer handle.
         * Alternatively the actual video data can be accessed at stBuffer.pvAddress
         *
         */
        hBuffHandle = (tPHX)stBuffer.pvContext;

        /* This copies/converts data from the direct capture buffer to the indirect display buffer */
        eStat = PIL_Convert(hBuffHandle, hDisplayBuffer);
        if (PHX_OK != eStat)
            printf("\nFailed to convert buffers\n");

#if defined _PHX_DISPLAY
        if (PHX_OK == eStat)
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

    printf("\n");

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
    if (hDisplayBuffer) PDL_BufferDestroy(&hDisplayBuffer);

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
    ui32           dwBufferWidth=2*1280;
    ui32 dwBufferStride = 1*dwBufferWidth;
    ui32           imperbuf = 1;
    ui32           dwBufferHeight=imperbuf*1020;
    ui32           dwBufferCount = 1;
    tFlag          fCameraIsCxp = FALSE;
    tFlag          fIsCxpCameraDiscovered = FALSE;
    int            i;

    int c = 0;

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
    printf("initial board number = %u \n", eBoardNumber);
    if (PHX_OK != eStat) goto Error;

    /* Set the channel number */
    eStat = PHX_ParameterSet(hCamera, PHX_CHANNEL_NUMBER, &eChannelNumber);
    printf("initial channel number = %u \n", eChannelNumber);
    if (PHX_OK != eStat) goto Error;

    /* Set the configuration mode */
    eStat = PHX_ParameterSet(hCamera, PHX_CONFIG_MODE, &eConfigMode);
    if (PHX_OK != eStat) goto Error;

    /* Open the board using the above configuration file */
    eStat = PHX_Open(hCamera);
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    printf("initial phxROI_XLENGTH %u \n", dwImageWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_ROI_XLENGTH, &camWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_XLENGTH, &dwImageWidth);
    printf("initial phxROI_XLENGTH %u \n", dwImageWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_ROI_YLENGTH, &camHeight);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ROI_YLENGTH, &dwImageHeight);
    if (PHX_OK != eStat) goto Error;

    eStat = PHX_ParameterSet(hCamera, PHX_CAM_ACTIVE_XLENGTH, &camWidth);
    if (PHX_OK != eStat) goto Error;
    ui32 w;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_ACTIVE_XLENGTH, &w);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_CAM_ACTIVE_YLENGTH, &camHeight);
    if (PHX_OK != eStat) goto Error;
    ui32 h;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_ACTIVE_YLENGTH, &h);
    if (PHX_OK != eStat) goto Error;
    //eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
    //if (PHX_OK != eStat) goto Error;
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_SRC_COL, &eCamSrcCol);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    eStat = PHX_ParameterGet(hCamera, PHX_BUS_FORMAT, &eCaptureFormat);
    if (PHX_OK != eStat) goto Error;
    eCamFormat = eCaptureFormat;
    eStat = PHX_ParameterSet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_CAM_FORMAT, &eCamFormat);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_ACQ_TYPE, &eAcqType);
    if (PHX_OK != eStat) goto Error;
//
    PHX_ParameterSet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    eStat = PHX_ParameterGet(hCamera, PHX_ACQ_NUM_BUFFERS, &dwBufferCount);
    if (PHX_OK != eStat) goto Error;

//
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
    eStat = PHX_ParameterSet(hCamera, PHX_CAM_SRC_DEPTH, &dwCamSrcDepth);
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
    if (PHX_OK != eStat) goto Error;


    /* Read back the Destination Buffer Width and Height */
    eStat = PHX_ParameterSet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_BUF_DST_XLENGTH, &dwBufferWidth);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterSet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStat) goto Error;
    eStat = PHX_ParameterGet(hCamera, PHX_BUF_DST_YLENGTH, &dwBufferHeight);
    if (PHX_OK != eStat) goto Error;

    /* Malloc space to hold the array of image pointers */
    psImageBuffers = (stImageBuff*)malloc((dwBufferCount+1) * sizeof(stImageBuff));
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
        printf("adding buffer %i", i);
        /* We create a capture buffer for our double buffering */
        eStat = PBL_BufferCreate(&phCaptureBuffers[i], (etBufferMode)PDL_BUFF_SYSTEM_MEM_DIRECT,
            0, hCamera, PHX_ErrHandlerDefault);
        if (PHX_OK != eStat) goto Error;
        eStat = PHX_ParameterSet(hCamera, PHX_ACQ_IMAGES_PER_BUFFER, &imperbuf);
        printf("images per buffer = %u \n", imperbuf);
        eStat = PHX_ParameterSet(hCamera, PHX_ACQ_IMAGES_PER_BUFFER, &imperbuf);
        printf("images per buffer = %u \n", imperbuf);
        /* Initialise our capture buffer */
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_WIDTH, &dwImageWidth);
        if (PHX_OK != eStat) goto Error;
        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_HEIGHT, &dwBufferHeight);  /* Use dwBufferHeight to take PHX_ACQ_IMAGES_PER_BUFFER count into account */
        if (PHX_OK != eStat) goto Error;

        eStat = PBL_BufferParameterSet(phCaptureBuffers[i], PBL_BUFF_STRIDE, &dwBufferStride);
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
    eStat = PDL_DisplayCreate(&hDisplay, NULL, hCamera, PhxCommonDisplayErrorHandler);
    if (PHX_OK != eStat) goto Error;

    /* We create a display buffer (indirect) */
    eStat = PDL_BufferCreate(&hDisplayBuffer, hDisplay, (etBufferMode)PDL_BUFF_SYSTEM_MEM_INDIRECT);
    if (PHX_OK != eStat) goto Error;

    /* Initialise the display, this associates the display buffer with the display */
    eStat = PDL_DisplayInit(hDisplay);
    if (PHX_OK != eStat) goto Error;

#endif

    /* Enable FIFO Overflow events */
    eParamValue = PHX_INTRPT_FIFO_OVERFLOW;
    eStat = PHX_ParameterSet(hCamera, PHX_INTRPT_SET, &eParamValue);
    if (PHX_OK != eStat) goto Error;

    /* Setup our own event context */
    eStat = PHX_ParameterSet(hCamera, PHX_EVENT_CONTEXT, &sPhxLive);
    if (PHX_OK != eStat) goto Error;


    /* Now start our capture, using the callback method */
    eStat = PHX_StreamRead(hCamera, PHX_START, phxlive_callback);
    if (PHX_OK != eStat) goto Error;


    /* Allocate global buffer */
    globalBuffer = (uint16_t*)malloc(globalBufferWidth * globalBufferHeight * sizeof(uint16_t));
    if (globalBuffer == NULL) {
        printf("Failed to allocate global buffer\n");
        goto Error;
    }
    /* Continue processing data until the user presses a stops the acquisition
     * or a FIFO overflow is detected
     */

    /*save pcf to file*/
    PHX_Action(hCamera, PHX_CONFIG_SAVE, PHX_SAVE_ALL, "Sens128016bit.pcf");

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

        /* This copies/converts data from the direct capture buffer to the indirect display buffer */
        eStat = PIL_Convert(hBuffHandle, hDisplayBuffer);
        if (PHX_OK != eStat)
            printf("\nFailed to convert buffers\n");

#if defined _PHX_DISPLAY
        if (PHX_OK == eStat)
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

    printf("\n");

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
    if (hDisplayBuffer) PDL_BufferDestroy(&hDisplayBuffer);

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

__declspec(dllexport) void stop_looping() {
    stop_loop = true;
}

__declspec(dllexport) void access_buffer() {
    read_buffer = true;
}

__declspec(dllexport) void write_config() {
    save_config = true;
}

/* VxWorks requires a unique function name for each application */
#if !defined _PHX_VXWORKS

__declspec(dllexport) tPhxCmd InitPhxCmd()
{
    tCxpRegisters  sCameraRegs;
    tPhxCmd        sPhxCmd;
    int            nStatus = -1;

    PhxCommonSkipParseCmd(&sPhxCmd);

    return sPhxCmd;

}

__declspec(dllexport) tCxpRegisters InitCameraRegs()
{
    tCxpRegisters  sCameraRegs;
    tPhxCmd        sPhxCmd;
    int            nStatus = -1;

    PhxCommonSkipParseCmd(&sPhxCmd);

    return sCameraRegs;

}

__declspec(dllexport) int InitMemory(tPhxCmd sPhxCmd, tCxpRegisters sCameraRegs) {
    memset(&sCameraRegs, 0, sizeof(sCameraRegs));
    if (NULL != sPhxCmd.pszConfigFileName) {
        if (PHX_OK != PhxCommonParseCxpRegs(sPhxCmd.pszConfigFileName, &sCameraRegs)) {
            return -1;
        }
    }
}


__declspec(dllexport) void InitPhxCommonKbInit()
{
    PhxCommonKbInit();
}

__declspec(dllexport) void InitPhxCommonKbClose()
{
    PhxCommonKbClose();
}

__declspec(dllexport) int Initphxlive(tPhxCmd sPhxCmd, tCxpRegisters sCameraRegs)
{
    phxlive(sPhxCmd.eBoardNumber, sPhxCmd.eChannelNumber, sPhxCmd.pszConfigFileName, sCameraRegs, sPhxCmd.eConfigMode);
    return 0;
}

//__declspec(dllexport) stImageBuff get_display_buffer()
//{
//    return expDisplayBuff;
//}



int main()
{
    tCxpRegisters  sCameraRegs;
    tPhxCmd        sPhxCmd;
    int            nStatus = -1;

    sPhxCmd = InitPhxCmd();
    sCameraRegs = InitCameraRegs();
    nStatus = InitMemory(sPhxCmd, sCameraRegs);

    InitPhxCommonKbInit();
    Initphxlive(sPhxCmd, sCameraRegs);
    InitPhxCommonKbClose();
    return nStatus;
}
#endif
