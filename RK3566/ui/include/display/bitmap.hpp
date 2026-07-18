#include <cstdint>

class Bitmap
{
public:
   uint16_t width;
   uint16_t height;

   /**
    * (width + 7) / 8
    */
   uint16_t stride;
   const uint8_t *data;
};