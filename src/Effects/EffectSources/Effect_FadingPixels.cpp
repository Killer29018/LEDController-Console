#include "../EffectHeaders/Effect_FadingPixels.hpp"

#include "../../Utils/Helper.hpp"

#include "../../Panels/Logger.hpp"

#include <cmath>

Effect_FadingPixels::Effect_FadingPixels(LEDMatrix* matrix)
    : Effect(EffectEnum::FADING_PIXELS, matrix)
{
    m_RandomPixels = 20;
    m_DecaySpeed = 10;

    m_HueOffset = 0;
    m_DeltaHue = 1;
    m_AnimateHue = false;
    m_RandomColour = false;

    m_CurrentCount = 0;
    m_MaxCount = 10;
}

Effect_FadingPixels::~Effect_FadingPixels()
{
}

void Effect_FadingPixels::update()
{
    setRandomPixels();
    decayPixels();

    if (m_AnimateHue)
    {
        m_CurrentCount++;

        if (m_CurrentCount >= m_MaxCount)
        {
            m_HueOffset += m_DeltaHue;
            m_CurrentCount = 0;
        }
    }
}

void Effect_FadingPixels::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::Text("Count");
        uint16_t min = 1, max = 64;
        ImGui::SliderScalar("##Count", ImGuiDataType_U16, &m_RandomPixels, &min, &max, "%u");

        ImGui::Text("Decay Speed");
        ImGui::SliderScalar("##Decay", ImGuiDataType_U16, &m_DecaySpeed, &min, &max, "%u");

        ImGui::Text("Animate Hue");
        if (ImGui::Checkbox("##AnimateHue", &m_AnimateHue))
        {
            if (!m_AnimateHue) m_HueOffset = 0;
        }

        if (m_AnimateHue)
        {
            ImGui::Text("Delta Hue");
            min = 0;
            max = 255;
            ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &m_DeltaHue, &min, &max, "%u");

            ImGui::Text("Hue Update Speed");
            uint8_t value = max - m_MaxCount;
            ImGui::SliderScalar("##HueUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
            m_MaxCount = max - value;
        }
        else
        {
            ImGui::Text("Random Colours");
            ImGui::Checkbox("##RandomColours", &m_RandomColour);
        }

    }
    ImGui::End();
}

void Effect_FadingPixels::setRandomPixels()
{
    cHSV hue;
    for (uint32_t i = 0; i < m_RandomPixels; i++)
    {
        int x = (rand() % m_Matrix->getColumns());
        int y = (rand() % m_Matrix->getRows());

        if (m_Matrix->getLED(x, y).v != 0)
            continue;

        if (m_RandomColour)
            hue = cHSV((uint8_t)(randomValue() * 255));
        else
            hue = m_PrimaryColour;

        hue.h += m_HueOffset;

        m_Matrix->setLED(x, y, hue);
    }
}

void Effect_FadingPixels::decayPixels()
{
    for (uint32_t y = 0; y < m_Matrix->getRows(); y++)
    {
        for (uint32_t x = 0; x < m_Matrix->getColumns(); x++)
        {
            cHSV colour = m_Matrix->getLED(x, y);

            int value = colour.v;
            if (value > 0)
                value -= randomValue() * m_DecaySpeed;

            value = std::min(255, std::max(0, value));
            colour.v = value;

            m_Matrix->setLED(x, y, colour);
        }
    }
}
