#include "../EffectHeaders/Effect_Snake.hpp"

#include <fstream>
#include <sstream>

Effect_Snake::Effect_Snake(LEDMatrix* matrix)
    : Effect(EffectEnum::SNAKE, matrix)
{
    m_AI = true;
    m_AnimateHue = false;
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

    for (uint32_t i = 0; i < m_MazeH; i++)
    {
        m_Maze[i] = new Cell[m_MazeW];

        for (uint32_t j = 0; j < m_MazeW; j++)
        {
            m_Maze[i][j] = Cell({j, i});
        }
    }

    reset();
    possibleSolve = true;
}

Effect_Snake::~Effect_Snake()
{
    for (uint32_t i = 0; i < m_MazeH; i++)
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

    m_SnakeCurrentCount++;
    if (m_SnakeCurrentCount >= m_SnakeMaxCount)
    {
        if (m_AI)
        {
            Dir newDir = m_Path[getIndex(m_Body.body[0])];

            m_Body.changeDir(newDir);
        }

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

void Effect_Snake::render(const char* panelName)
{
    int min, max;
    if (ImGui::Begin(panelName))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("AI");
        ImGui::Checkbox("##AI", &m_AI);
        
        ImGui::Text("Apple Growth Amount");
        min = 1;
        max = 10;
        ImGui::SliderScalar("##Apple", ImGuiDataType_U8, &m_AppleGrowthAmount, &min, &max, "%u");

        ImGui::Text("Animate Hue");
        ImGui::Checkbox("##AnimateHue", &m_AnimateHue);
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
        
        ImGui::Text("Snake Update Speed");
        min = 0;
        max = 255;
        int value = max - m_SnakeMaxCount;
        ImGui::SliderScalar("##SnakeUpdate", ImGuiDataType_U8, &value, &min, &max, "%u");
        m_SnakeMaxCount = max - value;

        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void Effect_Snake::resetCells()
{
    for (uint32_t i = 0; i < m_MazeH; i++)
    {
        for (uint32_t j = 0; j < m_MazeW; j++)
        {
            m_Maze[i][j] = Cell({j, i});
        }
    }
}

void Effect_Snake::checkReset()
{
    Pos& head = m_Body.body[0];
    if (m_Body.body.size() >= 5)
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

    if (head.x >= m_Matrix->getColumns() || head.y >= m_Matrix->getRows())
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
    Dir dir = Dir::LEFT;

    m_Path.resize(m_MazeW * m_MazeH * 4);

    // for (int i = 0; i < m_Path.size(); i++)
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
                case Dir::LEFT:
                    if ((pos.x + 1) % 2 == 0)
                        dir = Dir::UP;
                    break;
                case Dir::DOWN:
                    if ((pos.y + 1) % 2 == 1)
                        dir = Dir::LEFT;
                    break;
                case Dir::RIGHT:
                    if ((pos.x + 1) % 2 == 1)
                        dir = Dir::DOWN;
                    break;
                case Dir::UP:
                    if ((pos.y + 1) % 2 == 0)
                        dir = Dir::RIGHT;
                    break;
            }
        }
        else if (!front) // Turn Left
        {
            switch(dir)
            {
                case Dir::LEFT:
                    if ((pos.x + 1) % 2 == 1)
                        dir = Dir::DOWN;
                    break;
                case Dir::DOWN:
                    if ((pos.y + 1) % 2 == 0)
                        dir = Dir::RIGHT;
                    break;
                case Dir::RIGHT:
                    if ((pos.x + 1) % 2 == 0)
                        dir = Dir::UP;
                    break;
                case Dir::UP:
                    if ((pos.y + 1) % 2 == 1)
                        dir = Dir::LEFT;
                    break;
            }
        }

        advanceDir(pos, dir);

        if (pos.x >= m_MazeW * 2 || pos.y >= m_MazeH * 2)
            assert(false && "Not possible");

        m_Path[index] = dir;
    }
    while (pos.x != initialPos.x || pos.y != initialPos.y);

    assert(pos.x == initialPos.x && pos.y == initialPos.y && "Not Possible");
}

void Effect_Snake::setNextPathPos()
{

}

size_t Effect_Snake::getIndex(Pos pos)
{
    return (pos.x + (m_Matrix->getColumns() * pos.y));
}

Pos Effect_Snake::getMazeIndex(Pos pos)
{
    Pos returnPos;
    returnPos.x = pos.x / 2;
    returnPos.y = pos.y / 2;
    return returnPos;
}

void Effect_Snake::advanceDir(Pos& pos, Dir dir)
{
    switch (dir)
    {
    case Dir::UP:
        pos.y -= 1; break;
    case Dir::LEFT:
        pos.x -= 1; break;
    case Dir::DOWN:
        pos.y += 1; break;
    case Dir::RIGHT:
        pos.x += 1; break;
    }
}

bool Effect_Snake::getWallRight(Dir dir, Cell& cell)
{
    switch (dir)
    {
    case Dir::UP:
        return cell.right;
    case Dir::LEFT:
        return cell.up;
    case Dir::DOWN:
        return cell.left;
    case Dir::RIGHT:
        return cell.down;
    }

    assert(false && "Not possible");
}

bool Effect_Snake::getWallFront(Dir dir, Cell& cell)
{
    switch (dir)
    {
    case Dir::UP:
        return cell.up;
    case Dir::LEFT:
        return cell.left;
    case Dir::DOWN:
        return cell.down;
    case Dir::RIGHT:
        return cell.right;
    }

    assert(false && "Not possible");
}

SnakeBody::SnakeBody(uint32_t x, uint32_t y)
{
    body.push_back({ x    , y });

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
}

void SnakeBody::changeDir(Dir dir)
{
    currentDir = dir;
    switch (dir)
    {
    case Dir::UP:
        if (yDir != 1)
        {
            yDir = -1;
            xDir = 0;
        }
        break;
    case Dir::DOWN:
        if (yDir != -1)
        {
            yDir = 1;
            xDir = 0;
        }
        break;
    case Dir::LEFT:
        if (xDir != 1)
        {
            xDir = -1;
            yDir = 0;
        }
        break;
    case Dir::RIGHT:
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

bool SnakeBody::checkCollision(Apple apple, uint8_t growthAmount)
{
    Pos& head = body[0];
    if (apple.pos.x == head.x && apple.pos.y == head.y)
    {
        for (int i = 0; i < growthAmount; i++) increaseSize();

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
    uint32_t x, y;
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
