#ifndef LED_MATRIX_HPP
#define LED_MATRIX_HPP

#include "LEDController.hpp"

enum class StartPosition
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

enum class StartDirection
{
    SNAKE_COLUMN,
    SNAKE_ROW
};

class LEDMatrix : public LEDController
{
private:
    uint32_t m_Rows, m_Columns;
    uint16_t** m_IndexArr;

    StartPosition m_StartPosition;
    StartDirection m_Direction;
public:
    LEDMatrix() = default;
    ~LEDMatrix();

    void setup(unsigned int width, unsigned int height, StartPosition position = StartPosition::TOP_RIGHT, StartDirection direction = StartDirection::SNAKE_ROW);

    void setLED(int x, int y, const cHSV& led);

    cHSV& getLED(int x, int y);
    cHSV getLEDWBrightness(int x, int y);

    void fillRow(cHSV colour, int row);
    void fillColumn(cHSV colour, int col);
    void fillRainbow(cHSV hsv, uint8_t deltaHue);

    uint32_t getColumns() { return m_Columns; }
    uint32_t getRows() { return m_Rows; }

    void changeSize(int cols, int rows);
    void changeCols(int cols);
    void changeRows(int rows);

    void setStartPosition(StartPosition position);
    void setStartDirection(StartDirection direction);

    StartPosition getStartPosition() { return m_StartPosition; }
    StartDirection getStartDirection() { return m_Direction; }
private:
    int getIndex(int x, int y);

    void createIndexArr();
    void deleteIndexArr();

    void printIndexArr();
};

#endif
