#include "../EffectHeaders/Effect_Snake.hpp"

#include <fstream>
#include <sstream>

// #include "KRE/System/Keyboard.hpp"
// #include "GLFW/glfw3.h"

Effect_Snake::Effect_Snake(LEDMatrix* matrix)
    : Effect(EffectEnum::SNAKE, matrix)
{
    m_AI = true;
    m_AnimateHue = true;
    m_SnakeCurrentCount = 0;
    m_SnakeMaxCount = 10;

    m_MaxCount = 10;
    m_CurrentCount = 0;

    m_HueOffset = 0;
    m_DeltaHue = 1;

    m_AppleGrowthAmount = 1;

    if ((m_Matrix->getRows() & 1) == 1 && (m_Matrix->getColumns() & 1) == 1)
    {
        Logger::log(LoggerType::LOG_ERROR, "Unsupported size. One dimension must be even");
        possibleSolve = false;
    }

    m_MazeW = m_Matrix->getColumns() / 2;
    m_MazeH = m_Matrix->getRows() / 2;
    m_Maze = new Cell*[m_MazeH];

    for (int32_t i = 0; i < m_MazeH; i++)
    {
        m_Maze[i] = new Cell[m_MazeW];

        for (int32_t j = 0; j < m_MazeW; j++)
        {
            m_Maze[i][j] = Cell({j, i});
        }
    }

    reset();
    possibleSolve = true;
}

Effect_Snake::~Effect_Snake()
{
    for (int32_t i = 0; i < m_MazeH; i++)
    {
        delete[] m_Maze[i];
    }
    delete[] m_Maze;
}

void Effect_Snake::update()
{
    m_Matrix->fillSolid({ 0, 0, 0 });

    if (!possibleSolve)
        return;

    cHSV currentColour = m_PrimaryColour;
    if (m_AnimateHue)
        currentColour.H += m_HueOffset;

    m_Apple.render(currentColour, m_Matrix);
    m_Body.render(currentColour, m_Matrix);

    if (m_Body.checkCollision(m_Apple, m_AppleGrowthAmount))
    {
        m_Apple.resetPosition(m_MazeW * 2, m_MazeH * 2, m_Body);
    }

    checkReset();

    // addKeys();

    m_SnakeCurrentCount++;
    if (m_SnakeCurrentCount >= m_SnakeMaxCount)
    {
        m_Body.changeDir(getNextSnakeDirection());

        m_Body.update();

        m_SnakeCurrentCount = 0;
    }

    if (m_AnimateHue) m_CurrentCount++;
    if (m_CurrentCount > m_MaxCount)
    {
        m_HueOffset += m_DeltaHue;
        m_CurrentCount = 0;
    }
}

// void Effect_Snake::render(const char* panelName)
// {
//     int min, max;
//     if (ImGui::Begin(panelName))
//     {
//         ImGui::PushItemWidth(-1);

//         ImGui::Text("AI");
//         ImGui::Checkbox("##AI", &m_AI);

//         if (!m_AI)
//         {
//             ImGui::PushTextWrapPos(ImGui::GetFontSize() * 10.0f);
//             ImGui::TextUnformatted("Use either arrow keys or wasd for moving the snake\n");

//             ImGui::PopTextWrapPos();
//         }

//         ImGui::Text("Animate Hue");
//         ImGui::Checkbox("##AnimateHue", &m_AnimateHue);
//         if (m_AnimateHue)
//         {
//             ImGui::Text("Delta Hue");
//             min = 0;
//             max = 255;
//             ImGui::SliderScalar("##DeltaHue", ImGuiDataType_U8, &m_DeltaHue, &min, &max, "%u");

//             ImGui::Text("Hue Update Speed");
//             uint8_t value = max - m_MaxCount;
//             ImGui::SliderScalar("##HueUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
//             m_MaxCount = max - value;
//         }

//         ImGui::Text("Apple Growth Amount");
//         min = 1;
//         max = 10;
//         ImGui::SliderScalar("##Apple", ImGuiDataType_U8, &m_AppleGrowthAmount, &min, &max, "%u");
        
//         ImGui::Text("Snake Update Speed");
//         min = 0;
//         max = 255;
//         int value = max - m_SnakeMaxCount;
//         ImGui::SliderScalar("##SnakeUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
//         m_SnakeMaxCount = max - value;

