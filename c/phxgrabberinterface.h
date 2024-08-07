#ifndef PHXGRABBERINTERFACE_H
#define PHXGRABBERINTERFACE_H

#ifdef _WIN32
#ifdef BUILDING_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif

// Defining the OS (must be declared somewhere else)
#define _PHX_WIN32


#include <string>
#include "phx_os.h"
#include "common/common.h"
#include <atomic>


// Define an application specific structure to hold user information
typedef struct {
    volatile ui32 nBufferReadyCount;
    volatile tFlag fFifoOverFlow;
} EventContext;

class DLL_EXPORT PHXGrabberInterface {
public:
    PHXGrabberInterface();
    virtual ~PHXGrabberInterface();

    void open(const std::string& cameraConfigFile);
    void close();

    bool isOpened() const;
    std::string lastError() const;

    void setEventCounterUsage(bool useEventCounter);

    void setStopLoop(volatile bool* stop);


private:
    int nStatus;
    std::string _cameraConfigFile;
    std::string _lastError = "no error \n";
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

    // Method to call live function
    int live(char* file);
};

#endif // PHXGRABBERINTERFACE_H

