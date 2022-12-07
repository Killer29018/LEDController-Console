#ifndef EFFECT_EFFECT_EXTRA_HPP
#define EFFECT_EFFECT_EXTRA_HPP

#include <vector>

enum class EffectEnum : int
{
    NONE,
    RAINBOW,
    GLITTER,
    PLASMA,
    METABALLS,
    FIREWORKS,
    FALLING_RAIN,
    FADING_PIXELS,
    FIRE,
    SNAKE,
};

const std::vector<const char*> EffectName = {
    "Blank",
    "Rainbow",
    "Glitter",
    "Plasma",
    "Metaballs",
    "Fireworks",
    "Falling Rain",
    "Fading Pixels",
    "Fire",
    "Snake",
};

#endif
