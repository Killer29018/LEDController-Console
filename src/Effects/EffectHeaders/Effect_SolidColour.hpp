#ifndef EFFECT_SOLID_COLOUR_HPP
#define EFFECT_SOLID_COLOUR_HPP

#include "Effect.hpp"

class Effect_SolidColour : public Effect
{
private:
public:
    Effect_SolidColour();
    ~Effect_SolidColour();

    void update(LEDMatrix* matrix) override;
    void render(const char* panelName) override;
};

#endif
