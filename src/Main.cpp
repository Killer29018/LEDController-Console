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
    controller.solidColour(cRGB(0x00, 0xFF, 0xFF));
    // controller.setLED(0, 1, 255, 255, 255);
    
    cRGB colourRGB = cRGB(0x37, 0x52, 0x56);
    cHSV hsv = rgb2hsv(colourRGB);

    std::cout << (int)hsv.H << " : " << (int)100 * (hsv.S / 255.0) << " : " << (int)100 * (hsv.V / 255.0) << "\n";

    int count = 0;
    bool even = false;
    while (true)
    {
        // if (count >= 5000)
        // {
        //     setCheckerboard(controller, even);
        //     even = !even;
        //     count= 0;
        // }
        // count++;
        controller.upload(socket);
    }
}
