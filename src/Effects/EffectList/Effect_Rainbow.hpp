#ifndef EFFECTS_RAINBOW_HPP
#define EFFECTS_RAINBOW_HPP

#include "Effect.hpp"

class Effect_Rainbow : public Effect
{
private:
    uint8_t deltaHue;
public:
    Effect_Rainbow() 
        : Effect(EffectEnum::RAINBOW) 
    {
        deltaHue = 1;
    }

    ~ Effect_Rainbow() {}

    void updateEffect(LEDMatrix* matrix, float dt) override
    {
        static uint8_t hue = 0;

        m_DeltaTotal += dt;
        if (m_DeltaTotal >= (1.0 / (float)m_FPS))
        {
            matrix->fillRainbow(cHSV(hue++, 255, 255), deltaHue);

            m_DeltaTotal = 0;
        }
    }

    void render(const char* panelName) override
    {
        if (ImGui::Begin(panelName))
        {
            ImGui::PushItemWidth(-1);
            int hue = deltaHue;
            ImGui::Text("Hue Change");
            ImGui::SliderInt("##DeltaHue", &hue, 1, 64);
            deltaHue = hue;
            ImGui::PopItemWidth();

            ImGui::End();
        }
    }
};

#endif
