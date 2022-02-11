#include "../EffectHeaders/Effect_FadingPixels.hpp"

#include "../../Utils/Helper.hpp"

#include "../../Panels/Logger.hpp"

#include <cmath>

Effect_FadingPixels::Effect_FadingPixels(LEDMatrix* matrix)
    : Effect(EffectEnum::FADING_PIXELS, matrix)
{
    m_RandomPixels = 20;
    m_DecaySpeed = 10;

    m_ValuesCols = m_Matrix->getColumns();
    m_ValuesRows = m_Matrix->getRows();

    m_Colours = new cHSV[m_ValuesRows * m_ValuesCols];
    memset(m_Colours, 0, m_ValuesRows * m_ValuesCols * sizeof(cHSV));

    m_HueOffset = 0;
    m_DeltaHue = 1;
    m_AnimateHue = true;
    m_RandomColour = false;

    m_Count = 0;
    m_CountMax = 20;
}

Effect_FadingPixels::~Effect_FadingPixels()
{
    delete[] m_Colours;
}

void Effect_FadingPixels::update()
{
    checkValuesSize();
    // m_Matrix->setLED(1, 1, { 255, 255, 255 });
    setRandomPixels();
    decayPixels();


    if (m_AnimateHue)
    {
        m_Count++;

        if (m_Count >= m_CountMax)
        {
            m_HueOffset += m_DeltaHue;
            m_Count = 0;
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

        ImGui::Text("Delta Hue");
        min = 0;
        max = 255;
        ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &m_DeltaHue, &min, &max, "%u");

        ImGui::Text("Hue Update Speed");
        uint8_t value = max - m_CountMax;
        ImGui::SliderScalar("##HueUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
        m_CountMax = max - value;

        ImGui::Text("Random Colours");
        ImGui::Checkbox("##RandomColours", &m_RandomColour);
    }
    ImGui::End();
}

void Effect_FadingPixels::setRandomPixels()
{
    uint8_t hue;
    for (uint32_t i = 0; i < m_RandomPixels; i++)
    {
        int x = (rand() % m_Matrix->getColumns());
        int y = (rand() % m_Matrix->getRows());

        if (getValue(x, y).v != 0)
            continue;

        if (m_RandomColour)
            hue = random() * 255;
        else
            hue = m_PrimaryColour.getHue();

        setValue(x, y, { hue + m_HueOffset, 255, 255 });
        m_Matrix->setLED(x, y, getValue(x, y));
    }
}

void Effect_FadingPixels::decayPixels()
{
    for (uint32_t y = 0; y < m_Matrix->getRows(); y++)
    {
        for (uint32_t x = 0; x < m_Matrix->getColumns(); x++)
        {
            cHSV colour = getValue(x, y);

            int value = colour.v;
            if (value > 0)
                value -= random() * m_DecaySpeed;

            value = std::min(255, std::max(0, value));
            colour.v = value;

            setValue(x, y, colour);

            m_Matrix->setLED(x, y, colour);
        }
    }
}

void Effect_FadingPixels::checkValuesSize()
{
    if (m_Matrix->getColumns() != m_ValuesCols || m_Matrix->getRows() != m_ValuesRows)
    {
        m_ValuesCols = m_Matrix->getColumns();
        m_ValuesRows = m_Matrix->getRows();
        m_Colours = (cHSV*)realloc((void*)m_Colours, m_ValuesCols * m_ValuesRows * sizeof(cHSV));
    }
}

uint32_t Effect_FadingPixels::getIndex(int x, int y)
{
    return (y * m_ValuesCols) + x;
}

void Effect_FadingPixels::setValue(int x, int y, cHSV value)
{
    m_Colours[getIndex(x, y)] = value;
}

cHSV Effect_FadingPixels::getValue(int x, int y)
{
    return m_Colours[getIndex(x, y)];
}
