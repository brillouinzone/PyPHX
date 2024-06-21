#ifndef PHXGRABBERINTERFACE_H
#define PHXGRABBERINTERFACE_H

// Defining the OS (must be declared somewhere else)
#define _PHX_WIN32

#include <string>
#include "phx_os.h"
#include "common/common.h"


// Define an application specific structure to hold user information
typedef struct {
    volatile ui32 nBufferReadyCount;
    volatile tFlag fFifoOverFlow;
} EventContext;

class PHXGrabberInterface {
public:
    PHXGrabberInterface();
    virtual ~PHXGrabberInterface();

    void open(const std::string& cameraConfigFile);
    void close();

    bool isOpened() const;
    std::string lastError() const;

    void setEventCounterUsage(bool useEventCounter);

private:
    int nStatus;
    std::string _cameraConfigFile;
    std::string _lastError;
    bool _isOpened;
    bool _useEventCounter;
    etParamValue eBoardNumber = PHX_BOARD_NUMBER_1;
    etParamValue eChannelNumber = PHX_CHANNEL_NUMBER_1;
    etParamValue eConfigMode;

    std::string cameraConfigFile;
    tPhxCmd phxGrabberInfo;
    tHandle hCamera;
    tPHX hDisplay;
    tPHX hBuffer1;
    tPHX hBuffer2;
    EventContext eventContext; // User defined Event Context
    tPhxLive       sPhxLive; 
    tPHX           hDisplayBuffer = 0;           /* Display buffer handle */
    tPHX           hBuffHandle;

    bool configureGrabber();
    bool configureGrabberLegacy();
    bool livePicture();
    void releaseGrabber();
    void setState(bool isOpened);
    void messageOutput(const std::string& message);
    void updateEventCounter(int count);
};

#endif // PHXGRABBERINTERFACE_H

