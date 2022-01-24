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

class LEDMatrix : public LEDController
{
private:
    unsigned int m_Rows, m_Columns;
    uint16_t** m_IndexArr;
public:
    LEDMatrix() = default;
    ~LEDMatrix();

    void setup(unsigned int width, unsigned int height, StartPosition direction = StartPosition::TOP_RIGHT);

    void setLED(int x, int y, const cRGB& led);

    cRGB& getLED(int x, int y);
private:
    int getIndex(int x, int y);
};

#endif
