#include "../EffectHeaders/Effect_Pong.hpp"

#include <cmath>
#include "../../Utils/Helper.hpp"

#include "KRE/System/Keyboard.hpp"
#include "GLFW/glfw3.h"

float Paddle::s_LimitY = 0;
float Paddle::s_MaxSpeed = 0;
int Paddle::s_XOffset = 1;

float Ball::s_LimitX = 0;
float Ball::s_LimitY = 0;
float Ball::s_MaxSpeed = 0;

Effect_Pong::Effect_Pong(LEDMatrix* matrix)
    : Effect(EffectEnum::PONG, matrix)
{
    m_AI = true;

    m_CurrentCount = 0;
    m_MaxCount = 10;

    m_AnimateHue = true;
    m_DeltaHue = 1;
    m_HueOffset = 0;

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

    cHSV colour = m_PrimaryColour;
    if (m_AnimateHue)
    {
        m_CurrentCount++;
        if (m_CurrentCount >= m_MaxCount)
        {
            m_HueOffset += m_DeltaHue;
            m_CurrentCount = 0;
        }
        colour.H += m_HueOffset;
    }

    m_Paddle1.render(colour, m_Matrix);
    m_Paddle2.render(colour, m_Matrix);
    m_Ball.render(colour, m_Matrix);

    m_Paddle1.update();
    m_Paddle2.update();
    m_Ball.update();

    addKeys();
    updatePaddles();

    // m_Ball.checkCollide(m_Paddle1);
    if (checkCollide(m_Paddle1))
        m_Ball.velX *= -1;
    if (checkCollide(m_Paddle2))
        m_Ball.velX *= -1;
}

void Effect_Pong::render(const char* panelName)
{
    int min, max, v;
    float minF, maxF, vF;
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("AI");
        ImGui::Checkbox("##AI", &m_AI);

        if (!m_AI)
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 10.0f);
            ImGui::TextUnformatted("Use wasd for left paddle");
            ImGui::TextUnformatted("Use arrow keys for right paddle\n");

            ImGui::PopTextWrapPos();
        }

        ImGui::Text("Animate Hue");
        ImGui::Checkbox("##AnimateHue", &m_AnimateHue);
        int min, max;

        if (m_AnimateHue)
        {
            ImGui::Text("Delta Hue");
            min = 0;
            max = 255;
            ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &m_DeltaHue, &min, &max, "%u");

            ImGui::Text("Hue Update Speed");
            uint8_t value = max - m_MaxCount;
            ImGui::SliderScalar("##HueUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
            m_MaxCount = max - value;
        }

        if (ImGui::TreeNode("Paddle"))
        {
            min = 1;
            max = m_Matrix->getColumns() / 2.0f - 1;

            ImGui::Text("Width: ");
            v = m_Paddle1.w;
            ImGui::SliderScalar("##PaddleW", ImGuiDataType_U32, &v, &min, &max, "%i");
            m_Paddle1.setWidth(v); m_Paddle2.setWidth(v);

            min = 1;
            max = m_Matrix->getRows();
            v = m_Paddle1.h;
            ImGui::SliderScalar("##PaddleH", ImGuiDataType_U32, &v, &min, &max, "%i");
            m_Paddle1.setHeight(v); m_Paddle2.setHeight(v);

            ImGui::Text("Speed");
            minF = 0.01f;
            maxF = 1.0f;
            vF = Paddle::s_MaxSpeed;
            ImGui::SliderScalar("##PaddleSpeed", ImGuiDataType_Float, &vF, &minF, &maxF, "%.3f");
            Paddle::s_MaxSpeed = vF;

            ImGui::TreePop();
            ImGui::Separator();
        }


        if (ImGui::TreeNode("Ball"))
        {
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

            ImGui::TreePop();
            ImGui::Separator();
        }

        ImGui::PopItemWidth();
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

void Effect_Pong::addKeys()
{
    if (m_AI)
        return;

    if (KRE::Keyboard::getKey(GLFW_KEY_W))
    {
        m_PaddleL.push(PaddleDir::UP);
        m_MovingL = true;
    }
    else if (KRE::Keyboard::getKey(GLFW_KEY_S))
    {
        m_PaddleL.push(PaddleDir::DOWN);
        m_MovingL = true;
    }
    else if (m_MovingL)
    {
        m_PaddleL.push(PaddleDir::NONE);
        m_MovingL = false;
    }

    if (KRE::Keyboard::getKey(GLFW_KEY_UP))
    {
        m_PaddleR.push(PaddleDir::UP);
        m_MovingR = true;
    }
    else if (KRE::Keyboard::getKey(GLFW_KEY_DOWN))
    {
        m_PaddleR.push(PaddleDir::DOWN);
        m_MovingR = true;
    }
    else if (m_MovingR)
    {
        m_PaddleR.push(PaddleDir::NONE);
        m_MovingR = false;
    }
}

void Effect_Pong::updatePaddles()
{
    if (m_AI)
    {
        setTargetX(m_Paddle1);
        setTargetX(m_Paddle2);
    }
    else
    {
        if (m_PaddleL.size() > 0)
        {
            m_Paddle1.move(m_PaddleL.front());
            m_PaddleL.pop();
        }

        if (m_PaddleR.size() > 0)
        {
            m_Paddle2.move(m_PaddleR.front());
            m_PaddleR.pop();
        }
    }
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

void Paddle::move(PaddleDir dir)
{
    switch (dir)
    {
    case PaddleDir::UP:
        velY = -s_MaxSpeed;
        break;
    case PaddleDir::DOWN:
        velY = s_MaxSpeed;
        break;
    case PaddleDir::NONE:
        velY = 0;
        break;
    }
}

Ball::Ball(float x, float y, float r)
    : x(x), y(y), r(r), initialX(x), initialY(y)
{
    velX = mapValue(randomValue(), 0.0f, 1.0f, 0.5f, 1.0f) * s_MaxSpeed;
    velY = mapValue(randomValue(), 0.0f, 1.0f, 0.5f, 1.0f) * s_MaxSpeed;
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
        reset();
    x = std::max(r, std::min(s_LimitX - r - 1, x));

    y += velY;

    if (y <= r || y >= s_LimitY - r - 1)
        velY *= -1;
    y = std::max(r, std::min(s_LimitY - r - 1, y));
}

void Ball::reset()
{
    x = initialX;
    y = initialY;

    velX = mapValue(randomValue(), 0.0f, 1.0f, 0.5f, 1.0f) * s_MaxSpeed;
    velY = mapValue(randomValue(), 0.0f, 1.0f, 0.5f, 1.0f) * s_MaxSpeed;
}
