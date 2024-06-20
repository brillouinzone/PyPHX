#ifndef PHXGRABBERINTERFACEWRAPPER_H
#define PHXGRABBERINTERFACEWRAPPER_H



#ifdef _WIN32
#ifdef BUILDING_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef void* PHXGrabberHandle;

	// Create and destroy the PHXGrabberInterface object
	DLL_EXPORT PHXGrabberHandle create_phxgrabber();
	DLL_EXPORT void destroy_phxgrabber(PHXGrabberHandle handle);

	// Open and close methods
	DLL_EXPORT void phxgrabber_open(PHXGrabberHandle handle, const char* cameraConfigFile);
	DLL_EXPORT void phxgrabber_close(PHXGrabberHandle handle);

	// Status and error methods
	DLL_EXPORT int phxgrabber_is_opened(PHXGrabberHandle handle);
	DLL_EXPORT const char* phxgrabber_last_error(PHXGrabberHandle handle);

	// Event counter usage method
	DLL_EXPORT void phxgrabber_set_event_counter_usage(PHXGrabberHandle handle, int useEventCounter);


#ifdef __cplusplus
}
#endif

#endif // PHXGRABBERINTERFACEWRAPPER_H
#pragma once
