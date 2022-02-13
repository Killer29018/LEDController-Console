#ifndef COLOUR_PALETTES_HPP
#define COLOUR_PALETTES_HPP

#include "Panels/Logger.hpp"
#include "Colour.hpp"

#include <cassert>

struct ColourPaletteItem
{
    uint8_t targetHue;
    cHSV colour;

    ColourPaletteItem() = default;

    ColourPaletteItem(uint8_t targetHue, uint8_t hue, uint8_t sat, uint8_t val)
        : targetHue(targetHue), colour(hue, sat, val) {}

    ColourPaletteItem(uint8_t targetHue, cHSV colour)
        : targetHue(targetHue), colour(colour) {}

    ColourPaletteItem(uint8_t targetHue, uint8_t outputHue)
        : targetHue(targetHue), colour(outputHue, 255, 255) {}
};

typedef std::vector<ColourPaletteItem> ColourPalette;

inline cHSV getColourFromPalette(ColourPalette& palette, uint8_t currentHue)
{
    assert(palette.size() >= 2); // Palette must have atleast 2 values
    assert(palette.at(0).targetHue == 0); // Palette must start with 0
    assert(palette.at(palette.size() - 1).targetHue == 255); // Palette must end with 255

#define lerp(a, b, t) (a + t * (b - a))
    uint32_t index = 0;
    for (size_t i = 0; i < palette.size() - 1; i++)
    {
        if (palette.at(i + 1).targetHue >= currentHue)
        {
            index = i;
            break;
        }
    }

    cHSV colour1 = palette.at(index).colour;
    uint8_t targetHue1 = palette.at(index).targetHue;
    cHSV colour2 = palette.at(index + 1).colour;
    uint8_t targetHue2 = palette.at(index + 1).targetHue;

    float value = (float)std::abs(currentHue - targetHue1) / (float)(targetHue2 - targetHue1);

    uint8_t hue = lerp(colour1.hue, colour2.hue, value);
    uint8_t sat = lerp(colour1.sat, colour2.sat, value);
    uint8_t val = lerp(colour1.val, colour2.val, value);

#undef lerp
    return cHSV(hue, sat, val);
}

#endif
