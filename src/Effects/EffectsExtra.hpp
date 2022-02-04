#ifndef EFFECT_EFFECT_EXTRA_HPP
#define EFFECT_EFFECT_EXTRA_HPP

#include <vector>

enum class EffectEnum
{
    NONE        = 0,
    SOLID_COLOUR= 1,
    RAINBOW     = 2,
};

const std::vector<const char*> EffectName = {
    "Blank",
    "Solid Colour",
    "Rainbow"
};

#endif
