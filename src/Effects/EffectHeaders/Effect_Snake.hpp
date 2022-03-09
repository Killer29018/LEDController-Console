#ifndef EFFECT_EFFECT_SNAKE_HPP
#define EFFECT_EFFECT_SNAKE_HPP

#include "Effect.hpp"

#include <vector>

#include "../../Utils/Helper.hpp"

struct Pos
{
    uint32_t x;
    uint32_t y;

    Pos(uint32_t x, uint32_t y)
        : x(x), y(y) {}
    Pos()
        : x(0), y(0) {}
};

struct Cell
{
    Pos pos;
    bool visited;
    bool neighbour = false;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    Cell() : pos(), visited(false) {}
    Cell(Pos pos) : pos(pos), visited(false) {}
};

enum class Dir
{
    UP      = 1,
    LEFT    = 2,
    DOWN    = 3,
    RIGHT   = 4,
};

struct Apple;

struct SnakeBody
{
    std::vector<Pos> body;

    Dir currentDir = Dir::LEFT;

    int32_t xDir;
    int32_t yDir;

    SnakeBody() = default;
    SnakeBody(uint32_t x, uint32_t y);

    void render(cHSV& colour, LEDMatrix* matrix);
    void update();

    void changeDir(Dir dir);

    void increaseSize();

    bool checkCollision(Apple apple, uint8_t growthAmount = 1); 
};

struct Apple
{
    Pos pos;

    void render(cHSV& colour, LEDMatrix* matrix);
    void resetPosition(uint32_t limitX, uint32_t limitY, SnakeBody body);

    Apple() = default;
};

class Effect_Snake : public Effect
{
private:
    SnakeBody m_Body;
    Apple m_Apple;

    bool m_AI;

    bool m_AnimateHue;
    uint8_t m_HueOffset;
    uint8_t m_DeltaHue;

    uint8_t m_CurrentCount;
    uint8_t m_MaxCount;

    uint8_t m_AppleGrowthAmount;

    uint32_t m_SnakeCurrentCount;
    uint32_t m_SnakeMaxCount;

    Cell** m_Maze;
    std::vector<Cell*> m_NeighbourCells;
    uint32_t m_MazeW, m_MazeH;

    std::vector<Dir> m_Path;

    bool possibleSolve = false;
public:
    Effect_Snake(LEDMatrix* matrix);
    ~Effect_Snake();

    void update() override;
    void render(const char* panelName) override;
private:
    void resetCells();

    void checkReset();
    void reset();

    void generateMaze();
    void addNeighbours(Cell& c);
    std::vector<Cell*> getActiveNeighbours(Cell& c);
    void carvePath(Cell& current, Cell& next);

    void generatePath();
    void setNextPathPos();

    size_t getIndex(Pos pos);
    Pos getMazeIndex(Pos pos);

    void advanceDir(Pos& pos, Dir dir);
    bool getWallRight(Dir dir, Cell& cell);
    bool getWallFront(Dir dir, Cell& cell);
};

#endif
