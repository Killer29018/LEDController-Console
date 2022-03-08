#include "../EffectHeaders/Effect_Snake.hpp"

#include <fstream>
#include <sstream>

Effect_Snake::Effect_Snake(LEDMatrix* matrix)
    : Effect(EffectEnum::SNAKE, matrix)
{
    reset();

    m_SnakeCurrentCount = 0;
    m_SnakeMaxCount = 10;

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

    generateMaze();
    possibleSolve = true;
}

Effect_Snake::~Effect_Snake()
{
    for (uint32_t i = 0; i < m_Matrix->getRows(); i++)
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
    m_Apple.resetPosition(m_Matrix, m_Body);

    m_SnakeCurrentCount = 0;
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
    printMaze("Maze.txt");
    printMaze2("Maze2.txt");
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

void Effect_Snake::printMaze(const char* output)
{
    std::ofstream file;
    file.open(output);

    for (int i = 0; i < m_MazeW * 2 + 1; i++) file.put('#');
    file.put('\n');

    for (int i = 0; i < m_MazeH; i++)
    {
        file.put('#');
        for (int j = 0; j < m_MazeW; j++)
        {
            if (m_Maze[i][j].visited)
                file.put(' ');
            else
                file.put('0');
            if (m_Maze[i][j].right)
                file.put(' ');
            else
                file.put('#');
        }
        file.put('\n');

        file.put('#');
        for (int j = 0; j < m_MazeW; j++)
        {
            if (m_Maze[i][j].down)
                file.put(' ');
            else
                file.put('#');
            file.put('#');
        }
        file.put('\n');
    }
    file.put('\n');
    file.close();
}

void Effect_Snake::printMaze2(const char* output)
{
    // First need to calculate edges that follow walls. For hamiltonian Cycle
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
    uint32_t x, y;
    while (true)
    {
        x = randomValue() * (matrix->getColumns() - 1);
        y = randomValue() * (matrix->getRows() - 1);

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
