#include "bitmap.hpp"

#include <cstddef>

namespace
{
    bool isInBounds(int32_t x, int32_t y, uint16_t width, uint16_t height)
    {
        return x >= 0 && y >= 0 &&
               x < static_cast<int32_t>(width) &&
               y < static_cast<int32_t>(height);
    }
}

bool Bitmap::setPixel(int32_t x, int32_t y, bool value)
{
    if (data.empty() || !isInBounds(x, y, width, height))
        return false;

    const uint32_t byteIndex =
        static_cast<uint32_t>(y) * stride +
        static_cast<uint32_t>(x / 8);

    const uint8_t bitMask = static_cast<uint8_t>(0x80u >> (x % 8));

    if (value)
        data[byteIndex] |= bitMask;
    else
        data[byteIndex] &= static_cast<uint8_t>(~bitMask);

    return true;
}

bool Bitmap::getPixel(int32_t x, int32_t y) const
{
    if (data.empty() || !isInBounds(x, y, width, height))
        return false;

    const uint32_t byteIndex =
        static_cast<uint32_t>(y) * stride +
        static_cast<uint32_t>(x / 8);

    const uint8_t bitMask = static_cast<uint8_t>(0x80u >> (x % 8));

    return (data[byteIndex] & bitMask) != 0;
}

void Bitmap::clear(bool value)
{
    if (data.empty())
        return;

    for (uint32_t i = 0; i < static_cast<uint32_t>(height) * stride; ++i)
    {
        data[i] = value ? 0xFFu : 0x00u;
    }
}

void Bitmap::drawBitmap(int32_t x, int32_t y, const Bitmap &bitmap, bool value)
{
    if (data.empty() || bitmap.getData() == nullptr)
        return;

    for (int32_t row = 0; row < static_cast<int32_t>(bitmap.getHeight()); ++row)
    {
        for (int32_t col = 0; col < static_cast<int32_t>(bitmap.getWidth()); ++col)
        {
            if (bitmap.getPixel(col, row))
                setPixel(x + col, y + row, value);
        }
    }
}
