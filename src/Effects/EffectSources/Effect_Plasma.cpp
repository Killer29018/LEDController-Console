#include "../EffectHeaders/Effect_Plasma.hpp"

#include <glm/gtc/noise.hpp>

Effect_Plasma::Effect_Plasma()
    : Effect(EffectEnum::PLASMA)
{
    m_OffsetX = 0;
    m_OffsetY = 0;
    m_HueOffset = 0;
    m_Angle = 0.0f;

    m_HueChange = 1;
    m_AngleChange = 1;
}

Effect_Plasma::~Effect_Plasma() {}

void Effect_Plasma::update(LEDMatrix* matrix, float dt)
{
    m_DeltaTotal += dt;

    const int32_t maxX = 2 << 8;
    const int32_t maxY = 2 << 8;

    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        for (int i = 0; i < matrix->getColumns(); i++)
        {
            for (int j = 0; j < matrix->getRows(); j++)
            {
                float x = (i + m_OffsetX) / (float)(matrix->getColumns() + (maxX / 4.0));
                float y = (j + m_OffsetY) / (float)(matrix->getRows() + (maxY / 4.0));
                float fvalue = glm::simplex(glm::vec2(x, y));
                uint8_t value = (fvalue + 1) * 128;
                // uint8_t value = DiamondSquare::getValue(x, y);
                cHSV colour(value + m_HueOffset, 255, 255);
                matrix->setLED(i, j, colour);
            }
        }

        m_OffsetX = maxX * std::sin(m_Angle + 2.13);
        m_OffsetY = maxY * std::cos(m_Angle + 1.534);

        if (m_AngleChange > 0) m_Angle += (m_AngleChange / 1000.0f);


        m_HueOffset += m_HueChange;
        m_DeltaTotal = 0;
    }
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
