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
    cRGB pColour;
public:
    Particle() = default;
    Particle(float x, float y, cRGB colour, float maxSpeed);

    void update(float decayAmount);
};

class Firework
{
public:
    Particle particles[50];
    cRGB colour;
    float posX;
    float posY;
    float velY;
    float accY;
    bool exploded = false;
public:
    Firework() = default;
    Firework(float x, float y, float maxSpeed);

    void update();
    void explode();
    void draw(LEDMatrix* matrix);
    bool isDead();
};

class Effect_Fireworks : public Effect
{
private:
    static constexpr uint16_t MAX_FIREWORKS = 50;
    Firework m_Fireworks[MAX_FIREWORKS];
    uint32_t m_CurrentFireworks;

public:
    Effect_Fireworks();
    ~Effect_Fireworks();

    void update(LEDMatrix* matrix, float dt) override;
    void render(const char* panelName) override;
private:
    void createFireworks(LEDMatrix* matrix);
    void fireworkUpdate(int i, LEDMatrix* matrix);
};

#endif
