#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstddef>

static constexpr uint16_t DEFAULT_DISPLAY_WIDTH = 960;
static constexpr uint16_t DEFAULT_DISPLAY_HEIGHT = 552;

enum class PixelColor : uint8_t
{
   White = 0,
   Black = 1
};

/**
 * Stores a 1-bit frame buffer.
 * Default frame size is 960x552, making the frame buffer
 * 66.24 KB.
 */
class Framebuffer
{
public:
   /**
    * Constructs a framebuffer with a custom width and height. Not recommended for UI testing.
    */
   explicit Framebuffer(uint16_t width, uint16_t height)
       : i_width(width),
         i_height(height),
         updated(false)
   {
      if ((static_cast<size_t>(width) * height) % 8 != 0)
         throw std::invalid_argument("Framebuffer size must be byte aligned");

      i_data.resize(size());
   }

   /**
    * Constructs a framebuffer with the default height and width.
    */
   Framebuffer() : Framebuffer(DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_HEIGHT) {}

   /**
    * Returns the width of the display in pixels.
    */
   [[nodiscard]] uint16_t width() const
   {
      return i_width;
   };

   /**
    * Returns the height of the display in pixels.
    */
   [[nodiscard]] uint16_t height() const
   {
      return i_height;
   };

   /**
    * Returns total size of frame buffer in bytes.
    */
   [[nodiscard]] size_t size() const
   {
      return (static_cast<size_t>(i_width) * i_height) / 8;
   }

   /**
    * Returns the pointer to the top of the framebuffer (first pixel)
    */
   uint8_t *data();

   /**
    * Returns the pointer to the top of the framebuffer (first pixel)
    */
   const uint8_t *data() const;

   bool hasUpdatedSinceLastDraw()
   {
      return updated;
   }

   void markUpdated()
   {
      updated = true;
   }

   /**
    * Sets a pixel without checking bounds. Fast but unsafe
    */
   void setPixelUnchecked(uint16_t x,
                          uint16_t y,
                          PixelColor color);

   /**
    * Updates the specific pixel to the specified value
    */
   void setPixel(uint16_t x, uint16_t y, PixelColor value);

   /**
    * Returns the value of the specified pixel
    */
   PixelColor getPixel(uint16_t x, uint16_t y) const;

   /**
    * Resets the frame buffer to have each pixel set to the specified value (default white).
    */
   void clear(PixelColor value = PixelColor::White);

private:
   uint16_t i_width;
   uint16_t i_height;

   bool updated;

   std::vector<uint8_t> i_data;
};