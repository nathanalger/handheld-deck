#include "fonts.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <string_view>
#include <vector>

namespace
{
    std::vector<uint8_t> buildGlyphData(const std::array<std::string_view, 7> &rows)
    {
        std::vector<uint8_t> data(7, 0);

        for (size_t row = 0; row < rows.size(); ++row)
        {
            uint8_t value = 0;

            for (size_t col = 0; col < 5; ++col)
            {
                if (rows[row].size() > col && rows[row][col] == '1')
                    value |= static_cast<uint8_t>(0x80u >> col);
            }

            data[row] = value;
        }

        return data;
    }

    Glyph makeGlyph(const std::array<std::string_view, 7> &rows, uint16_t width, uint16_t advance = 6)
    {
        return Glyph(width, 7, buildGlyphData(rows), advance);
    }
}

Glyph::Glyph(uint16_t width, uint16_t height, const std::vector<uint8_t> &data,
             uint16_t advance)
    : i_width(width), i_height(height), i_data(data), i_advance(advance)
{
}

Bitmap Glyph::toBitmap(uint16_t scale) const
{
    if (i_data.empty())
        return Bitmap();

    const uint16_t outWidth = static_cast<uint16_t>(i_width * scale);
    const uint16_t outHeight = static_cast<uint16_t>(i_height * scale);
    const uint16_t outStride = (outWidth + 7) / 8;
    std::vector<uint8_t> outData(static_cast<size_t>(outHeight) * outStride, 0);

    for (uint16_t row = 0; row < i_height; ++row)
    {
        for (uint16_t col = 0; col < i_width; ++col)
        {
            const bool on = (i_data[row] & static_cast<uint8_t>(0x80u >> col)) != 0;
            if (!on)
                continue;

            for (uint16_t sy = 0; sy < scale; ++sy)
            {
                for (uint16_t sx = 0; sx < scale; ++sx)
                {
                    const uint16_t outX = static_cast<uint16_t>(col * scale + sx);
                    const uint16_t outY = static_cast<uint16_t>(row * scale + sy);
                    const uint32_t byteIndex = static_cast<uint32_t>(outY) * outStride + static_cast<uint32_t>(outX / 8);
                    const uint8_t bitMask = static_cast<uint8_t>(0x80u >> (outX % 8));
                    outData[byteIndex] |= bitMask;
                }
            }
        }
    }

    return Bitmap(outWidth, outHeight, outData);
}

Font::Font(uint16_t size)
    : i_size(size)
{
}

void Font::setSize(uint16_t size)
{
    i_size = std::max<uint16_t>(1, size);
}

void Font::addGlyph(char ch, const Glyph &glyph)
{
    i_glyphs[static_cast<char>(std::toupper(static_cast<unsigned char>(ch)))] = glyph;
}

bool Font::hasGlyph(char ch) const
{
    return i_glyphs.find(static_cast<char>(std::toupper(static_cast<unsigned char>(ch)))) != i_glyphs.end();
}

