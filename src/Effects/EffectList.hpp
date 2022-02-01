#ifndef EFFECT_EFFECT_LIST_HPP
#define EFFECT_EFFECT_LIST_HPP

enum class EffectList
{
    NONE, 
    RAINBOW
};

inline const char* effectListToString(EffectList effect)
{
    switch (effect)
    {
    case EffectList::NONE: return "No Colour";
    case EffectList::RAINBOW: return "Rainbow";
    default: return "";
    }
}

#endif
