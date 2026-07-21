#include "fonts.hpp"

#include <array>
#include <cctype>
#include <string_view>

namespace
{
    std::array<uint8_t, 7> buildGlyph(const std::array<std::string_view, 7> &rows)
    {
        std::array<uint8_t, 7> data{};

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
}

Bitmap Font::glyphBitmap(char c) const
{
    const unsigned char ch = static_cast<unsigned char>(std::toupper(c));

    switch (ch)
    {
    case 'A':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "10001", "11111", "10001", "10001", "10001"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'B':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11110", "10001", "10001", "11110", "10001", "10001", "11110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'C':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "10000", "10000", "10000", "10001", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'D':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11110", "10001", "10001", "10001", "10001", "10001", "11110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'E':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11111", "10000", "10000", "11110", "10000", "10000", "11111"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'F':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11111", "10000", "10000", "11110", "10000", "10000", "10000"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'H':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "10001", "10001", "10001", "11111", "10001", "10001", "10001"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'I':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11111", "00100", "00100", "00100", "00100", "00100", "11111"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'L':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "10000", "10000", "10000", "10000", "10000", "10000", "11111"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'O':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "10001", "10001", "10001", "10001", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'P':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11110", "10001", "10001", "11110", "10000", "10000", "10000"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'R':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11110", "10001", "10001", "11110", "10100", "10010", "10001"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'S':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01111", "10000", "10000", "01110", "00001", "00001", "11110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'T':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11111", "00100", "00100", "00100", "00100", "00100", "00100"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'U':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "10001", "10001", "10001", "10001", "10001", "10001", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case 'Y':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "10001", "10001", "01010", "00100", "00100", "00100", "00100"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '0':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "10011", "10101", "11001", "10001", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '1':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00100", "01100", "00100", "00100", "00100", "00100", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '2':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "00001", "00010", "00100", "01000", "11111"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '3':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11110", "00001", "00001", "01110", "00001", "00001", "11110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '4':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00010", "00110", "01010", "10010", "11111", "00010", "00010"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '5':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11111", "10000", "10000", "11110", "00001", "00001", "11110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '6':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10000", "10000", "11110", "10001", "10001", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '7':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "11111", "00001", "00010", "00100", "01000", "01000", "01000"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '8':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "10001", "01110", "10001", "10001", "01110"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '9':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "10001", "01111", "00001", "00010", "01100"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '.':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00000", "00000", "00000", "00000", "00000", "00100", "00100"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '-':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00000", "00000", "00000", "11111", "00000", "00000", "00000"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '+':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00000", "00100", "00100", "11111", "00100", "00100", "00000"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '!':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00100", "00100", "00100", "00100", "00100", "00000", "00100"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    case '?':
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "01110", "10001", "00001", "00110", "00000", "00000", "00100"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }

    default:
    {
        static const auto data = buildGlyph(std::array<std::string_view, 7>{
            "00000", "00000", "00000", "00000", "00000", "00000", "00000"});
        return Bitmap(5, 7, const_cast<uint8_t *>(data.data()));
    }
    }
}

int Font::glyphAdvance(char c) const
{
    return (c == ' ' ? 3 : 6);
}