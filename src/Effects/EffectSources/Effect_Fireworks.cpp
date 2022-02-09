#include "../EffectHeaders/Effect_Fireworks.hpp"

#include "../../Panels/Logger.hpp"

static float s_ParticleMaxSpeed = 0.05f;
static float s_FireworkMaxSpeed = 0.05f;
static float s_ParticleDecaySpeed = 2.2f;
static float s_FireworkDeceleration = 0.075f;

float random() { return (rand() / (float)RAND_MAX); }

Particle::Particle(float x, float y, cRGB colour, float maxSpeed)
{
    pColour = colour;
    lifetime = (random() * 155) + 100;
    posX = x;
    posY = y;

    float t = 2*3.14159*random();
    float u = random()+random();
    float r = u < 1 ? 2 - u : u;
    velX = r*cos(t) * maxSpeed;
    velY = r*sin(t) * maxSpeed;
}

void Particle::update(float decayAmount)
{
    if (lifetime <= 0) return;
    posX += velX;
    posY += velY;
    lifetime -= decayAmount;
}

Firework::Firework(float x, float y, float maxSpeed)
{
    posX = x;
    posY = y;

    // velY = -((rand() % 1000) * (maxSpeed) / 1000.0f);
    velY = -(random() * 0.8f) - 0.8f;
    accY = s_FireworkDeceleration;

    uint8_t hue = random() * 255;
    colour = cHSV(hue, 255, 255);
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
            p.update(s_ParticleDecaySpeed);
        }
    }
}

void Firework::explode()
{
    for (Particle& p : particles)
    {
        p = Particle(posX, posY, colour, s_ParticleMaxSpeed);
    }
    exploded = true;
}

void Firework::draw(LEDMatrix* matrix)
{
    int maxX = matrix->getColumns();
    int maxY = matrix->getRows();
    if (!exploded)
    {
        if (posX >= maxX || posY >= maxY || posX < 0 || posY < 0) return;

        matrix->setLED(posX, posY, colour);
    }
    else
    {
        int index = 0;
        for (Particle& p : particles)
        {
            if (p.posX >= maxX || p.posY >= maxY || p.posX < 0 || p.posY < 0) continue;
            if (p.lifetime <= 0) continue;

            cRGB newColour = p.pColour;
            float multiplier = p.lifetime / 255.0f;
            float r = (float)newColour.R * multiplier;
            float g = (float)newColour.G * multiplier;
            float b = (float)newColour.B * multiplier;

            newColour = cRGB(r, g, b);
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



Effect_Fireworks::Effect_Fireworks(LEDMatrix* matrix) 
    : Effect(EffectEnum::FIREWORKS, matrix)
{
    m_CurrentFireworks = 10;
        createFireworks();
}

Effect_Fireworks::~Effect_Fireworks() {}

void Effect_Fireworks::update()
{
    uint8_t hue = m_PrimaryColour.getHue();

    m_Matrix->fillSolid({ 0, 0, 0 });

    for (int i = 0; i < m_CurrentFireworks; i++)
    {
        fireworkUpdate(i);
    }
}

void Effect_Fireworks::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("Firework: ");
        ImGui::Text("Count");
        uint32_t min = 1, max = MAX_FIREWORKS;
        ImGui::SliderScalar("##FCount", ImGuiDataType_U32, &m_CurrentFireworks, &min, &max, "%u");
        ImGui::Text("Max speed");
        ImGui::SliderFloat("##FMaxSpeed", &s_FireworkMaxSpeed, 0.01f, 1.0f, "%.3f");
        ImGui::Text("Deceleration");
        ImGui::SliderFloat("##FDeceleration", &s_FireworkDeceleration, 0.01f, 1.0f, "%.3f");

        ImGui::Text("\nParticles");
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
    for (int i = 0; i < MAX_FIREWORKS; i++)
    {
        int rX = random() * m_Matrix->getColumns();
        int rY = m_Matrix->getRows() + random() * 5;
        m_Fireworks[i] = Firework(rX, rY, s_FireworkMaxSpeed);
    }
}

void Effect_Fireworks::fireworkUpdate(int i)
{

    m_Fireworks[i].draw(m_Matrix);
    m_Fireworks[i].update();
    if (m_Fireworks[i].isDead() || m_Fireworks[i].posY < 0)
    {
        int rX = random() * m_Matrix->getColumns();
        int rY = m_Matrix->getRows() + random() * 5;
        m_Fireworks[i] = Firework(rX, rY, s_FireworkMaxSpeed);
    }
}
