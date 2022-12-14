#ifndef EFFECT_RAINBOW_HPP
#define EFFECT_RAINBOW_HPP

#include "Effect.hpp"

class Effect_Rainbow : public Effect
{
private:
    uint8_t m_DeltaHue;
    uint8_t m_HueOffset;
    bool m_ChangeHue;
public:
    Effect_Rainbow(LEDMatrix* matrix);
    ~Effect_Rainbow();

    void update() override;
    void render(const char* panelName) override;
};

#endif
