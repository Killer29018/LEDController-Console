#ifndef EFFECT_PLASMA_HPP
#define EFFECT_PLASMA_HPP

#include "Effect.hpp"

class Effect_Plasma : public Effect
{
private:
    float m_Angle = 0;
    int32_t m_OffsetX, m_OffsetY;
    uint8_t m_HueOffset;
public:
    Effect_Plasma();
    ~Effect_Plasma();

    void update(LEDMatrix* matrix, float dt) override;
    void render(const char* panelName) override;
};

#endif