const Glyph *Font::glyph(char ch) const
{
    const auto it = i_glyphs.find(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
    if (it == i_glyphs.end())
        return nullptr;

    return &it->second;
}

Bitmap Font::glyphBitmap(char c) const
{
    const Glyph *g = glyph(c);
    if (g == nullptr)
        return Bitmap();

    return g->toBitmap(i_size);
}

int Font::glyphAdvance(char c) const
{
    const Glyph *g = glyph(c);
    if (g == nullptr)
        return 0;

    return static_cast<int>(g->advance() * i_size);
}

uint16_t Font::glyphWidth() const
{
    return i_size * 5;
}

uint16_t Font::glyphHeight() const
{
    return i_size * 7;
}

BitmapFont::BitmapFont(uint16_t size)
    : Font(size)
{
    addGlyph('A', makeGlyph({"01110", "10001", "10001", "11111", "10001", "10001", "10001"}, 5));
    addGlyph('B', makeGlyph({"11110", "10001", "10001", "11110", "10001", "10001", "11110"}, 5));
    addGlyph('C', makeGlyph({"01110", "10001", "10000", "10000", "10000", "10001", "01110"}, 5));
    addGlyph('D', makeGlyph({"11110", "10001", "10001", "10001", "10001", "10001", "11110"}, 5));
    addGlyph('E', makeGlyph({"11111", "10000", "10000", "11110", "10000", "10000", "11111"}, 5));
    addGlyph('F', makeGlyph({"11111", "10000", "10000", "11110", "10000", "10000", "10000"}, 5));
    addGlyph('H', makeGlyph({"10001", "10001", "10001", "11111", "10001", "10001", "10001"}, 5));
    addGlyph('I', makeGlyph({"11111", "00100", "00100", "00100", "00100", "00100", "11111"}, 5));
    addGlyph('L', makeGlyph({"10000", "10000", "10000", "10000", "10000", "10000", "11111"}, 5));
    addGlyph('O', makeGlyph({"01110", "10001", "10001", "10001", "10001", "10001", "01110"}, 5));
    addGlyph('P', makeGlyph({"11110", "10001", "10001", "11110", "10000", "10000", "10000"}, 5));
    addGlyph('R', makeGlyph({"11110", "10001", "10001", "11110", "10100", "10010", "10001"}, 5));
    addGlyph('S', makeGlyph({"01111", "10000", "10000", "01110", "00001", "00001", "11110"}, 5));
    addGlyph('T', makeGlyph({"11111", "00100", "00100", "00100", "00100", "00100", "00100"}, 5));
    addGlyph('U', makeGlyph({"10001", "10001", "10001", "10001", "10001", "10001", "01110"}, 5));
    addGlyph('Y', makeGlyph({"10001", "10001", "01010", "00100", "00100", "00100", "00100"}, 5));
    addGlyph('0', makeGlyph({"01110", "10001", "10011", "10101", "11001", "10001", "01110"}, 5));
    addGlyph('1', makeGlyph({"00100", "01100", "00100", "00100", "00100", "00100", "01110"}, 5));
    addGlyph('2', makeGlyph({"01110", "10001", "00001", "00010", "00100", "01000", "11111"}, 5));
    addGlyph('3', makeGlyph({"11110", "00001", "00001", "01110", "00001", "00001", "11110"}, 5));
    addGlyph('4', makeGlyph({"00010", "00110", "01010", "10010", "11111", "00010", "00010"}, 5));
    addGlyph('5', makeGlyph({"11111", "10000", "10000", "11110", "00001", "00001", "11110"}, 5));
    addGlyph('6', makeGlyph({"01110", "10000", "10000", "11110", "10001", "10001", "01110"}, 5));
    addGlyph('7', makeGlyph({"11111", "00001", "00010", "00100", "01000", "01000", "01000"}, 5));
    addGlyph('8', makeGlyph({"01110", "10001", "10001", "01110", "10001", "10001", "01110"}, 5));
    addGlyph('9', makeGlyph({"01110", "10001", "10001", "01111", "00001", "00010", "01100"}, 5));
    addGlyph('.', makeGlyph({"00000", "00000", "00000", "00000", "00000", "00100", "00100"}, 3, 3));
    addGlyph('-', makeGlyph({"00000", "00000", "00000", "11111", "00000", "00000", "00000"}, 5));
    addGlyph('+', makeGlyph({"00000", "00100", "00100", "11111", "00100", "00100", "00000"}, 5));
    addGlyph('!', makeGlyph({"00100", "00100", "00100", "00100", "00100", "00000", "00100"}, 3));
    addGlyph('?', makeGlyph({"01110", "10001", "00001", "00110", "00000", "00000", "00100"}, 5));
    addGlyph(' ', makeGlyph({"00000", "00000", "00000", "00000", "00000", "00000", "00000"}, 3));
}
