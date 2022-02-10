#include "../EffectHeaders/Effect_Fireworks.hpp"

#include "../../Panels/Logger.hpp"

static float s_ParticleMaxSpeed = 0.05f;
static float s_ParticleDecaySpeed = 2.2f;
static float s_FireworkDeceleration = 0.1;
static uint32_t s_FireworkMinDistance = 2;
static uint32_t s_TrailIntensity = 5;

static bool s_FireworkRandomColour = false;
static bool s_ParticleFireworkColour = false;
static bool s_ParticleRandomColour = false;

static float random() { return (rand() / (float)RAND_MAX); }

static float mapValue(float initial, float iS, float iE, float oS, float oE)
{
    float slope = (oE - oS) / (iE - iS);
    return oS + slope * (initial - iS);
}

Effect_Fireworks::Effect_Fireworks(LEDMatrix* matrix) 
    : Effect(EffectEnum::FIREWORKS, matrix)
{
    m_CurrentFireworks = 10;
    createFireworks();
}

Effect_Fireworks::~Effect_Fireworks() {}

void Effect_Fireworks::update()
{
    // TODO: Add random Colour option
    // uint8_t hue = m_PrimaryColour.getHue();

    m_Matrix->fillSolid({ 0, 0, 0 });

    for (uint32_t i = 0; i < m_CurrentFireworks; i++)
    {
        updateFirework(i);
    }
}

void Effect_Fireworks::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("Firework: ");

        ImGui::Text("Random Colours");
        ImGui::Checkbox("##FRandom", &s_FireworkRandomColour);

        ImGui::Text("Count");
        uint32_t min = 1, max = MAX_FIREWORKS;
        ImGui::SliderScalar("##FCount", ImGuiDataType_U32, &m_CurrentFireworks, &min, &max, "%u");

        min = 0;
        max = m_Matrix->getRows() - 1;
        ImGui::Text("Bottom Distance");
        ImGui::SliderScalar("##FDistance", ImGuiDataType_U32, &s_FireworkMinDistance, &min, &max, "%u");

        ImGui::Text("Deceleration");
        ImGui::SliderFloat("##FDeceleration", &s_FireworkDeceleration, 0.01f, 1.0f, "%.3f");

        min = 0;
        max = m_Matrix->getRows();
        ImGui::Text("Trail Intensity");
        ImGui::SliderScalar("##TrailIntensity", ImGuiDataType_U32, &s_TrailIntensity, &min, &max, "%u");

        ImGui::Text("\nParticles");

        ImGui::Text("Firework Colours");
        ImGui::Checkbox("##PFirework", &s_ParticleFireworkColour);

        ImGui::Text("Random Colours");
        ImGui::Checkbox("##PRandom", &s_ParticleRandomColour);

        ImGui::Text("Max Speed");
        ImGui::SliderFloat("##PMaxSpeed", &s_ParticleMaxSpeed, 0.01f, 1.0f, "%.3f");

        ImGui::Text("Decay Speed");
        ImGui::SliderFloat("##PDecay", &s_ParticleDecaySpeed, 0.01f, 3.0f, "%.3f");

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void Effect_Fireworks::createFireworks()
{
    for (uint32_t i = 0; i < MAX_FIREWORKS; i++)
    {
        resetFirework(m_Fireworks[i]);
    }
}

void Effect_Fireworks::updateFirework(int i)
{
    m_Fireworks[i].draw(m_Matrix, m_PrimaryColour);
    m_Fireworks[i].update();
    if (m_Fireworks[i].isDead() || m_Fireworks[i].posY < 0)
    {
        resetFirework(m_Fireworks[i]);
    }
}

void Effect_Fireworks::resetFirework(Firework& firework)
{
    int rX = random() * m_Matrix->getColumns();
    int rY = (m_Matrix->getRows() * 2) + (random() * (m_Matrix->getRows() * 2));
    
    float minY = std::sqrt(2 * s_FireworkDeceleration * (rY - m_Matrix->getRows() + s_FireworkMinDistance));
    float maxY = std::sqrt(2 * s_FireworkDeceleration * (rY - 2));
    firework = Firework(rX, rY, minY, maxY);
}


/// PARTICLES


Particle::Particle(float x, float y)
{
    lifetime = (random() * 255);
    posX = x;
    posY = y;

    pColour = cHSV(random() * 255, 255, 255);

    float t = 2*3.14159*random();
    float u = random()+random();
    float r = u < 1 ? 2 - u : u;
    velX = r * cos(t) * s_ParticleMaxSpeed;
    velY = r * sin(t) * s_ParticleMaxSpeed;
}

void Particle::update()
{
    if (lifetime <= 0) return;
    posX += velX;
    posY += velY;
    lifetime -= s_ParticleDecaySpeed;
}


// FIREWORK


Firework::Firework(float x, float y, float minY, float maxY)
{
    posX = x;
    posY = y;

    velY = -((random() * (maxY - minY)) + minY);
    accY = s_FireworkDeceleration;

    uint8_t hue = random() * 255;
    fColour = cHSV(hue, 255, 255);
}

void Firework::update()
{
    if (!exploded)
    {
        posY += velY;
        velY += accY;

        if (velY >= 0)
            explode();
    }
    else
    {
        for (Particle& p : particles)
        {
            p.update();
        }
    }
}

void Firework::explode()
{
    for (Particle& p : particles)
    {
        p = Particle(posX, posY);
    }
    exploded = true;
}

void Firework::draw(LEDMatrix* matrix, cRGB& primaryColour)
{
    int maxX = matrix->getColumns();
    int maxY = matrix->getRows();
    if (!exploded)
    {
        if (posX >= maxX  || posX < 0) return;

        for (uint32_t i = 0; i < s_TrailIntensity; i++)
        {
            float x = posX;
            float y = posY - (i * velY);
            if (y < 0 || y >= maxY) continue;

            cHSV currentColour;
            if (s_FireworkRandomColour)
                currentColour = fColour;
            else
                currentColour = primaryColour;

            float value = mapValue(i, 0, s_TrailIntensity - 1, 255, 64);
            currentColour.v = value;

            matrix->setLED(x, y, currentColour);
        }
    }
    else
    {
        int index = 0;
        for (Particle& p : particles)
        {
            if (p.posX >= maxX || p.posY >= maxY || p.posX < 0 || p.posY < 0) continue;
            if (p.lifetime <= 0) continue;

            cHSV newColour;
            if (s_ParticleRandomColour)
                newColour = p.pColour;
            else if (s_ParticleFireworkColour)
                newColour = fColour;
            else
                newColour = primaryColour;

            float multiplier = p.lifetime / 255.0f;
            newColour.v *= multiplier;

            matrix->setLED(p.posX, p.posY, newColour);
            index++;
        }
    }
}

bool Firework::isDead()
{
    if (!exploded) return false;

    for (Particle& p : particles)
    {
        if (p.lifetime > 0) return false;
    }
    return true;
}
