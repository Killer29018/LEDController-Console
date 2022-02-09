#include "../EffectHeaders/Effect_Plasma.hpp"

#include <glm/gtc/noise.hpp>

Effect_Plasma::Effect_Plasma(LEDMatrix* matrix)
    : Effect(EffectEnum::PLASMA, matrix)
{
    m_OffsetX = 0;
    m_OffsetY = 0;
    m_HueOffset = 0;
    m_Angle = 0.0f;

    m_HueChange = 1;
    m_AngleChange = 1;
}

Effect_Plasma::~Effect_Plasma() {}

void Effect_Plasma::update()
{
    const int32_t maxX = 2 << 8;
    const int32_t maxY = 2 << 8;

    for (int i = 0; i < m_Matrix->getColumns(); i++)
    {
        for (int j = 0; j < m_Matrix->getRows(); j++)
        {
            float x = (i + m_OffsetX) / (float)(m_Matrix->getColumns() + (maxX / 4.0));
            float y = (j + m_OffsetY) / (float)(m_Matrix->getRows() + (maxY / 4.0));
            float fvalue = glm::simplex(glm::vec2(x, y));
            uint8_t value = (fvalue + 1) * 128;
            cHSV colour(value + m_HueOffset, 255, 255);

            m_Matrix->setLED(i, j, colour);
        }
    }

    m_OffsetX = maxX * std::sin(m_Angle + 2.13);
    m_OffsetY = maxY * std::cos(m_Angle + 1.534);

    if (m_AngleChange > 0) m_Angle += (m_AngleChange / 1000.0f);


    m_HueOffset += m_HueChange;
}

void Effect_Plasma::render(const char* panelName)
{
     if (ImGui::Begin(panelName))
     {
         ImGui::PushItemWidth(-1);

         uint8_t min = 0, max = 16;
         ImGui::Text("Colour Change Speed");
         ImGui::SliderScalar("##HueChange", ImGuiDataType_U8, &m_HueChange, &min, &max, "%u");

         max = 50;
         ImGui::Text("Movement Speed");
         ImGui::SliderScalar("##MoveSpeed", ImGuiDataType_U8, &m_AngleChange, &min, &max, "%u");

         ImGui::PopItemWidth();
     }
     ImGui::End();
}
