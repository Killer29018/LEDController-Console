#ifndef EFFECT_EFFECT_FADING_PIXELS_HPP
#define EFFECT_EFFECT_FADING_PIXELS_HPP

#include "Effect.hpp"

class Effect_FadingPixels : public Effect
{
private:
    uint16_t m_RandomPixels;
    uint16_t m_DecaySpeed;

    bool m_AnimateHue;
    bool m_RandomColour;

    uint8_t m_HueOffset;
    uint8_t m_DeltaHue;
    uint8_t m_CurrentCount;
    uint8_t m_MaxCount;
public:
    Effect_FadingPixels(LEDMatrix* matrix);
    ~Effect_FadingPixels();

    void update() override;
    // void render(const char* panelName) override;
private:
    void setRandomPixels();
    void decayPixels();
};

#endif
