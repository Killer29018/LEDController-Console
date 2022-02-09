#ifndef EFFECT_PLASMA_HPP
#define EFFECT_PLASMA_HPP

#include "Effect.hpp"

class Effect_Plasma : public Effect
{
private:
    float m_Angle;
    float m_OffsetX, m_OffsetY;
    uint8_t m_HueOffset;

    uint8_t m_AngleChange;
    uint8_t m_HueChange;
public:
    Effect_Plasma();
    ~Effect_Plasma();

    void update(LEDMatrix* matrix) override;
    void render(const char* panelName) override;
};

#endif
