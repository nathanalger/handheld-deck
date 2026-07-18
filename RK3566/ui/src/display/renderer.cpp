#include "renderer.hpp"
#include <algorithm>

Renderer::Renderer(Framebuffer &framebuffer)
    : i_framebuffer(framebuffer)
{
}

void Renderer::clear()
{
   i_framebuffer.clear();
}

void Renderer::drawPixel(int32_t x,
                         int32_t y,
                         PixelColor color)
{
   if ((uint32_t)x >= i_framebuffer.width() ||
       (uint32_t)y >= i_framebuffer.height())
      return;

   i_framebuffer.setPixelUnchecked(
       static_cast<uint16_t>(x),
       static_cast<uint16_t>(y),
       color);
}

// TODO: Clipping
void Renderer::drawRect(int32_t x,
                        int32_t y,
                        uint32_t width,
                        uint32_t height,
                        PixelColor color)
{
   int32_t maxX = x + static_cast<int32_t>(width);
   int32_t maxY = y + static_cast<int32_t>(height);

   if (x >= static_cast<int32_t>(i_framebuffer.width()) ||
       y >= static_cast<int32_t>(i_framebuffer.height()))
      return;

   if (maxX <= 0 || maxY <= 0)
      return;

   x = std::max(x, 0);
   y = std::max(y, 0);

   maxX = std::min(maxX, static_cast<int32_t>(i_framebuffer.width()));
   maxY = std::min(maxY, static_cast<int32_t>(i_framebuffer.height()));

   for (int32_t row = y; row < maxY; row++)
   {
      for (int32_t col = x; col < maxX; col++)
      {
         drawPixel(col, row, color);
      }
   }
}

// TODO: Clipping
void Renderer::drawBitmap(int32_t x,
                          int32_t y,
                          const Bitmap &bitmap,
                          PixelColor primaryColor)
{
   for (uint16_t row = 0; row < bitmap.height; row++)
   {
      for (uint16_t col = 0; col < bitmap.width; col++)
      {
         uint32_t byteIndex =
             row * bitmap.stride + (col / 8);

         uint8_t bitMask =
             static_cast<uint8_t>(0x80 >> (col % 8));

         if (bitmap.data[byteIndex] & bitMask)
         {
            drawPixel(
                x + static_cast<int32_t>(col),
                y + static_cast<int32_t>(row),
                primaryColor);
         }
      }
   }
}