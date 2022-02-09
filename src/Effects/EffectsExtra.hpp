#ifndef EFFECT_EFFECT_EXTRA_HPP
#define EFFECT_EFFECT_EXTRA_HPP

#include <vector>

enum class EffectEnum
{
    NONE        = 0,
    SOLID_COLOUR= 1,
    RAINBOW     = 2,
    GLITTER     = 3,
    PLASMA      = 4,
    METABALLS   = 5,
    FIREWORKS   = 6,
    FALLING_RAIN= 7,
};

const std::vector<const char*> EffectName = {
    "Blank",
    "Solid Colour",
    "Rainbow",
    "Glitter",
    "Plasma",
    "Metaballs",
    "Fireworks",
    "Falling Rain",
};

#endif
