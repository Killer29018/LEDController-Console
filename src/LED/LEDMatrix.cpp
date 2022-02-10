#include "LEDMatrix.hpp"

#include <cmath>

LEDMatrix::~LEDMatrix()
{
    for (uint32_t i= 0; i < m_Columns; i++)
    {
        delete[] m_IndexArr[i];
    }
    delete[] m_IndexArr;
}

void LEDMatrix::setup(unsigned int width, unsigned int height, StartPosition position, StartDirection direction)
{
    m_Rows = height;
    m_Columns = width;
    m_StartPosition = position;
    m_Direction = direction;

    LEDController::setup(m_Rows * m_Columns);

    createIndexArr();
}

void LEDMatrix::setLED(int x, int y, const cRGB& led)
{
    m_LEDs[getIndex(x, y)] = led;
}

cRGB& LEDMatrix::getLED(int x, int y)
{
    return m_LEDs[getIndex(x, y)];
}

cRGB LEDMatrix::getLEDWBrightness(int x, int y)
{
    return m_LEDs[getIndex(x, y)] / (255.0/ m_Brightness);
}

void LEDMatrix::fillRow(cRGB colour, int row)
{
    for (uint32_t i = 0; i < m_Columns; i++)
    {
        m_LEDs[getIndex(i, row)] = colour;
    }
}

void LEDMatrix::fillColumn(cRGB colour, int column)
{
    for (uint32_t i = 0; i < m_Rows; i++)
    {
        m_LEDs[getIndex(column, i)] = colour;
    }
}

void LEDMatrix::fillRainbow(cHSV hsv, uint8_t deltaHue)
{
    for (uint32_t i = 0; i < m_Columns; i++)
    {
        for (uint32_t j = 0; j < m_Rows; j++)
        {
            int x = i;
            int y = m_Rows - j - 1;

            if (i % 2 == 0)
                y = j;

            if (j == 0)
                m_LEDs[getIndex(x, y)] = cRGB(255, 255, 255);

            uint8_t hue = hsv.h + (deltaHue * (i * m_Rows + j));
            m_LEDs[getIndex(x, y)] = cHSV(hue, hsv.S, hsv.V);
        }
    }
}

void LEDMatrix::changeSize(int cols, int rows)
{
    deleteIndexArr();

    m_Columns = cols;
    m_Rows = rows;

    createIndexArr();

    LEDController::changeSize(m_Columns * m_Rows);
}

void LEDMatrix::changeCols(int cols)
{
    changeSize(cols, m_Rows);
}

void LEDMatrix::changeRows(int rows)
{
    changeSize(m_Columns, rows);
}

void LEDMatrix::setStartPosition(StartPosition position)
{
    deleteIndexArr();
    m_StartPosition = position;
    createIndexArr();
}

void LEDMatrix::setStartDirection(StartDirection direction)
{
    deleteIndexArr();
    m_Direction = direction;
    createIndexArr();
}

int LEDMatrix::getIndex(int x, int y)
{
    return m_IndexArr[x][y];
}

void LEDMatrix::createIndexArr()
{
    m_IndexArr = new uint16_t*[m_Columns];
    for (uint32_t i = 0; i < m_Columns; i++)
    {
        m_IndexArr[i] = new uint16_t[m_Rows];
    }

    int xIndex, yIndex;
    if (m_Direction == StartDirection::SNAKE_ROW)
    {
        for (uint32_t x = 0; x < m_Columns; x++)
        {
            for (uint32_t y = 0; y < m_Rows; y++)
            {
                switch (m_StartPosition)
                {
                case StartPosition::TOP_LEFT:
                    xIndex = x * m_Rows;
                    yIndex = ((x & 1) == 0) ? y : m_Rows - y - 1;
                    break;
                case StartPosition::TOP_RIGHT:
                    xIndex = (m_Columns - x - 1) * m_Rows;

                    if ((m_Columns & 1) == 0)
                        yIndex = ((x & 1) == 1) ? y : m_Rows - y - 1;
                    else
                        yIndex = ((x & 1) == 0) ? y : m_Rows - y - 1;
                    break;
                case StartPosition::BOTTOM_LEFT:
                    xIndex = x * m_Rows;
                    yIndex = ((x & 1) == 1) ? y : m_Rows - y - 1;
                    break;
                case StartPosition::BOTTOM_RIGHT:
                    xIndex = (m_Columns - x - 1) * m_Rows;

                    if ((m_Columns & 1) == 1)
                        yIndex = ((x & 1) == 1) ? y : m_Rows - y - 1;
                    else
                        yIndex = ((x & 1) == 0) ? y : m_Rows - y - 1;
                    break;
                }
                m_IndexArr[x][y] = xIndex + yIndex;
            }
        }
    }
    else
    {
        for (uint32_t y = 0; y < m_Rows; y++)
        {
            for (uint32_t x = 0; x < m_Columns; x++)
            {
                switch (m_StartPosition)
                {
                case StartPosition::TOP_LEFT:
                    xIndex = ((y & 1) == 0) ? x : m_Columns - x - 1;
                    yIndex = y * m_Columns;
                    break;
                case StartPosition::TOP_RIGHT:
                    xIndex = ((y & 1) == 1) ? x : m_Columns - x - 1;
                    yIndex = y * m_Columns;
                    break;
                case StartPosition::BOTTOM_LEFT:
                    if ((m_Rows & 1) == 0)
                        xIndex = ((y & 1) == 1) ? x : m_Columns - x - 1; 
                    else
                        xIndex = ((y & 1) == 0) ? x : m_Columns - x - 1;

                    yIndex = (m_Rows - y - 1) * m_Columns;
                    break;
                case StartPosition::BOTTOM_RIGHT:
                    if ((m_Rows & 1) == 0)
                        xIndex = ((y & 1) == 0) ? x : m_Columns - x - 1; 
                    else
                        xIndex = ((y & 1) == 1) ? x : m_Columns - x - 1;

                    yIndex = (m_Rows - y - 1) * m_Columns;
                    break;
                }

                m_IndexArr[x][y] = xIndex + yIndex;
            }
        }
    }
}

void LEDMatrix::deleteIndexArr()
{
    for (uint32_t i= 0; i < m_Columns; i++)
    {
        delete[] m_IndexArr[i];
    }
    delete[] m_IndexArr;
}

void LEDMatrix::printIndexArr()
{
    int characters = std::ceil(std::log10(m_Rows * m_Columns));
    for (uint32_t y = 0; y < m_Rows; y++)
    {
        for (uint32_t x = 0; x < m_Columns; x++)
        {
            printf("%.*u ", characters, m_IndexArr[x][y]);
        }
        std::cout << "\n";
    }
}
