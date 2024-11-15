#pragma once

#include "Platform.h"
#include "ActiveDeviceFactoryTypes.h"

struct DeviceLocation;

namespace ActiveSilicon {
   namespace ActiveApi {

      /*!
      * \brief Holds information about an enumerated device.
      * The device enumeration process triggered with 
      * IActiveDeviceFactory::RefreshKnownDeviceList generates a list of 
      * CActiveDeviceInfo objects. A CActiveDeviceInfo object stores information
      * about a device that can be accessed without opening the device.
      */
      class ACTIVEDEVICEFACTORY_DECL_CLASS_EXPORT CActiveDeviceInfo
      {
      public:
         CActiveDeviceInfo();
         CActiveDeviceInfo(const CActiveDeviceInfo &);
         virtual ~CActiveDeviceInfo();

         /*!
         * \brief Return a unique ID of the module.
         */
         const wchar_t *GetId(ActiveModuleType module) const;

         /*!
         * \brief Return the user-friendly name of the module.
         */
         const wchar_t *GetName(ActiveModuleType module) const;

         bool operator ==(const CActiveDeviceInfo& rhs) const;
         bool operator != (const CActiveDeviceInfo& rhs) const;

         // Internal use only
         CActiveDeviceInfo(const DeviceLocation&);

      private:
         struct CImpl;
         CImpl *m_d;
      };
   }
}