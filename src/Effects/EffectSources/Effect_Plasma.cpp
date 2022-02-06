#include "../EffectHeaders/Effect_Plasma.hpp"

#include "../../Utils/DiamondSquare.hpp"

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
    const uint32_t size = DiamondSquare::getSize();

    const int32_t minX = 0;
    const int32_t maxX = size - matrix->getColumns() - 1;
    const int32_t minY = 0;
    const int32_t maxY = size - matrix->getRows() - 1;

    if (m_DeltaTotal >= (1.0 / (float)m_FPS))
    {
        for (int i = 0; i < matrix->getColumns(); i++)
        {
            for (int j = 0; j < matrix->getRows(); j++)
            {
                uint32_t x = i + m_OffsetX;
                uint32_t y = j + m_OffsetY;
                uint8_t value = DiamondSquare::getValue(x, y);
                cHSV colour(value + m_HueOffset, 255, 255);
                matrix->setLED(i, j, colour);
            }
        }

        m_OffsetX = ((maxX / 2) * std::sin(m_Angle + 2.13)) + (maxX / 2);
        m_OffsetX = std::max(minX, std::min(m_OffsetX, maxX));
        m_OffsetY = ((maxX / 2)* std::cos(m_Angle + 1.534)) + (maxX / 2);
        m_OffsetY = std::max(minY, std::min(m_OffsetY, maxY));

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
