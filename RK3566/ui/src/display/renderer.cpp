#include "renderer.hpp"
#include "fonts.hpp"

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

void Renderer::drawRect(int32_t x,
                        int32_t y,
                        uint32_t width,
                        uint32_t height,
                        PixelColor color)
{
   const int32_t framebufferWidth = static_cast<int32_t>(i_framebuffer.width());
   const int32_t framebufferHeight = static_cast<int32_t>(i_framebuffer.height());

   const int32_t rectRight = x + static_cast<int32_t>(width);
   const int32_t rectBottom = y + static_cast<int32_t>(height);

   const int32_t clipLeft = std::max<int32_t>(0, x);
   const int32_t clipTop = std::max<int32_t>(0, y);
   const int32_t clipRight = std::min<int32_t>(framebufferWidth, rectRight);
   const int32_t clipBottom = std::min<int32_t>(framebufferHeight, rectBottom);

   if (clipRight <= clipLeft || clipBottom <= clipTop)
      return;

   for (int32_t row = clipTop; row < clipBottom; row++)
   {
      for (int32_t col = clipLeft; col < clipRight; col++)
      {
         drawPixel(col, row, color);
      }
   }
}

void Renderer::drawBitmap(int32_t x,
                          int32_t y,
                          const Bitmap &bitmap,
                          PixelColor primaryColor)
{
   if (bitmap.getData() == nullptr)
      return;

   const int32_t framebufferWidth = static_cast<int32_t>(i_framebuffer.width());
   const int32_t framebufferHeight = static_cast<int32_t>(i_framebuffer.height());
   const int32_t bitmapWidth = static_cast<int32_t>(bitmap.getWidth());
   const int32_t bitmapHeight = static_cast<int32_t>(bitmap.getHeight());

   const int32_t startCol = std::max<int32_t>(0, -x);
   const int32_t endCol = std::min<int32_t>(bitmapWidth,
                                            std::max<int32_t>(0, framebufferWidth - x));
   const int32_t startRow = std::max<int32_t>(0, -y);
   const int32_t endRow = std::min<int32_t>(bitmapHeight,
                                            std::max<int32_t>(0, framebufferHeight - y));

   if (startCol >= endCol || startRow >= endRow)
      return;

   for (int32_t row = startRow; row < endRow; row++)
   {
      for (int32_t col = startCol; col < endCol; col++)
      {
         const uint32_t byteIndex =
             static_cast<uint32_t>(row) * bitmap.getStride() +
             static_cast<uint32_t>(col / 8);

         const uint8_t bitMask =
             static_cast<uint8_t>(0x80u >> (col % 8));

         if (bitmap.getData()[byteIndex] & bitMask)
         {
            drawPixel(
                x + static_cast<int32_t>(col),
                y + static_cast<int32_t>(row),
                primaryColor);
         }
      }
   }
}

void Renderer::drawText(int32_t x,
                        int32_t y,
                        const std::string &text,
                        const Font &font,
                        PixelColor color)
{
   int32_t cursorX = x;
   int32_t cursorY = y;

   for (char ch : text)
   {
      if (ch == '\n')
      {
         cursorX = x;
         cursorY += static_cast<int32_t>(font.glyphHeight()) + 1;
         continue;
      }

      if (ch == ' ')
      {
         cursorX += font.glyphAdvance(ch);
         continue;
      }

      Bitmap glyph = font.glyphBitmap(ch);
      drawBitmap(cursorX, cursorY, glyph, color);
      cursorX += font.glyphAdvance(ch);
   }
}