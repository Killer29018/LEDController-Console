#ifndef EFFECT_EFFECT_FADING_PIXELS_HPP
#define EFFECT_EFFECT_FADING_PIXELS_HPP

#include "Effect.hpp"

class Effect_FadingPixels : public Effect
{
private:
    uint16_t m_RandomPixels;
    uint16_t m_DecaySpeed;

    uint32_t m_ValuesCols;
    uint32_t m_ValuesRows;
    cHSV* m_Colours;

    bool m_AnimateHue;
    bool m_RandomColour;

    uint8_t m_HueOffset;
    uint8_t m_DeltaHue;
    uint8_t m_Count;
    uint8_t m_CountMax;
public:
    Effect_FadingPixels(LEDMatrix* matrix);
    ~Effect_FadingPixels();

    void update() override;
    void render(const char* panelName) override;
private:
    void setRandomPixels();
    void decayPixels();

    void checkValuesSize();

    uint32_t getIndex(int x, int y);
    void setValue(int x, int y, cHSV value);
    cHSV getValue(int x, int y);
};

#endif
