#include "../EffectHeaders/Effect_Pong.hpp"

#include <cmath>
#include "../../Utils/Helper.hpp"

float Paddle::s_LimitY = 0;
float Paddle::s_MaxSpeed = 0;
int Paddle::s_XOffset = 1;

float Ball::s_LimitX = 0;
float Ball::s_LimitY = 0;
float Ball::s_MaxSpeed = 0;

Effect_Pong::Effect_Pong(LEDMatrix* matrix)
    : Effect(EffectEnum::PONG, matrix)
{
    Paddle::s_MaxSpeed = 0.1f;
    Ball::s_MaxSpeed = 0.2f;

    float w = 1;
    float h = 5;
    float x = 1;
    float y = m_Matrix->getRows() / 2.0 - h / 2.0;
    m_Paddle1 = Paddle(x, y, w, h);

    x = m_Matrix->getColumns() - 1.0 - w;
    m_Paddle2 = Paddle(x, y, w, h);

    m_Ball = Ball(m_Matrix->getColumns() / 2.0f, m_Matrix->getRows() / 2.0f, 2);
}

Effect_Pong::~Effect_Pong() {}

void Effect_Pong::update()
{
    Paddle::s_LimitY = m_Matrix->getRows();
    Ball::s_LimitX = m_Matrix->getColumns();
    Ball::s_LimitY = m_Matrix->getRows();

    m_Matrix->fillSolid({ 0, 0, 0 });

    m_Paddle1.render(m_PrimaryColour, m_Matrix);
    m_Paddle2.render(m_PrimaryColour, m_Matrix);
    m_Ball.render(m_PrimaryColour, m_Matrix);

    m_Paddle1.update();
    m_Paddle2.update();
    m_Ball.update();

    // m_Paddle1.target(m_Ball.y);
    // m_Paddle2.target(m_Ball.y);
    setTargetX(m_Paddle1);
    setTargetX(m_Paddle2);

    // m_Ball.checkCollide(m_Paddle1);
    if (checkCollide(m_Paddle1))
        m_Ball.velX *= -1;
    if (checkCollide(m_Paddle2))
        m_Ball.velX *= -1;
}

void Effect_Pong::render(const char* panelName)
{
    if (ImGui::Begin(panelName))
    {
        ImGui::Text("Paddles:");

        int min = 1;
        int max = m_Matrix->getColumns() / 2.0f - 1;

        ImGui::Text("Width: ");
        int v = m_Paddle1.w;
        ImGui::SliderScalar("##PaddleW", ImGuiDataType_U32, &v, &min, &max, "%i");
        m_Paddle1.setWidth(v); m_Paddle2.setWidth(v);

        min = 1;
        max = m_Matrix->getRows();
        v = m_Paddle1.h;
        ImGui::SliderScalar("##PaddleH", ImGuiDataType_U32, &v, &min, &max, "%i");
        m_Paddle1.setHeight(v); m_Paddle2.setHeight(v);

        ImGui::Text("Speed");
        float minF = 0.01f;
        float maxF = 1.0f;
        float vF = Paddle::s_MaxSpeed;
        ImGui::SliderScalar("##PaddleSpeed", ImGuiDataType_Float, &vF, &minF, &maxF, "%.3f");
        Paddle::s_MaxSpeed = vF;


        ImGui::Text("Ball:");

        min = 1;
        max = 10;
        v = m_Ball.r;
        ImGui::Text("Radius");
        ImGui::SliderScalar("##BallR", ImGuiDataType_U32, &v, &min, &max, "%i");
        m_Ball.setRadius(v);

        ImGui::Text("Speed");
        minF = 0.01f;
        maxF = 1.0f;
        vF = Ball::s_MaxSpeed;
        ImGui::SliderScalar("##BallSpeed", ImGuiDataType_Float, &vF, &minF, &maxF, "%.3f");
        Ball::s_MaxSpeed = vF;
    }
    ImGui::End();
}

