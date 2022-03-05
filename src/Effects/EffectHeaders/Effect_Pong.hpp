#ifndef EFFECT_EFFECT_PONG_HPP
#define EFFECT_EFFECT_PONG_HPP

#include "Effect.hpp"

struct Paddle
{
    float x;
    float y;

    float w;
    float h;

    static float s_LimitY;
    static float s_MaxSpeed;
    static int s_XOffset;

    float velY;

    Paddle() = default;
    Paddle(float x, float y, float w, float h);

    void render(cHSV& colour, LEDMatrix* matrix);

    void update();
    void target(float targetY);

    void setWidth(float width) { w = width; }
    void setHeight(float height) { h = height; }
};

struct Ball
{
    float x;
    float y;
    float r;

    static float s_LimitY;
    static float s_LimitX;
    static float s_MaxSpeed;


    float velX;
    float velY;

    Ball() = default;
    Ball(float x, float y, float r);

    void render(cHSV colour, LEDMatrix* matrix);
    void update();

    void setRadius(float radius) { r = radius; };
};

class Effect_Pong : public Effect
{
private:
    Paddle m_Paddle1;
    Paddle m_Paddle2;

    Ball m_Ball;

    uint8_t m_CurrentCount;
    uint8_t m_MaxCount;

    bool m_AnimateHue;
    uint8_t m_DeltaHue;
    uint8_t m_HueOffset;
public:
    Effect_Pong(LEDMatrix* matrix);
    ~Effect_Pong();

    void update() override;
    void render(const char* panelName) override;
private:
    void setTargetX(Paddle& paddle);

    bool checkCollide(Paddle& paddle);
};

#endif
