#ifndef EFFECT_EFFECT_PONG_HPP
#define EFFECT_EFFECT_PONG_HPP

#include "Effect.hpp"

#include <queue>
#include <unordered_map>

enum class PaddleDir
{
    NONE,
    UP,
    DOWN,
};

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

    void move(PaddleDir dir);

    void setWidth(float width) { w = width; }
    void setHeight(float height) { h = height; }
};

struct Ball
{
    float x;
    float y;
    float r;

    float initialX, initialY;

    static float s_LimitY;
    static float s_LimitX;
    static float s_MaxSpeed;

    float velX;
    float velY;

    Ball() = default;
    Ball(float x, float y, float r);

    void render(cHSV colour, LEDMatrix* matrix);
    void update();
    void reset();

    void setRadius(float radius) { r = radius; };
};

class Effect_Pong : public Effect
{
private:
    Paddle m_Paddle1;
    Paddle m_Paddle2;

    Ball m_Ball;

    bool m_AI;

    uint8_t m_CurrentCount;
    uint8_t m_MaxCount;

    bool m_AnimateHue;
    uint8_t m_DeltaHue;
    uint8_t m_HueOffset;

    std::queue<PaddleDir> m_PaddleL;
    std::queue<PaddleDir> m_PaddleR;

    bool m_MovingL = false;
    bool m_MovingR = false;
public:
    Effect_Pong(LEDMatrix* matrix);
    ~Effect_Pong();

    void update() override;
    void render(const char* panelName) override;
private:
    void setTargetX(Paddle& paddle);

    void addKeys();
    void updatePaddles();

    bool checkCollide(Paddle& paddle);
};

#endif
