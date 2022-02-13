#include "../EffectHeaders/Effect_Fire.hpp"

#include "../../Panels/Logger.hpp"

#include "../../Utils/Helper.hpp"

#include "glm/gtc/noise.hpp"

Effect_Fire::Effect_Fire(LEDMatrix* matrix)
    : Effect(EffectEnum::FIRE, matrix)
{ 
    m_OffsetY = 0.0f;
    m_BufferSizeX = m_Matrix->getColumns();
    m_BufferSizeY = m_Matrix->getRows();
    buffer1 = new cHSV[m_BufferSizeX * m_BufferSizeY];
    buffer2 = new cHSV[m_BufferSizeX * m_BufferSizeY];
    checkBufferSize();

    memset(buffer1, 0, m_BufferSizeX * m_BufferSizeY * sizeof(cHSV));
    memset(buffer2, 0, m_BufferSizeX * m_BufferSizeY * sizeof(cHSV));

    for (int i = 0; i < m_Matrix->getColumns(); i++)
    {
        buffer1[getIndex(i, m_Matrix->getRows() - 1)] = cHSV(0, 255, 255);
    }
}

Effect_Fire::~Effect_Fire()
{
    delete[] buffer1;
    delete[] buffer2;
}


void Effect_Fire::update()
{
    for (int i = 0; i < m_Matrix->getColumns(); i++)
    {
        buffer1[getIndex(i, m_Matrix->getRows() - 1)] = cHSV(0, 255, 255);
        buffer1[getIndex(i, m_Matrix->getRows() - 2)] = cHSV(0, 255, 128);
    }

    uint8_t n1, n2, n3, n4;
    float c;
    for (uint32_t y = 1; y < m_Matrix->getRows() - 1; y++)
    {
        for (uint32_t x = 1; x < m_Matrix->getColumns() - 1; x++)
        {
            n1 = buffer1[getIndex(x + 1, y)].v;
            n2 = buffer1[getIndex(x - 1, y)].v;
            n3 = buffer1[getIndex(x, y + 1)].v;
            n4 = buffer1[getIndex(x, y - 1)].v;

            c = (getValue(x, y) / 255.0f) * 10.0f;
            int16_t value = (n1 + n2 + n3 + n4) / 4;
            value -= c;

            buffer2[getIndex(x,y-1)].v = value;
            value = std::min((int16_t)255, std::max(value, (int16_t)0));
            Logger::log(LoggerType::LOG_INFO, "%d", value);

            cHSV colour = cHSV(0, 255, value);
            m_Matrix->setLED(x, y - 1, colour);
        }
    }

    memcpy(buffer1, buffer2, m_BufferSizeX * m_BufferSizeY * sizeof(cHSV));

    m_OffsetY += random() * 0.04f;
    m_OffsetX += 0.01f * glm::perlin(glm::vec2(1.0f, m_OffsetY));
}

void Effect_Fire::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {

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
        buffer1 = (cHSV*)realloc((void*)buffer1, newSize);
        buffer2 = (cHSV*)realloc((void*)buffer2, newSize);
    }
}