void Effect_Pong::setTargetX(Paddle& paddle)
{
    float x = m_Ball.x;
    float y = m_Ball.y;

    float velX = m_Ball.velX;
    float velY = m_Ball.velY;
    float r = m_Ball.r;

    // while (x - r >= paddle.x && 
    //        x - r >= paddle.x + paddle.w && 
    //        x + r <= paddle.x &&
    //        x + r <= paddle.x + paddle.w)
    while (true)
    {
        x += velX;

        if (x <= r || x >= Ball::s_LimitX - r)
            velX *= -1;

        x = std::max(r, std::min(Ball::s_LimitX - r, x));

        y += velY;

        if (y <= r || y >= Ball::s_LimitY - r)
            velY *= -1;

        y = std::max(r, std::min(Ball::s_LimitY - r, y));

        if (paddle.x < m_Matrix->getColumns() / 2)
        {
            if (x - r <= paddle.x || x - r <= paddle.x + paddle.w)
                break;
        }
        else
        {
            if (x + r >= paddle.x || x + r >= paddle.x + paddle.w)
                break;
        }
    }

    paddle.target(y - paddle.h / 2.0f);
}

bool Effect_Pong::checkCollide(Paddle& paddle)
{
    float xMin1 = paddle.x;
    float xMax1 = paddle.x + paddle.w;
    float yMin1 = paddle.y;
    float yMax1 = paddle.y + paddle.h;

    float xMin2 = m_Ball.x - m_Ball.r;
    float xMax2 = m_Ball.x + m_Ball.r;
    float yMin2 = m_Ball.y - m_Ball.r;
    float yMax2 = m_Ball.y + m_Ball.r;

    if (xMin1 < xMax2 &&
        xMax1 > xMin2 &&
        yMin1 < yMax2 &&
        yMax1 > yMin2)
    {
        return true;
    }
    return false;
}


Paddle::Paddle(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h), velY(0) {}

void Paddle::render(cHSV& colour, LEDMatrix* matrix)
{
    // Update X positions relative to position within 
    if (x <= s_XOffset)
    {
        x = s_XOffset;
    }
    else
    {
        x = matrix->getColumns() - s_XOffset - w;
    }

    for (int xOff = 0; xOff < w; xOff++)
    {
        for (int yOff = 0; yOff < h; yOff++)
        {
            matrix->setLED(x + xOff, y + yOff, colour);
        }
    }
}

void Paddle::update()
{
    y += velY;
    y = std::max(0.0f, std::min(s_LimitY - h, y));
}

void Paddle::target(float targetY)
{
    velY = (targetY - y);
    if (velY < -s_MaxSpeed)
        velY = -s_MaxSpeed;
    else if (velY > s_MaxSpeed)
        velY = s_MaxSpeed;
}

Ball::Ball(float x, float y, float r)
    : x(x), y(y), r(r)
{
    velX = s_MaxSpeed;
    velY = s_MaxSpeed;
}

void Ball::render(cHSV colour, LEDMatrix* matrix)
{
    for (int xOff = -r; xOff <= r; xOff++)
    {
        for (int yOff = -r; yOff <= r; yOff++)
        {
            float tempX = x + xOff;
            float tempY = y + yOff;

            float dist = xOff * xOff + yOff * yOff;

            colour.V = mapValue(dist, 0, 2 * (r * r), 255, 64);
            matrix->setLED(tempX, tempY, colour);
        }
    }
}

void Ball::update()
{
    if (velX < 0) velX = -s_MaxSpeed;
    else if (velX > 0) velX = s_MaxSpeed;

    if (velY < 0) velY = -s_MaxSpeed;
    else if (velY > 0) velY = s_MaxSpeed;

    x += velX;

    if (x <= r || x >= s_LimitX - r - 1)
        velX *= -1;
    x = std::max(r, std::min(s_LimitX - r - 1, x));

    y += velY;

    if (y <= r || y >= s_LimitY - r - 1)
        velY *= -1;
    y = std::max(r, std::min(s_LimitY - r - 1, y));
}
