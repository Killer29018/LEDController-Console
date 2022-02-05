#include "../EffectHeaders/Effect_Glitter.hpp"

Effect_Glitter::Effect_Glitter()
    : Effect(EffectEnum::GLITTER)
{
    m_DeltaHue = 1;
    m_HueOffset = 0;
}

Effect_Glitter::~Effect_Glitter() {}

void Effect_Glitter::update(LEDMatrix* matrix, float dt)
{
    uint8_t hue = m_PrimaryColour.getHue();

    m_DeltaTotal += dt;
    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        m_DeltaTotal = 0;

        cRGB colour;
        for (int i = 0; i < matrix->getColumns(); i++)
        {
            cHSV newHue = cHSV(hue + (i * m_DeltaHue), 255, 255);
            HSV2RGB_rainbow(newHue + m_HueOffset, colour);

            matrix->fillColumn(colour, i);
        }

        if (!m_Reversed) m_HueOffset--;
        else m_HueOffset++;
    }
}

void Effect_Glitter::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        uint8_t hue = m_DeltaHue;
        uint8_t min = 0, max = 32;
        ImGui::Text("Hue Change");
        ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &hue, &min, &max, "%u");
        m_DeltaHue = hue;

        ImGui::Text("Reversed");
        ImGui::Checkbox("##Reversed", &m_Reversed);

        ImGui::PopItemWidth();

        ImGui::End();
    }
}
