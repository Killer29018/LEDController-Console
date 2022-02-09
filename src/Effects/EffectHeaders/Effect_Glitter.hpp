#ifndef EFFECT_GLITTER_HPP
#define EFFECT_GLITTER_HPP

#include "Effect.hpp"

class Effect_Glitter : public Effect
{
private:
    uint8_t m_DeltaHue;
    uint8_t m_HueOffset;

    bool m_Reversed = false;
public:
    Effect_Glitter();
    ~Effect_Glitter();

    void update(LEDMatrix* matrix) override;
    void render(const char* panelName) override;
};

#endif
