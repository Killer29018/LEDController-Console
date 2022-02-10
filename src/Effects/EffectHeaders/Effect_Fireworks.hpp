#ifndef EFFECT_EFFECT_FIREWORKS_HPP
#define EFFECT_EFFECT_FIREWORKS_HPP

#include "Effect.hpp"

#include <cmath>

class Particle
{
public:
    float lifetime;
    float posX;
    float posY;
    float velX;
    float velY;
    cHSV pColour;
public:
    Particle() = default;
    Particle(float x, float y);

    void update();
};

class Firework
{
public:
    Particle particles[50];
    cHSV fColour;
    float posX;
    float posY;
    float velY;
    float accY;
    bool exploded = false;
public:
    Firework() = default;
    Firework(float x, float y, float minY, float maxY);

    void update();
    void explode();
    void draw(LEDMatrix* matrix, cRGB& primaryColour);
    bool isDead();
};

class Effect_Fireworks : public Effect
{
private:
    static constexpr uint16_t MAX_FIREWORKS = 50;
    Firework m_Fireworks[MAX_FIREWORKS];
    uint32_t m_CurrentFireworks;
    bool m_RainbowColours;

public:
    Effect_Fireworks(LEDMatrix* matrix);
    ~Effect_Fireworks();

    void update() override;
    void render(const char* panelName) override;
private:
    void createFireworks();
    void updateFirework(int i);
    void resetFirework(Firework& firework);
};

#endif
