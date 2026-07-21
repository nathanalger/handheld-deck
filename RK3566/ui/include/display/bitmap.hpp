#pragma once

#include <cstdint>

class Bitmap
{
public:
   Bitmap() : width(0), height(0), stride(0), data(nullptr) {}

   Bitmap(uint16_t width, uint16_t height, uint8_t *data)
       : width(width),
         height(height),
         stride((width + 7) / 8),
         data(data)
   {
   }

   [[nodiscard]] uint16_t getWidth() const
   {
      return width;
   }

   [[nodiscard]] uint16_t getHeight() const
   {
      return height;
   }

   [[nodiscard]] uint16_t getStride() const
   {
      return stride;
   }

   [[nodiscard]] const uint8_t *getData() const
   {
      return data;
   }

   [[nodiscard]] uint8_t *getDataMutable()
   {
      return data;
   }

   bool setPixel(int32_t x, int32_t y, bool value);
   bool getPixel(int32_t x, int32_t y) const;
   void clear(bool value = false);
   void drawBitmap(int32_t x, int32_t y, const Bitmap &bitmap, bool value = true);

private:
   uint16_t width;
   uint16_t height;
   uint16_t stride;
   uint8_t *data;
};