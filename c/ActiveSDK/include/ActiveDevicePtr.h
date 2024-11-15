#pragma once

#include "Platform.h"
#include <memory>

namespace ActiveSilicon {
   namespace ActiveApi {

      struct IActiveDevice;

      /*!
      * \class  CActiveDevicePtr
      * \brief Smart pointer holding a reference to a device.
      */
      class ACTIVEDEVICEFACTORY_DECL_CLASS_EXPORT CActiveDevicePtr
      {
      public:
         CActiveDevicePtr();
         CActiveDevicePtr(IActiveDevice *p);

         IActiveDevice* operator->() const;

         IActiveDevice* get() const;

         /*!
         * \brief Return true if a device is referenced.
         */
         bool IsValid() const;
         operator bool() const;

         /*!
         * \brief Return true if this is the last reference to the device.
         */
         bool IsUnique() const;

         /*!
         * \brief Release the reference to the device.
         */
         void Release();

      private:
         std::shared_ptr<IActiveDevice> m_p;
      };

   }
}