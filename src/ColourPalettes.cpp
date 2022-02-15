#include "ColourPalettes.hpp"

#include "Panels/Logger.hpp"

ColourPalette defaultPalette =
{
    { 0, 0 },
    { 255, 255, }
};

cHSV getColourFromPalette(const ColourPalette& palette, cHSV colour)
{
    return getColourFromPalette(palette, colour.h);
}

cHSV getColourFromPalette(const ColourPalette& palette, uint8_t currentHue)
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

uint8_t getHueFromPalette(const ColourPalette& palette, cHSV colour)
{
    return getColourFromPalette(palette, colour.h).h;
}

uint8_t getHueFromPalette(const ColourPalette& palette, uint8_t hue)
{
    return getColourFromPalette(palette, hue).h;
}
