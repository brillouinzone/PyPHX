#include "PHXGrabberInterfaceWrapper.h"
#include "PHXGrabberInterface.h"

// Create and destroy the PHXGrabberInterface object
PHXGrabberHandle create_phxgrabber() {
    return new PHXGrabberInterface();
}

void destroy_phxgrabber(PHXGrabberHandle handle) {
    delete static_cast<PHXGrabberInterface*>(handle);
}

// Open and close methods
void phxgrabber_open(PHXGrabberHandle handle, const char* cameraConfigFile) {
    static_cast<PHXGrabberInterface*>(handle)->open(std::string(cameraConfigFile));
}

void phxgrabber_close(PHXGrabberHandle handle) {
    static_cast<PHXGrabberInterface*>(handle)->close();
}

// Status and error methods
int phxgrabber_is_opened(PHXGrabberHandle handle) {
    return static_cast<PHXGrabberInterface*>(handle)->isOpened() ? 1 : 0;
}

const char* phxgrabber_last_error(PHXGrabberHandle handle) {
    return static_cast<PHXGrabberInterface*>(handle)->lastError().c_str();
}

// Event counter usage method
void phxgrabber_set_event_counter_usage(PHXGrabberHandle handle, int useEventCounter) {
    static_cast<PHXGrabberInterface*>(handle)->setEventCounterUsage(useEventCounter != 0);
}