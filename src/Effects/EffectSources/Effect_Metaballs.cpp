#include "../EffectHeaders/Effect_Metaballs.hpp"

#include <cmath>

#include "../../Panels/Logger.hpp"

Effect_Metaballs::Effect_Metaballs(LEDMatrix* matrix)
    : Effect(EffectEnum::METABALLS, matrix)
{
    m_Threshold = 128;
    m_CurrentBlobs = 2;
    m_MaxSpeed = 0.3f;
    m_Size = 5;

    createBlobs();
}

Effect_Metaballs::~Effect_Metaballs() {}

void Effect_Metaballs::update()
{
    uint8_t hue = m_PrimaryColour.getHue();

    m_Matrix->fillSolid({ 0, 0, 0 });
    for (int x = 0; x < m_Matrix->getColumns(); x++)
    {
        for (int y = 0; y < m_Matrix->getRows(); y++)
        {
            uint8_t value = (255 - getValue(x, y));
            cHSV colour(value + hue, 255, 255);

            if (value < 255 - m_Threshold)
            {
                m_Matrix->setLED(x, y, colour);
            }
        }
    }

    updateBlobs();
}

void Effect_Metaballs::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);


        uint16_t min = 1;
        uint16_t max = 10;

        
        ImGui::Text("Count");
        ImGui::SliderScalar("##Amount", ImGuiDataType_U16, &m_CurrentBlobs, &min, &max, "%u");

        min = 0;
        max = 255;
        ImGui::Text("Threshold");
        uint8_t value = 255 - m_Threshold;
        ImGui::SliderScalar("##Threshold", ImGuiDataType_U8, &value, &min, &max, "%u");
        m_Threshold = 255 - value;

        min = 1;
        max = 10;
        ImGui::Text("Size");
        ImGui::SliderScalar("##Size", ImGuiDataType_U8, &m_Size, &min, &max, "%u");


        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void Effect_Metaballs::createBlobs()
{
    int width  = m_Matrix->getColumns();
    int height = m_Matrix->getRows();

    for (int i = 0; i < MAX_BLOBS; i++)
    {
        m_Blobs[i].x = (rand() % width) + 0.01;
        m_Blobs[i].y = (rand() % height) + 0.01;
        m_Blobs[i].velX = ((rand() % 1000) * (m_MaxSpeed * 2.0f) / 1000.0f) - m_MaxSpeed;
        m_Blobs[i].velY = ((rand() % 1000) * (m_MaxSpeed * 2.0f) / 1000.0f) - m_MaxSpeed;
        randomiseAcc(m_Blobs[i]);
    }
}

void Effect_Metaballs::updateBlobs()
{
    int width = m_Matrix->getColumns();
    int height = m_Matrix->getRows();

    float x, y;
    for (int i = 0; i < m_CurrentBlobs; i++)
    {
        x = m_Blobs[i].x;
        y = m_Blobs[i].y;
        x += m_Blobs[i].velX;
        y += m_Blobs[i].velY;

        updateVel(m_Blobs[i]);
        randomiseAcc(m_Blobs[i]);

        if (x < 0)
        {
            x = 0;
            m_Blobs[i].velX *= -1;
        }
        else if (x > width)
        {
            x = width;
            m_Blobs[i].velX *= -1;
        }

        if (y < 0)
        {
            y = 0;
            m_Blobs[i].velY *= -1;
        }
        else if (y > height)
        {
            y = height;
            m_Blobs[i].velY *= -1;
        }

        m_Blobs[i].x = x;
        m_Blobs[i].y = y;
    }
}

void Effect_Metaballs::randomiseAcc(Blob& b)
{
    float value = (m_MaxSpeed / 8.0f);
    b.accX = (((rand() % 1000) * (value * 2.0f)) / 1000.0f) - value;
    b.accY = (((rand() % 1000) * (value * 2.0f)) / 1000.0f) - value;
}

void Effect_Metaballs::updateVel(Blob& b)
{
    b.velX += b.accX;
    b.velY += b.accY;
    b.velX = std::min(b.velX, m_MaxSpeed);
    b.velY = std::min(b.velY, m_MaxSpeed);
}

uint8_t Effect_Metaballs::getValue(int x, int y)
{
    float sum = 0;
    for (uint16_t i = 0; i < m_CurrentBlobs; i++)
    {
        float dX = std::pow((m_Blobs[i].x - x), 2);
        float dY = std::pow((m_Blobs[i].y - y), 2);
        sum += 1 / std::sqrt(dX + dY);
    }
    int value = (sum * m_Size * 100);
    value = std::min(value, 255);
    if (value < m_Threshold)
        value = 0;

    return (uint8_t)value;
}
