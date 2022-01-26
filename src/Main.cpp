#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "Socket.hpp"
#include "LEDController.hpp"
#include "LEDMatrix.hpp"

#define LED_SIZE 21*36
#define MAX_BYTES 1400

//TODO: Fix for Serpentine Layout
//

void setCheckerboard(LEDMatrix& controller, bool even = false)
{
    for (int i = 0; i < 36; i++)
    {
        for (int j = 0; j < 21; j++)
        {
            uint8_t c = 0x00;
            if ((i + j) % 2 == even)
            {
                c = 0xFF;
            }
                controller.setLED(i, j, cRGB(c, c, c));
        }
    }
}

int main(int argc, char* argv[])
{
    Socket socket("192.168.0.99", 65506);

    LEDMatrix controller;
    controller.setup(36, 21, StartPosition::BOTTOM_LEFT);


    controller.setBrightness(255);
    controller.fillSolid(cRGB(0x00, 0xFF, 0xFF));

    // cRGB colourRGB = RGBFromHex(0x00945ccc);
    // std::cout << printRGB(colourRGB);
    // cHSV hsv = RGB2HSV(colourRGB);
    // std::cout << printHSV(hsv);
    // cRGB rgb = HSV2RGB(hsv);
    // std::cout << printRGB(rgb);

    controller.fillRainbow(cHSV(0, 255, 255), 2);

    int count = 0;
    int count2 = 0;
    int iteration = 0;
    uint8_t temp = 0;
    while (true)
    {
        if (count >= 100)
        {
            controller.fillRainbow(cHSV(temp, 255, 255), 1);
            count = 0;
            temp += 1;
            iteration++;
        }
        if (count2 % 100)
            controller.upload(socket);

        std::cout << "Count: " << (int)iteration << "\t\r" << std::flush;

        count++;
        count2++;
    }
}