//         ImGui::PopItemWidth();
//     }
//     ImGui::End();
// }

void Effect_Snake::resetCells()
{
    for (int32_t i = 0; i < m_MazeH; i++)
    {
        for (int32_t j = 0; j < m_MazeW; j++)
        {
            m_Maze[i][j] = Cell({j, i});
        }
    }
}

void Effect_Snake::checkReset()
{
    Pos& head = m_Body.body[0];
    if (m_Body.body.size() >= 5 && !(m_Body.body[1].x == head.x && m_Body.body[1].y == head.y))
    {
        for (size_t i = 4; i < m_Body.body.size(); i++)
        {
            if (m_Body.body[i].x == head.x && 
                m_Body.body[i].y == head.y)
            {
                reset();
                return;
            }
        }
    }

    if ((uint32_t)head.x >= m_Matrix->getColumns() || (uint32_t)head.y >= m_Matrix->getRows())
    {
        reset();
        return;
    }
}

void Effect_Snake::reset()
{
    m_Body = SnakeBody(m_Matrix->getColumns() / 2, m_Matrix->getRows() / 2);
    m_Apple.resetPosition(m_MazeW * 2, m_MazeH * 2, m_Body);

    m_SnakeCurrentCount = 0;

    resetCells();

    generateMaze();
}

// void Effect_Snake::addKeys()
// {
//     if (KRE::Keyboard::getKey(GLFW_KEY_W) || KRE::Keyboard::getKey(GLFW_KEY_UP))
//     {
//         if (!m_ProcessedKeys[SnakeDir::UP])
//         {
//             m_PressedKeys.push(SnakeDir::UP);
//             m_ProcessedKeys[SnakeDir::UP] = true;
//         }
//     }
//     else
//     {
//         m_ProcessedKeys[SnakeDir::UP] = false;
//     }

//     if (KRE::Keyboard::getKey(GLFW_KEY_A) || KRE::Keyboard::getKey(GLFW_KEY_LEFT))
//     {
//         if (!m_ProcessedKeys[SnakeDir::LEFT])
//         {
//             m_PressedKeys.push(SnakeDir::LEFT);
//             m_ProcessedKeys[SnakeDir::LEFT] = true;
//         }
//     }
//     else
//     {
//         m_ProcessedKeys[SnakeDir::LEFT] = false;
//     }

//     if (KRE::Keyboard::getKey(GLFW_KEY_S) || KRE::Keyboard::getKey(GLFW_KEY_DOWN))
//     {
//         if (!m_ProcessedKeys[SnakeDir::DOWN])
//         {
//             m_PressedKeys.push(SnakeDir::DOWN);
//             m_ProcessedKeys[SnakeDir::DOWN] = true;
//         }
//     }
//     else
//     {
//         m_ProcessedKeys[SnakeDir::DOWN] = false;
//     }

//     if (KRE::Keyboard::getKey(GLFW_KEY_D) || KRE::Keyboard::getKey(GLFW_KEY_RIGHT))
//     {
//         if (!m_ProcessedKeys[SnakeDir::RIGHT])
//         {
//             m_PressedKeys.push(SnakeDir::RIGHT);
//             m_ProcessedKeys[SnakeDir::RIGHT] = true;
//         }
//     }
//     else
//     {
//         m_ProcessedKeys[SnakeDir::RIGHT] = false;
//     }
// }

