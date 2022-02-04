#include "../EffectHeaders/Effect_Rainbow.hpp"

Effect_Rainbow::Effect_Rainbow()
    : Effect(EffectEnum::RAINBOW) 
{
    m_DeltaHue = 1;
    m_ChangeHue = true;
    m_CurrentHue = 0;
}

Effect_Rainbow::~Effect_Rainbow() {}

void Effect_Rainbow::update(LEDMatrix* matrix, float dt)
{
    uint8_t hue = m_PrimaryColour.getHue();
    if (m_CurrentHue != hue)
        m_CurrentHue = hue;

    m_DeltaTotal += dt;

    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        matrix->fillRainbow(cHSV(m_CurrentHue + m_HueOffset, 255, 255), m_DeltaHue);

        if (m_ChangeHue)
            m_HueOffset++;
        else
            m_HueOffset = 0;

        m_DeltaTotal = 0;
    }
}

void Effect_Rainbow::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        int hue = m_DeltaHue;
        ImGui::Text("Hue Change");
        ImGui::SliderInt("##DeltaHue", &hue, 0, 16);
        m_DeltaHue = hue;


        ImGui::Text("Animate the Hue");
        if (ImGui::Checkbox("##ChangeHue", &m_ChangeHue))
        {
            m_HueOffset = 0;
        }


        ImGui::PopItemWidth();
        ImGui::End();
    }
}
