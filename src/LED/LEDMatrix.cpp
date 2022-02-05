#include "LEDMatrix.hpp"


LEDMatrix::~LEDMatrix()
{
    for (int i= 0; i < m_Columns; i++)
    {
        delete[] m_IndexArr[i];
    }
    delete[] m_IndexArr;
}

void LEDMatrix::setup(unsigned int width, unsigned int height, StartPosition position)
{
    m_Rows = height;
    m_Columns = width;
    m_StartPosition = position;

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
    for (int i = 0; i < m_Columns; i++)
    {
        m_LEDs[getIndex(i, row)] = colour;
    }
}

void LEDMatrix::fillColumn(cRGB colour, int column)
{
    for (int i = 0; i < m_Rows; i++)
    {
        m_LEDs[getIndex(column, i)] = colour;
    }
}

void LEDMatrix::fillRainbow(cHSV hsv, uint8_t deltaHue)
{
    for (int i = 0; i < m_Columns; i++)
    {
        for (int j = 0; j < m_Rows; j++)
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

int LEDMatrix::getIndex(int x, int y)
{
    return m_IndexArr[x][y];
}

void LEDMatrix::createIndexArr()
{
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
            if (m_StartPosition == StartPosition::TOP_LEFT || m_StartPosition == StartPosition::BOTTOM_LEFT)
                xIndex = x * m_Rows;
            else
                xIndex = (m_Columns - x - 1) * m_Rows;

            if (m_StartPosition == StartPosition::TOP_LEFT || m_StartPosition == StartPosition::BOTTOM_RIGHT)
                yIndex = (x%2 == 0 ? y : m_Rows - y - 1);
            else
                yIndex = (x%2 == 1 ? y : m_Rows - y - 1);

            m_IndexArr[x][y] = xIndex + yIndex;
        }
    }
}

void LEDMatrix::deleteIndexArr()
{
    for (int i= 0; i < m_Columns; i++)
    {
        delete[] m_IndexArr[i];
    }
    delete[] m_IndexArr;
}