SnakeDir Effect_Snake::getNextSnakeDirection()
{
    if (!m_AI)
    {
        if (m_PressedKeys.size() > 0)
        {
            SnakeDir dir = m_PressedKeys.front();
            m_PressedKeys.pop();
            return dir;
        }

        return SnakeDir::NONE;
    }
    else
    {
        /*
         * Modified From
         * https://github.com/johnflux/snake_game/blob/master/snake.cpp
         * aigetNewSnakeDirection()
        */

        const int totalSize = m_MazeW * m_MazeH * 4;

        Pos pos = m_Body.body[0];
        int x = pos.x;
        int y = pos.y;

        int pathNumber = getPathIndex(pos);

        int distanceToFood = pathDistance(pathNumber, getPathIndex(m_Apple.pos));
        int distanceToTail = pathDistance(pathNumber, getPathIndex(m_Body.body[m_Body.body.size() - 1]));

        int cuttingAmountAvail = distanceToTail - m_Body.growthAmount - 3;
        int emptySquares = totalSize - m_Body.body.size() - m_AppleGrowthAmount;

        if (emptySquares < totalSize / 2)
            cuttingAmountAvail = 0;
        else if (distanceToFood < distanceToTail)
        {
            cuttingAmountAvail -= m_AppleGrowthAmount;
            if ((distanceToTail - distanceToFood) * 4 > emptySquares)
            {
                cuttingAmountAvail -= 10;
            }
        }

        int cuttingDesired = distanceToFood;
        if (cuttingDesired < cuttingAmountAvail)
            cuttingAmountAvail = cuttingDesired;

        if (cuttingAmountAvail < 0)
            cuttingAmountAvail = 0;

        bool canGoRight = !checkCollision({ x + 1, y });
        bool canGoLeft = !checkCollision({ x - 1, y });
        bool canGoDown = !checkCollision({ x, y + 1 });
        bool canGoUp = !checkCollision({ x, y - 1 });

        SnakeDir bestDir;
        int bestDist = -1;

        if (canGoRight)
        {
            int dist = pathDistance(pathNumber, getPathIndex({ x + 1, y }));
            if (dist <= cuttingAmountAvail && dist > bestDist)
            {
                bestDir = SnakeDir::RIGHT;
                bestDist = dist;
            }
        }
        if (canGoLeft)
        {
            int dist = pathDistance(pathNumber, getPathIndex({ x - 1, y }));
            if (dist <= cuttingAmountAvail && dist > bestDist)
            {
                bestDir = SnakeDir::LEFT;
                bestDist = dist;
            }
        }
        if (canGoDown)
        {
            int dist = pathDistance(pathNumber, getPathIndex({ x, y + 1 }));
            if (dist <= cuttingAmountAvail && dist > bestDist)
            {
                bestDir = SnakeDir::DOWN;
                bestDist = dist;
            }
        }
        if (canGoUp)
        {
            int dist = pathDistance(pathNumber, getPathIndex({ x, y - 1 }));
            if (dist <= cuttingAmountAvail && dist > bestDist)
            {
                bestDir = SnakeDir::UP;
                bestDist = dist;
            }
        }

        if (bestDist >= 0)
            return bestDir;

        if (canGoUp)
            return SnakeDir::UP;
        if (canGoLeft)
            return SnakeDir::LEFT;
        if (canGoDown)
            return SnakeDir::DOWN;
        if (canGoUp)
            return SnakeDir::UP;

        return SnakeDir::NONE;
    }
}

int Effect_Snake::pathDistance(int a, int b)
{
    if (a < b)
        return (b - a - 1);
    return (b - a - 1) + (m_MazeW * m_MazeH * 4);
}

bool Effect_Snake::checkCollision(Pos pos)
{
    if (pos.x < 0 || pos.x >= (m_MazeW * 2) || pos.y < 0 || pos.y >= (m_MazeH * 2))
        return true;

    return m_Body.checkCollisionBody(pos);
}

void Effect_Snake::generateMaze()
{
    uint32_t x = randomValue() * m_MazeW;
    uint32_t y = randomValue() * m_MazeH;

    m_Maze[y][x].visited = true;
    addNeighbours(m_Maze[y][x]);

    while (m_NeighbourCells.size() != 0)
    {
        size_t index = randomValue() * (m_NeighbourCells.size() - 1);

        Cell* currentCell = m_NeighbourCells[index];
        m_NeighbourCells.erase(m_NeighbourCells.begin() + index);

        assert(!currentCell->visited && "Not possible");

        currentCell->visited = true;

        addNeighbours(*currentCell);

        std::vector<Cell*> neighbours = getActiveNeighbours(*currentCell);

        assert(neighbours.size() != 0 && "Not possible");

        size_t randomN = randomValue() * (neighbours.size() - 1);
        carvePath(*currentCell, *neighbours[randomN]);
    }

    generatePath();
}

