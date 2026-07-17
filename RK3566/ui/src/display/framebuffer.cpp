#include "framebuffer.hpp"

uint8_t *Framebuffer::data()
{
   return i_data.data();
}

const uint8_t *Framebuffer::data() const
{
   return i_data.data();
}

void Framebuffer::setPixel(uint16_t x, uint16_t y, PixelColor value)
{
   if (x >= i_width || y >= i_height)
      return;

   size_t pixel = static_cast<size_t>(y) * i_width + x;

   size_t byteIndex = pixel / 8;
   uint8_t bitIndex = 7 - (pixel % 8);
   uint8_t mask = static_cast<uint8_t>(1 << bitIndex);

   if (value == PixelColor::Black)
      i_data[byteIndex] |= mask;
   else
      i_data[byteIndex] &= ~mask;
}

PixelColor Framebuffer::getPixel(uint16_t x, uint16_t y) const
{
   if (x >= i_width || y >= i_height)
      return PixelColor::White;

   size_t pixel = static_cast<size_t>(y) * i_width + x;

   size_t byteIndex = pixel / 8;
   uint8_t bitIndex = 7 - (pixel % 8);
   uint8_t mask = static_cast<uint8_t>(1 << bitIndex);

   return (i_data[byteIndex] & mask) != 0
              ? PixelColor::Black
              : PixelColor::White;
}

void Framebuffer::clear(PixelColor value)
{
   uint8_t fillValue = (value == PixelColor::Black) ? 0xFF : 0x00;

   std::fill(i_data.begin(), i_data.end(), fillValue);
}