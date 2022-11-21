#ifndef EFFECT_EFFECT_SNAKE_HPP
#define EFFECT_EFFECT_SNAKE_HPP

#include "Effect.hpp"

#include <vector>
#include <queue>
#include <unordered_map>

#include "../../Utils/Helper.hpp"

struct Pos
{
    int32_t x;
    int32_t y;

    Pos(int32_t x, int32_t y)
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

enum class SnakeDir 
{
    NONE    = 0,
    UP      = 1,
    LEFT    = 2,
    DOWN    = 3,
    RIGHT   = 4,
};

struct Apple;

struct SnakeBody
{
    std::vector<Pos> body;
    uint32_t growthAmount = 0;

    SnakeDir currentDir = SnakeDir::LEFT;

    int32_t xDir;
    int32_t yDir;

    SnakeBody() = default;
    SnakeBody(int32_t x, int32_t y);

    void render(cHSV& colour, LEDMatrix* matrix);
    void update();

    void changeDir(SnakeDir dir);

    void increaseSize();

    bool checkCollision(Apple apple, uint8_t growthAmount = 1); 
    bool checkCollisionBody(Pos pos);
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
    int32_t m_MazeW, m_MazeH;

    std::vector<SnakeDir> m_Path;
    std::vector<int> m_PathValues;

    bool possibleSolve = false;

    std::queue<SnakeDir> m_PressedKeys;
    std::unordered_map<SnakeDir, bool> m_ProcessedKeys;
public:
    Effect_Snake(LEDMatrix* matrix);
    ~Effect_Snake();

    void update() override;
    // void render(const char* panelName) override;
private:
    void resetCells();

    void checkReset();
    void reset();

    // void addKeys();

    SnakeDir getNextSnakeDirection();

    int pathDistance(int a, int b);
    bool checkCollision(Pos pos);

    void generateMaze();
    void addNeighbours(Cell& c);
    std::vector<Cell*> getActiveNeighbours(Cell& c);
    void carvePath(Cell& current, Cell& next);

    void generatePath();
    void setNextPathPos();

    size_t getIndex(Pos pos);
    size_t getPathIndex(Pos pos);
    Pos getMazeIndex(Pos pos);

    void advanceDir(Pos& pos, SnakeDir dir);
    bool getWallRight(SnakeDir dir, Cell& cell);
    bool getWallFront(SnakeDir dir, Cell& cell);
};

#endif
