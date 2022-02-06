#ifndef UTILS_DIAMOND_SQUARE_HPP
#define UTILS_DIAMOND_SQUARE_HPP

#include <cstdint>
#include <random>

class DiamondSquare
{
private:
    static uint8_t** m_Grid;
    static uint32_t m_Size;
    static bool m_Generated;
public:
    static void generate(uint32_t size);
    static uint8_t getValue(uint32_t x, uint32_t y) { return m_Grid[x][y]; }
    static uint32_t getSize() { return m_Size; }
private:
    DiamondSquare() = default;
    // static ~DiamondSquare();

    static void step(uint32_t cellSize);
    static void diamond(uint32_t cellSize);
    static void square(uint32_t cellSize);
    static void printGrid();

    static uint8_t getRandom();
    static int8_t getRandom(float magnitude);
};

#endif
