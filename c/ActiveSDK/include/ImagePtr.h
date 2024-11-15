#pragma once

#include "Platform.h"
#include "IImage.h"

namespace ActiveSilicon {
   namespace ActiveApi {

      class CImagePtrImpl;

      /*!
      * \class  CImagePtr
      * \brief Smart pointer holding a reference to an image buffer.
      * 
      * Controls the reuse and lifespan of the referenced image buffer.
      * When all smart pointers referencing a buffer go out of scope, the image buffer is reused 
      * or destroyed.
      *
      * Note that the images must be returned to the acquisition engine for the acquisition to continue.
      *
      * \threading Instances of CImagePtr referencing the same image can be used from any thread context.
      */
      class ACTIVEDEVICEFACTORY_DECL_CLASS_EXPORT CImagePtr
      {
      public:
         /*!
         * \brief Constructs a smart pointer that does not reference any image buffer.
         */
         CImagePtr();
         CImagePtr(const CImagePtr &rhs);
         ~CImagePtr();

         CImagePtr& operator=(const CImagePtr &rhs);
         IImage* operator->() const;
         IImage *get() const;
         operator IImage&() const;

         /*!
         * \brief Return true if an image buffer is referenced
         */
         bool IsValid() const;
         operator bool() const;

         /*!
         * \brief Return true if this is the last reference to the image buffer.
         */
         bool IsUnique() const;

         /*!
         * \brief Release the reference to the iamge buffer.
         */
         void Release();

         // Internal use only
         CImagePtr& operator=(const CImagePtrImpl &rhs);
         CImagePtr(const CImagePtrImpl &rhs);

      protected:
         CImagePtrImpl *m_d;
      };

   }
}