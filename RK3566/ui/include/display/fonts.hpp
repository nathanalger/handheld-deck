#pragma once

#include <cstdint>
#include "bitmap.hpp"

class Font
{
public:
    Font() = default;

    [[nodiscard]] Bitmap glyphBitmap(char c) const;
    [[nodiscard]] int glyphAdvance(char c) const;
    [[nodiscard]] uint16_t glyphWidth() const { return 5; }
    [[nodiscard]] uint16_t glyphHeight() const { return 7; }
};