#include "../EffectHeaders/Effect_Snake.hpp"

Effect_Snake::Effect_Snake(LEDMatrix* matrix)
    : Effect(EffectEnum::SNAKE, matrix)
{
    // m_Body = SnakeBody(m_Matrix->getColumns() / 2, m_Matrix->getRows() / 2);
    reset();

    m_SnakeCurrentCount = 0;
    m_SnakeMaxCount = 10;
}

Effect_Snake::~Effect_Snake()
{

}

void Effect_Snake::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });

    m_Apple.render(m_PrimaryColour, m_Matrix);
    m_Body.render(m_PrimaryColour, m_Matrix);

    if (m_Body.checkCollision(m_Apple))
    {
        m_Apple.resetPosition(m_Matrix, m_Body);
    }

    checkReset();

    m_SnakeCurrentCount++;
    if (m_SnakeCurrentCount >= m_SnakeMaxCount)
    {
        m_Body.update();

        // Logger::log(LoggerType::LOG_INFO, "%d:%d", m_Body.xDir, m_Body.yDir);

        m_SnakeCurrentCount = 0;
    }
}

void Effect_Snake::render(const char* panelName)
{
    int min, max;
    if (ImGui::Begin(panelName))
    {
        if (ImGui::Button("Up"))
            m_Body.changeDir(SnakeDir::UP);

        if (ImGui::Button("Down"))
            m_Body.changeDir(SnakeDir::DOWN);

        if (ImGui::Button("Left"))
            m_Body.changeDir(SnakeDir::LEFT);

        if (ImGui::Button("Right"))
            m_Body.changeDir(SnakeDir::RIGHT);
        
        ImGui::Text("Snake Update Speed");
        min = 0;
        max = 255;
        int value = max - m_SnakeMaxCount;
        ImGui::SliderScalar("##SnakeUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
        m_SnakeMaxCount = max - value;
    }
    ImGui::End();
}

void Effect_Snake::checkReset()
{
    Pos& head = m_Body.body[0];
    if (m_Body.body.size() >= 5)
    {
        for (int i = 4; i < m_Body.body.size(); i++)
        {
            if (m_Body.body[i].x == head.x && 
                m_Body.body[i].y == head.y)
            {
                reset();
                return;
            }
        }
    }

    if (head.x >= m_Matrix->getColumns() || head.y >= m_Matrix->getRows())
    {
        reset();
        return;
    }
}

void Effect_Snake::reset()
{
    m_Body = SnakeBody(m_Matrix->getColumns() / 2, m_Matrix->getRows() / 2);
    m_Apple.resetPosition(m_Matrix, m_Body);

    m_SnakeCurrentCount = 0;
}

SnakeBody::SnakeBody(uint32_t x, uint32_t y)
{
    body.push_back({ x    , y });
    body.push_back({ x - 1, y });
    body.push_back({ x - 2, y });

    xDir = 1;
    yDir = 0;
}

void SnakeBody::render(cHSV& colour, LEDMatrix* matrix)
{
    for (size_t i = 0; i < body.size(); i++)
    {
        matrix->setLED(body[i].x, body[i].y, colour);
    }
}

void SnakeBody::update()
{
    for (int i = body.size() - 1; i > 0; i--)
    {
        body[i] = body[i - 1];
    }

    Pos& head = body[0];
    head.x += xDir;
    head.y += yDir;
}

void SnakeBody::changeDir(SnakeDir dir)
{
    switch (dir)
    {
    case SnakeDir::UP:
        if (yDir != 1)
        {
            yDir = -1;
            xDir = 0;
        }
        break;
    case SnakeDir::DOWN:
        if (yDir != -1)
        {
            yDir = 1;
            xDir = 0;
        }
        break;
    case SnakeDir::LEFT:
        if (xDir != 1)
        {
            xDir = -1;
            yDir = 0;
        }
        break;
    case SnakeDir::RIGHT:
        if (xDir != -1)
        {
            xDir = 1;
            yDir = 0;
        }
        break;
    }
}

void SnakeBody::increaseSize()
{
    body.push_back(body[body.size() - 1]);
}

bool SnakeBody::checkCollision(Apple apple)
{
    Pos& head = body[0];
    if (apple.pos.x == head.x && apple.pos.y == head.y)
    {
        increaseSize();
        return true;
    }
    return false;
}

void Apple::render(cHSV& colour, LEDMatrix* matrix)
{
    cHSV newColour = colour;
    newColour.h += Colours::HUE_GREEN;
    matrix->setLED(pos.x, pos.y, newColour);
}

void Apple::resetPosition(LEDMatrix* matrix, SnakeBody body)
{
    int x, y;
    while (true)
    {
        x = randomValue() * (matrix->getColumns() - 1);
        y = randomValue() * (matrix->getRows() - 1);

        bool hit = false;
        for (int i = 0; i < body.body.size(); i++)
        {
            if (x == body.body[i].x && y == body.body[i].y)
            {
                hit = true;
                break;
            }
        }

        if (!hit) break;
    }
    pos.x = x;
    pos.y = y;
}
