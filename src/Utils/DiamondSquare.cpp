#include "DiamondSquare.hpp"
#include <cmath>
#include <iostream>

uint8_t** DiamondSquare::m_Grid = nullptr;
uint32_t DiamondSquare::m_Size = 0;
bool DiamondSquare::m_Generated = false;

// DiamondSquare::~DiamondSquare()
// {
//     for (int i = 0; i < m_Size; i++)
//         delete[] m_Grid[i];
//     delete[] m_Grid;
// }

void DiamondSquare::generate(uint32_t size)
{
    if (m_Generated)
    {
        for (int i = 0; i < m_Size; i++)
            delete[] m_Grid[i];
        delete[] m_Grid;
    }


    int n = std::ceil(std::log2(size - 1));
    m_Size = std::pow(2, n) + 1;

    m_Generated = true;

    m_Grid = new uint8_t*[m_Size];
    for (int x = 0; x < m_Size; x++)
    {
        m_Grid[x] = new uint8_t[m_Size];
        for (int y = 0; y < m_Size; y++)
        {
            m_Grid[x][y] = 0;
        }
    }

    m_Grid[0         ][0         ] = getRandom();
    m_Grid[0         ][m_Size - 1] = getRandom();
    m_Grid[m_Size - 1][0         ] = getRandom();
    m_Grid[m_Size - 1][m_Size - 1] = getRandom();

    step(m_Size);
}

void DiamondSquare::step(uint32_t cellSize)
{
    if (cellSize <= 2)
        return;

    diamond(cellSize);
    // printGrid();
    square(cellSize);
    // printGrid();


    step(std::ceil((float)cellSize / 2.0));
    // printGrid();
}

void DiamondSquare::diamond(uint32_t cellSize)
{
    const uint32_t steps = m_Size / std::floor(m_Size / (cellSize - 1));
    const float mag = cellSize / (float)m_Size;

    for (int x = 0; x < m_Size - 1; x += steps)
    {
        for (int y = 0; y < m_Size - 1; y += steps)
        {
            uint32_t maxLength = cellSize - 1;

            uint8_t TL = m_Grid[x            ][y            ];
            uint8_t TR = m_Grid[x + maxLength][y            ];
            uint8_t BL = m_Grid[x            ][y + maxLength];
            uint8_t BR = m_Grid[x + maxLength][y + maxLength];

            uint8_t average = (TL + TR + BL + BR) / 4;

            m_Grid[x + (cellSize / 2)][y + (cellSize / 2)] = average + getRandom(mag);
        }
    }
}

void DiamondSquare::square(uint32_t cellSize)
{
    const uint32_t steps = m_Size / std::floor(m_Size / (cellSize - 1));
    const float mag = cellSize / (float)m_Size;

    for (int x = 0; x < m_Size - 1; x += steps)
    {
        for (int y = 0; y < m_Size - 1; y += steps)
        {
            int32_t maxLength = cellSize - 1;
            int32_t halfScale = cellSize / 2;

            uint8_t v;
            uint32_t average;
            { // Top
                average = 0;
                average += m_Grid[x][y];
                average += m_Grid[x + maxLength][y];
                average += m_Grid[x + halfScale][y + halfScale];

                if (y - halfScale >= 0)
                {
                    average += m_Grid[x + halfScale][y - halfScale];
                    average /= 4;
                }
                else
                    average /= 3;

                m_Grid[x + halfScale][y] = average + getRandom(mag);
            }

            { // Bottom
                average = 0;
                average += m_Grid[x][y + maxLength];
                average += m_Grid[x + maxLength][y + maxLength];
                average += m_Grid[x + halfScale][y + halfScale];

                if (y + halfScale + maxLength < m_Size)
                {
                    average += m_Grid[x + halfScale][y + halfScale + maxLength];
                    average /= 4;
                }
                else
                    average /= 3;

                m_Grid[x + halfScale][y + maxLength] = average + getRandom(mag);
            }

            { // Left
                average = 0;
                average += m_Grid[x][y];
                average += m_Grid[x][y + maxLength];
                average += m_Grid[x + halfScale][y + halfScale];

                if (x - halfScale >= 0)
                {
                    average += m_Grid[x - halfScale][y + halfScale];
                    average /= 4;
                }
                else
                    average /= 3;

                m_Grid[x][y + halfScale] = average + getRandom(mag);
            }

            { // Right
                average = 0;
                average += m_Grid[x + maxLength][y];
                average += m_Grid[x + maxLength][y + maxLength];
                average += m_Grid[x + halfScale][y + halfScale];

                if (x + halfScale + maxLength < m_Size)
                {
                    average += m_Grid[x + halfScale + maxLength][y + halfScale];
                    average /= 4;
                }
                else
                    average /= 3;

                m_Grid[x + maxLength][y + halfScale] = average + getRandom(mag);
            }
            
        }
    }
}

void DiamondSquare::printGrid()
{
    for (int y = 0; y < m_Size; y++)
    {
        for (int x = 0; x < m_Size; x++)
        {
            printf("%0.*u ", 3, m_Grid[x][y]);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

uint8_t DiamondSquare::getRandom()
{
    uint8_t r = (rand() % 255);
    return r;
}

int8_t DiamondSquare::getRandom(float mag)
{
    int max = 255 * mag;
    uint8_t r = (rand() % max) - (max / 2);
    return r;
}
