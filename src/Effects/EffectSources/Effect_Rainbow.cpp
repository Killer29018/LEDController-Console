#include "../EffectHeaders/Effect_Rainbow.hpp"

Effect_Rainbow::Effect_Rainbow()
    : Effect(EffectEnum::RAINBOW) 
{
    m_DeltaHue = 1;
    m_ChangeHue = true;
}

Effect_Rainbow::~Effect_Rainbow() {}

void Effect_Rainbow::update(LEDMatrix* matrix, float dt)
{
    uint8_t hue = m_PrimaryColour.getHue();

    m_DeltaTotal += dt;

    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        matrix->fillRainbow(cHSV(hue + m_HueOffset, 255, 255), m_DeltaHue);

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

        uint8_t min = 0, max = 16;
        ImGui::Text("Hue Change");
        ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &m_DeltaHue, &min, &max, "%u");


        ImGui::Text("Animate the Hue");
        if (ImGui::Checkbox("##ChangeHue", &m_ChangeHue))
        {
            m_HueOffset = 0;
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();
}