void Effect_Snake::addNeighbours(Cell& c)
{
    Cell* n;
    if (c.pos.x > 0)
    {
        n = &m_Maze[c.pos.y][c.pos.x - 1];
        if (!n->visited && !n->neighbour)
        {
            n->neighbour = true;
            m_NeighbourCells.push_back(n);
        }
    }

    if (c.pos.x < (m_MazeW - 1))
    {
        n = &m_Maze[c.pos.y][c.pos.x + 1];
        if (!n->visited && !n->neighbour)
        {
            n->neighbour = true;
            m_NeighbourCells.push_back(n);
        }
    }

    if (c.pos.y > 0)
    {
        n = &m_Maze[c.pos.y - 1][c.pos.x];
        if (!n->visited && !n->neighbour)
        {
            n->neighbour = true;
            m_NeighbourCells.push_back(n);
        }
    }

    if (c.pos.y < (m_MazeH - 1))
    {
        n = &m_Maze[c.pos.y + 1][c.pos.x];
        if (!n->visited && !n->neighbour)
        {
            n->neighbour = true;
            m_NeighbourCells.push_back(n);
        }
    }
}

std::vector<Cell*> Effect_Snake::getActiveNeighbours(Cell& c)
{
    std::vector<Cell*> neighbours;
    if (c.pos.x > 0 && m_Maze[c.pos.y][c.pos.x - 1].visited)
        neighbours.push_back(&m_Maze[c.pos.y][c.pos.x - 1]);

    if (c.pos.x < (m_MazeW - 1) && m_Maze[c.pos.y][c.pos.x + 1].visited)
        neighbours.push_back(&m_Maze[c.pos.y][c.pos.x + 1]);

    if (c.pos.y > 0 && m_Maze[c.pos.y - 1][c.pos.x].visited)
        neighbours.push_back(&m_Maze[c.pos.y - 1][c.pos.x]);

    if (c.pos.y < (m_MazeH - 1) && m_Maze[c.pos.y + 1][c.pos.x].visited)
        neighbours.push_back(&m_Maze[c.pos.y + 1][c.pos.x]);

    return neighbours;
}

void Effect_Snake::carvePath(Cell& current, Cell& next)
{
    if (current.pos.x == next.pos.x)
    {
        if (next.pos.y > current.pos.y) // Down
        {
            current.down = true;
            next.up = true;
        }
        else // Up
        {
            current.up = true;
            next.down = true;
        }
    }
    else
    {
        if (next.pos.x > current.pos.x) // Right
        {
            current.right = true;
            next.left = true;
        }
        else // Left
        {
            current.left = true;
            next.right = true;
        }
    }
}

void Effect_Snake::generatePath()
{
    Pos pos = Pos(0, 0);
    Pos initialPos = pos;
    Pos mazePos;
    SnakeDir dir = SnakeDir::LEFT;

    m_Path.resize(m_MazeW * m_MazeH * 4);
    m_PathValues.resize(m_MazeW * m_MazeH * 4);

    // for (int i = 0; i < m_Path.size(); i++)
    int currentIndex = 0;
    do
    {
        mazePos = getMazeIndex(pos);
        size_t index = getIndex(pos);

        Cell& cell = m_Maze[mazePos.y][mazePos.x];
        bool right = getWallRight(dir, cell);
        bool front = getWallFront(dir, cell);

        if (right) // Turn Right
        {
            switch(dir)
            {
                case SnakeDir::LEFT:
                    if ((pos.x + 1) % 2 == 0)
                        dir = SnakeDir::UP;
                    break;
                case SnakeDir::DOWN:
                    if ((pos.y + 1) % 2 == 1)
                        dir = SnakeDir::LEFT;
                    break;
                case SnakeDir::RIGHT:
                    if ((pos.x + 1) % 2 == 1)
                        dir = SnakeDir::DOWN;
                    break;
                case SnakeDir::UP:
                    if ((pos.y + 1) % 2 == 0)
                        dir = SnakeDir::RIGHT;
                    break;
                case SnakeDir::NONE:
                    break;
            }
        }
        else if (!front) // Turn Left
        {
            switch(dir)
            {
                case SnakeDir::LEFT:
                    if ((pos.x + 1) % 2 == 1)
                        dir = SnakeDir::DOWN;
                    break;
                case SnakeDir::DOWN:
                    if ((pos.y + 1) % 2 == 0)
                        dir = SnakeDir::RIGHT;
                    break;
                case SnakeDir::RIGHT:
                    if ((pos.x + 1) % 2 == 0)
                        dir = SnakeDir::UP;
                    break;
                case SnakeDir::UP:
                    if ((pos.y + 1) % 2 == 1)
                        dir = SnakeDir::LEFT;
                    break;
                case SnakeDir::NONE:
                    break;
            }
        }

        advanceDir(pos, dir);

        if (pos.x >= m_MazeW * 2 || pos.y >= m_MazeH * 2)
            assert(false && "Not possible");

        m_Path[index] = dir;
        m_PathValues[index] = currentIndex++;
    }
    while (pos.x != initialPos.x || pos.y != initialPos.y);

    assert(pos.x == initialPos.x && pos.y == initialPos.y && "Not Possible");
}

