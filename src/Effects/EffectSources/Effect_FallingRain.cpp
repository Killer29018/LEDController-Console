#include "../EffectHeaders/Effect_FallingRain.hpp"

#include "../../Panels/Logger.hpp"

static float random() { return (rand() / (float)RAND_MAX); }

static float mapValue(float initial, float iS, float iE, float oS, float oE)
{
    float slope = (oE - oS) / (iE - iS);
    return oS + slope * (initial - iS);
}

static float s_RaindropMinSpeed = 0.5f;
static float s_RaindropMaxSpeed = 0.9f;
static float s_RaindropMaxAcceleration = 0.2f;
static uint32_t s_TrailLength = 7;

Effect_FallingRain::Effect_FallingRain(LEDMatrix* matrix)
    : Effect(EffectEnum::FALLING_RAIN, matrix) 
{
    m_CurrentRaindrops = 20;
    m_RainbowColours = false;

    createRaindrops();
}

Effect_FallingRain::~Effect_FallingRain() {}

void Effect_FallingRain::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });
    updateRaindrops();
    renderRaindrops();
}

void Effect_FallingRain::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("Rainbow Colours");
        ImGui::Checkbox("##Rainbow", &m_RainbowColours);

        ImGui::Text("Raindrop Count");
        uint32_t min = 1, max = MAX_RAINDROPS;
        ImGui::SliderScalar("##Raindrops", ImGuiDataType_U32, &m_CurrentRaindrops, &min, &max, "%u");

        float minF = 0.01f, maxF = 1.0f;
        ImGui::DragFloatRange2("##Speed", &s_RaindropMinSpeed, &s_RaindropMaxSpeed, 0.01f, minF, maxF, "%.3f", "%.3f");

        ImGui::Text("Max Acceleration");
        ImGui::SliderScalar("##MaxAcceleration", ImGuiDataType_Float, &s_RaindropMaxAcceleration, &minF, &maxF, "%.3ff");

        min = 0;
        max = m_Matrix->getRows();
        ImGui::Text("Trail Length");
        ImGui::SliderScalar("##TrailLength", ImGuiDataType_U32, &s_TrailLength, &min, &max, "%u");

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void Effect_FallingRain::createRaindrops()
{
    for (uint32_t i = 0; i < MAX_RAINDROPS; i++)
    {
        resetRaindrop(m_Raindrops[i]);
    }
}

void Effect_FallingRain::resetRaindrop(Raindrop& raindrop)
{
    float x = random() * m_Matrix->getColumns();
    float y = -(random() * m_Matrix->getRows());
    raindrop = Raindrop(x, y, &m_RainbowColours);
}

void Effect_FallingRain::updateRaindrops()
{
    for (uint32_t i = 0; i < m_CurrentRaindrops; i++)
    {
        m_Raindrops[i].update();

        if (m_Raindrops[i].posY >= m_Matrix->getRows() + s_TrailLength)
            resetRaindrop(m_Raindrops[i]);
    }
}

void Effect_FallingRain::renderRaindrops()
{
    for (uint32_t i = 0; i < m_CurrentRaindrops; i++)
    {
        m_Raindrops[i].draw(m_Matrix, m_PrimaryColour);
    }
}

// RAINDROP


Raindrop::Raindrop(float x, float y, bool* useRandom)
    : posX(x), posY(y)
{
    m_RandomColour = useRandom;
    colour = cHSV(random() * 255, 255, 255);

    velY = (random() * (s_RaindropMaxSpeed - s_RaindropMaxSpeed)) + s_RaindropMinSpeed;

    accY = random() * (s_RaindropMaxAcceleration);
}

void Raindrop::update()
{
    posY += velY;
    velY += accY;

    velY = std::min(velY, s_RaindropMaxSpeed);
}

void Raindrop::draw(LEDMatrix* matrix, cRGB targetColour)
{
    cHSV currentColour;
    for (uint32_t i = 0; i < s_TrailLength; i++)
    {
        if (*m_RandomColour)
            currentColour = cHSV(colour.hue, colour.sat, colour.v);
        else
            currentColour = targetColour;

        float y = posY - i;
        float value = mapValue(i, 0, s_TrailLength - 1, 255, 64);
        currentColour.v = value;

        if (y >= 0 && y < matrix->getRows())
        {
            matrix->setLED(posX, y, currentColour);
        }
    }
}
