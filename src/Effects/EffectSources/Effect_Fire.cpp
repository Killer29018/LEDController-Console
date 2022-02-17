#include "../EffectHeaders/Effect_Fire.hpp"

#include "../../Panels/Logger.hpp"

#include "../../Utils/Helper.hpp"

#include "glm/gtc/noise.hpp"

ColourPalette Effect_Fire::m_FirePalette = {
    {   0,   0 },
    { 130,  16 },
    { 190,  32 },
    { 220,  48 },
    { 255,  58 }
};

Effect_Fire::Effect_Fire(LEDMatrix* matrix)
    : Effect(EffectEnum::FIRE, matrix)
{ 
    m_OffsetY = 0.0f;
    m_OffsetX = 0.0f;
    m_CoolingFactor = 40.0f;
    m_Speed = 0.04f;

    m_BufferSizeX = m_Matrix->getColumns();
    m_BufferSizeY = m_Matrix->getRows();
    m_Buffer1.resize(m_BufferSizeX * m_BufferSizeY);
    m_Buffer2.resize(m_BufferSizeX * m_BufferSizeY);

    checkBufferSize();

    for (uint32_t y = 0; y < m_Matrix->getRows(); y++)
    {
        for (uint32_t x = 0; x < m_Matrix->getColumns(); x++)
        {
            m_Buffer1.at(getIndex(x, y)) = cHSV(0, 255, 255);
            m_Buffer2.at(getIndex(x, y)) = cHSV(0, 255, 255);
        }
    }
}

Effect_Fire::~Effect_Fire() {}


void Effect_Fire::update()
{
    checkBufferSize();

    for (uint32_t i = 0; i < m_Matrix->getColumns(); i++)
    {
        uint8_t value = (255-60) + randomValue() * 60;
        m_Buffer1.at(getIndex(i, m_Matrix->getRows() - 1)) = cHSV(0, 255, value);
        m_Buffer2.at(getIndex(i, m_Matrix->getRows() - 1)) = cHSV(0, 255, value);
    }
    m_Matrix->fillSolid({ 0, 0, 0 });

    for (uint32_t y = 0; y < m_Matrix->getRows(); y++)
    {
        for (uint32_t x = 0; x < m_Matrix->getColumns(); x++)
        {
            uint8_t newValue;
            if (y < m_Matrix->getRows() - 1)
                newValue = getAverage(x, y);
            else
                newValue = m_Buffer1.at(getIndex(x, y)).v;

            cHSV& c = m_Buffer2.at(getIndex(x, y));
            c.v = newValue;

            uint8_t hue = newValue;
            c.h = getHueFromPalette(m_FirePalette, hue);

            m_Buffer2.at(getIndex(x, y)) = c;
            m_Matrix->setLED(x, y, c);
        }
    }

    m_Buffer1 = m_Buffer2;

    m_OffsetY += randomValue() * m_Speed;
    m_OffsetX += m_Speed * glm::perlin(glm::vec2(1.0f, m_OffsetY));
}

void Effect_Fire::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        uint8_t min = 0, max = 100;

        ImGui::Text("Movement Speed");
        uint8_t value = 100 * m_Speed;
        ImGui::SliderScalar("##MovementSpeed", ImGuiDataType_U8, &value, &min, &max, "%u");
        m_Speed = value / 100.0f;

        ImGui::Text("Cooling Speed");
        value = m_CoolingFactor;
        max = 255.0f;
        ImGui::SliderScalar("##CoolingSpeed", ImGuiDataType_U8, &value, &min, &max, "%u");
        m_CoolingFactor = value;
    }
    ImGui::End();
}

uint8_t Effect_Fire::getValue(uint32_t x, uint32_t y)
{
    glm::vec2 pos;
    float value = 0.0f;
    constexpr int runs = 3;
    float maxValue = 0;
    for (int i = 0; i < runs; i++) { maxValue += (1.0 / (i + 1)); } 
    for (int i = 0; i < runs; i++)
    {
        uint32_t addition = (1 << (runs - i - 1));
        float divisorX = m_Matrix->getColumns() + addition;
        float divisorY = m_Matrix->getRows() + addition;
        pos = glm::vec2(m_OffsetX + x / divisorX, m_OffsetY + y / divisorY);
        value += (1.0 / (i + 1)) * ((glm::perlin(pos) + 1) / 2.0f);
    }
    uint8_t returnValue = mapValue(value, 0, maxValue, 0, 255);
    return returnValue;
}

uint32_t Effect_Fire::getIndex(uint32_t x, uint32_t y)
{
    return (y * m_BufferSizeX) + x;
}

void Effect_Fire::checkBufferSize()
{
    if (m_Matrix->getColumns() != m_BufferSizeX || m_Matrix->getRows() != m_BufferSizeY)
    {
        m_BufferSizeX = m_Matrix->getColumns();
        m_BufferSizeY = m_Matrix->getRows();

        uint32_t newSize = m_BufferSizeX * m_BufferSizeY;
        m_Buffer1.resize(newSize);
        m_Buffer2.resize(newSize);
    }
}

uint8_t Effect_Fire::getAverage(uint32_t x, uint32_t y)
{
   return average_EvenBelow(x, y);
}

uint8_t Effect_Fire::average_EvenBelow(uint32_t x, uint32_t y)
{
    float value = 0.0f;
    uint8_t count = 0;

    const uint32_t maxX = m_Matrix->getColumns() - 1;
    const uint32_t maxY = m_Matrix->getRows() - 1;

    if (y < maxY)
    {
        value += m_Buffer1.at(getIndex(x, y + 1)).v;
        count++;

        if (x > 0)
        {
            value += m_Buffer1.at(getIndex(x - 1, y + 1)).v;
            count++;
        }

        if (x < maxX)
        {
            value += m_Buffer1.at(getIndex(x + 1, y + 1)).v;
            count++;
        }
    }

    if (y < maxY - 1)
    {
        value += m_Buffer1.at(getIndex(x, y + 2)).v;
        count++;
    }

    value /= (float)count;
    value -= (getValue(x, y) / 255.0f) * m_CoolingFactor;

    value = std::max(0.0f, std::min(255.0f, value));

    return value;
}

uint8_t Effect_Fire::getConvertedHue(uint8_t value)
{
    uint8_t value1 = 190, value2 = 130;
    if (value >= value1) // Yellow
        return mapValue(value, value1, 255, 32, 64);
    else if (value >= value2) // Orange
        return mapValue(value, value2, value1, 16, 32);
    else  // Red
        return mapValue(value, 0, value2, 0, 16);
}
