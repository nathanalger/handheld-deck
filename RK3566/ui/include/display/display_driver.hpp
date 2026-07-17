#pragma once

#include "framebuffer.hpp"

class DisplayDriver
{
public:
   virtual ~DisplayDriver() = default;

   virtual bool initialize() = 0;

   virtual void present(const Framebuffer &framebuffer) = 0;

   virtual bool processEvents()
   {
      return true;
   }
};