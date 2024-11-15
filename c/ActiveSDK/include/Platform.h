#pragma once

#if defined ActiveDeviceFactory_EXPORTS
#define ACTIVEDEVICEFACTORY_DECL_EXPORT       __declspec(dllexport) __cdecl
#define ACTIVEDEVICEFACTORY_DECL_CLASS_EXPORT __declspec(dllexport)
#else
#define ACTIVEDEVICEFACTORY_DECL_EXPORT       __declspec(dllimport) __cdecl
#define ACTIVEDEVICEFACTORY_DECL_CLASS_EXPORT __declspec(dllimport)
#endif //ActiveDeviceFactory_EXPORTS