void Effect_Snake::setNextPathPos()
{

}

size_t Effect_Snake::getIndex(Pos pos)
{
    if (pos.x > (m_MazeW * 2) || pos.y > (m_MazeH * 2))
        return (m_MazeW * m_MazeH * 10);
        // assert(false && "Not possible");
    return (pos.x + (m_MazeW * 2 * pos.y));
}

size_t Effect_Snake::getPathIndex(Pos pos)
{
    if (pos.x > (m_MazeW * 2) || pos.y > (m_MazeH * 2))
        return (m_MazeW * m_MazeH * 10);

    return m_PathValues[getIndex(pos)];
}

Pos Effect_Snake::getMazeIndex(Pos pos)
{
    Pos returnPos;
    returnPos.x = pos.x / 2;
    returnPos.y = pos.y / 2;
    return returnPos;
}

void Effect_Snake::advanceDir(Pos& pos, SnakeDir dir)
{
    switch (dir)
    {
    case SnakeDir::UP:
        pos.y -= 1; break;
    case SnakeDir::LEFT:
        pos.x -= 1; break;
    case SnakeDir::DOWN:
        pos.y += 1; break;
    case SnakeDir::RIGHT:
        pos.x += 1; break;
    case SnakeDir::NONE:
        break;
    }
}

bool Effect_Snake::getWallRight(SnakeDir dir, Cell& cell)
{
    switch (dir)
    {
    case SnakeDir::UP:
        return cell.right;
    case SnakeDir::LEFT:
        return cell.up;
    case SnakeDir::DOWN:
        return cell.left;
    case SnakeDir::RIGHT:
        return cell.down;

    default:
        return false;
    }
}

bool Effect_Snake::getWallFront(SnakeDir dir, Cell& cell)
{
    switch (dir)
    {
    case SnakeDir::UP:
        return cell.up;
    case SnakeDir::LEFT:
        return cell.left;
    case SnakeDir::DOWN:
        return cell.down;
    case SnakeDir::RIGHT:
        return cell.right;

    default:
        return false;
    }
}

SnakeBody::SnakeBody(int32_t x, int32_t y)
{
    body.push_back({ x    , y });

    increaseSize(); increaseSize();

    xDir = 1;
    yDir = 0;

    changeDir(currentDir);
}

void SnakeBody::render(cHSV& colour, LEDMatrix* matrix)
{
    cHSV headColour = colour;
    headColour.h += 5;
    matrix->setLED(body[0].x, body[0].y, headColour);
    for (size_t i = 1; i < body.size(); i++)
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

    if (growthAmount > 0)
        growthAmount--;
}

void SnakeBody::changeDir(SnakeDir dir)
{
    currentDir = dir;
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
    case SnakeDir::NONE:
        break;
    }
}

void SnakeBody::increaseSize()
{
    body.push_back(body[body.size() - 1]);
}

bool SnakeBody::checkCollision(Apple apple, uint8_t growthAmount)
{
    Pos& head = body[0];
    if (apple.pos.x == head.x && apple.pos.y == head.y)
    {
        for (int i = 0; i < growthAmount; i++) 
        {
            increaseSize();
            this->growthAmount++;
        }

        return true;
    }
    return false;
}

bool SnakeBody::checkCollisionBody(Pos pos)
{
    for (size_t i = 0; i < body.size(); i++)
    {
        if (pos.x == body[i].x && pos.y == body[i].y)
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

void Apple::resetPosition(uint32_t limitX, uint32_t limitY, SnakeBody body)
{
    int32_t x, y;
    while (true)
    {
        x = randomValue() * limitX;
        y = randomValue() * limitY;

        bool hit = false;
        for (size_t i = 0; i < body.body.size(); i++)
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
