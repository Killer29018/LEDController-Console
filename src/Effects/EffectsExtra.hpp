#ifndef EFFECT_EFFECT_EXTRA_HPP
#define EFFECT_EFFECT_EXTRA_HPP

#include <vector>

enum class EffectEnum
{
    NONE,
    SOLID_COLOUR,
    RAINBOW,
    GLITTER,
    PLASMA,
    METABALLS,
    FIREWORKS,
    FALLING_RAIN,
    FADING_PIXELS,
    FIRE,
    TEXT,
    PONG,
    SNAKE,
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
    "Fading Pixels",
    "Fire",
    "Text",
    "Pong",
    "Snake",
};

#endif
