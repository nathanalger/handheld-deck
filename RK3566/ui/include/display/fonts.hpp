#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "bitmap.hpp"

class Font;

class Glyph
{
public:
    Glyph() = default;
    Glyph(uint16_t width, uint16_t height, const std::vector<uint8_t> &data,
          uint16_t advance = 0);

    [[nodiscard]] Bitmap toBitmap(uint16_t scale = 1) const;
    [[nodiscard]] uint16_t width() const { return i_width; }
    [[nodiscard]] uint16_t height() const { return i_height; }
    [[nodiscard]] uint16_t advance() const { return i_advance; }

private:
    uint16_t i_width = 0;
    uint16_t i_height = 0;
    std::vector<uint8_t> i_data;
    uint16_t i_advance = 0;
};

class Font
{
public:
    explicit Font(uint16_t size = 1);
    virtual ~Font() = default;

    void setSize(uint16_t size);
    [[nodiscard]] uint16_t size() const { return i_size; }

    void addGlyph(char ch, const Glyph &glyph);
    [[nodiscard]] bool hasGlyph(char ch) const;
    [[nodiscard]] const Glyph *glyph(char ch) const;

    [[nodiscard]] virtual Bitmap glyphBitmap(char c) const;
    [[nodiscard]] virtual int glyphAdvance(char c) const;
    [[nodiscard]] virtual uint16_t glyphWidth() const;
    [[nodiscard]] virtual uint16_t glyphHeight() const;

protected:
    std::unordered_map<char, Glyph> i_glyphs;

private:
    uint16_t i_size = 1;
};

class BitmapFont : public Font
{
public:
    explicit BitmapFont(uint16_t size = 1);
};