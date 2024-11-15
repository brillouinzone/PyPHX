#pragma once

#include <stdint.h>
#include <memory>
#include "ActivePixelFormat.h"

namespace ActiveSilicon {
   namespace ActiveApi {

      /*!
      \class  IImage
      \brief  Provides a generic interface for an image.
      */

      class IImage
      {
      public:
         virtual ~IImage()
         {
         }

         virtual bool IsValid() const = 0;

         virtual uint32_t GetWidth() const = 0;
         virtual uint32_t GetHeight() const = 0;
         virtual ActivePixelFormat GetPixelFormat() const = 0;

         /*!
          * \brief Retrieve the start of the image data in the buffer, taking into account any offset (X and / or Y). The image
          *        data is thereafter 'sequential' in the buffer, with the caveat of the stride (x padding).
          */
         virtual const void *GetData() const = 0;
         virtual void *GetData() = 0;

         /*!
          * \brief Retrieve the offset in bytes between two consecutive lines in the image.
          */
         virtual size_t GetStride() const = 0;

         /*!
         * \brief Return the size of the internal buffer in bytes.
         */
         virtual size_t GetByteCount() const = 0;

         /*!
         * \brief Return the context pointer associated with the image.
         */
         virtual void * GetContext() const = 0;
      };

   }
}