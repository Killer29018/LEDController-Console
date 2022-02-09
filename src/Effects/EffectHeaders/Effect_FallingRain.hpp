#ifndef EFFECT_EFFECT_FALLING_RAIN_HPP
#define EFFECT_EFFECT_FALLING_RAIN_HPP

#include "Effect.hpp"

#include <cmath>

class Raindrop
{
public:
    float posX;
    float posY;

    float velY;
    float accY;

    cHSV colour;
private:
    bool* m_RandomColour;
public:
    Raindrop() = default;
    Raindrop(float x, float y, bool* useRandom);

    void update();
    void draw(LEDMatrix* matrix, cRGB targetColour);
};

class Effect_FallingRain : public Effect
{
private:
    static const uint32_t MAX_RAINDROPS = 100;
    uint32_t m_CurrentRaindrops;
    Raindrop m_Raindrops[MAX_RAINDROPS];
    bool m_RainbowColours;
public:
    Effect_FallingRain(LEDMatrix* matrix);
    ~Effect_FallingRain();

    void update();
    void render(const char* panelName);
private:
    void createRaindrops();
    void resetRaindrop(Raindrop& raindrop);
    void updateRaindrops();
    void renderRaindrops();
};

#endif
