#include "AllEffects.hpp"

Effect_Rainbow::Effect_Rainbow()
    : Effect(EffectEnum::RAINBOW) 
{
    deltaHue = 1;
}

Effect_Rainbow::~Effect_Rainbow() {}

void Effect_Rainbow::updateEffect(LEDMatrix* matrix, float dt)
{
    static uint8_t hue = 0;
    m_DeltaTotal += dt;

    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        matrix->fillRainbow(cHSV(hue++, 255, 255), deltaHue);

        m_DeltaTotal = 0;
    }
}

void Effect_Rainbow::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        int hue = deltaHue;
        ImGui::Text("Hue Change");
        ImGui::SliderInt("##DeltaHue", &hue, 0, 16);
        deltaHue = hue;

        ImGui::PopItemWidth();
        ImGui::End();
    }
}
