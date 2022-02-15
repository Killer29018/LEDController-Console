#ifndef COLOUR_PALETTES_HPP
#define COLOUR_PALETTES_HPP

#include "Panels/Logger.hpp"
#include "Colour.hpp"

#include <cassert>
#include <vector>

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

namespace Palettes
{
    extern std::vector<const char*> PaletteNames;

    enum PaletteEnum
    {
        DEFAULT = 0,
        OCEAN   = 1,
        CUSTOM  = 2, // Will be used for users to create their own palette
    };

    extern std::vector<ColourPalette> AllPalettes;
}


cHSV getColourFromPalette(const ColourPalette& palette, uint8_t colour);
cHSV getColourFromPalette(const ColourPalette& palette, cHSV colour);
uint8_t getHueFromPalette(const ColourPalette& palette, uint8_t colour);
uint8_t getHueFromPalette(const ColourPalette& palette, cHSV colour);

cHSV getColourFromPalette(Palettes::PaletteEnum palette, uint8_t currentHue);
cHSV getColourFromPalette(Palettes::PaletteEnum palette, cHSV colour);
uint8_t getHueFromPalette(Palettes::PaletteEnum palette, uint8_t hue);
uint8_t getHueFromPalette(Palettes::PaletteEnum palette, cHSV colour);


#endif
