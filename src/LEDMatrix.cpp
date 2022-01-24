#include "LEDMatrix.hpp"


LEDMatrix::~LEDMatrix()
{
    for (int i= 0; i < m_Columns; i++)
    {
        delete[] m_IndexArr[i];
    }
    delete[] m_IndexArr;
}

void LEDMatrix::setup(unsigned int width, unsigned int height, StartPosition direction)
{
    m_Rows = height;
    m_Columns = width;
    LEDController::setup(m_Rows * m_Columns);

    m_IndexArr = new uint16_t*[m_Columns];
    for (int i = 0; i < m_Columns; i++)
    {
        m_IndexArr[i] = new uint16_t[m_Rows];
    }

    for (int x = 0; x < m_Columns; x++)
    {
        for (int y = 0; y < m_Rows; y++)
        {
            int xIndex, yIndex;
            if (direction == StartPosition::TOP_LEFT || direction == StartPosition::BOTTOM_LEFT)
                xIndex = x * m_Rows;
            else
                xIndex = (m_Columns - x - 1) * m_Rows;

            if (direction == StartPosition::TOP_LEFT || direction == StartPosition::BOTTOM_RIGHT)
                yIndex = (x%2 == 0 ? y : m_Rows - y - 1);
            else
                yIndex = (x%2 == 1 ? y : m_Rows - y - 1);

            m_IndexArr[x][y] = xIndex + yIndex;

        }
    }
}

void LEDMatrix::setLED(int x, int y, const cRGB& led)
{
    m_LEDs[getIndex(x, y)] = led;
}

cRGB& LEDMatrix::getLED(int x, int y)
{
    return m_LEDs[getIndex(x, y)];
}

int LEDMatrix::getIndex(int x, int y)
{
    return m_IndexArr[x][y];
}
