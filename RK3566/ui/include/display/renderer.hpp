#pragma once
#include "framebuffer.hpp"

class Renderer
{
public:
   Renderer(Framebuffer &framebuffer);

   /**
    * Clears the display
    */
   void clear();

   /**
    * Draws a pixel at the specified location on the display
    */
   void drawPixel(int32_t x, int32_t y, PixelColor color);

   /**
    * Draws a rectangle on the display
    */
   void drawRect(int32_t x,
                 int32_t y,
                 uint32_t width,
                 uint32_t height,
                 PixelColor color);

   Framebuffer &framebuffer()
   {
      return i_framebuffer;
   }

   const Framebuffer &framebuffer() const
   {
      return i_framebuffer;
   }

private:
   Framebuffer &i_framebuffer;
};