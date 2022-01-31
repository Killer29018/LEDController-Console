#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "Socket.hpp"
#include "LEDController.hpp"
#include "LEDMatrix.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Window.hpp"

#include "Application.hpp"

static const unsigned int SCREEN_WIDTH = 800;
static const unsigned int SCREEN_HEIGHT = 600;

int main()
{
    Application::init("LED Controller", { SCREEN_WIDTH, SCREEN_HEIGHT }, "192.168.0.99", 65506);
    Application::start();
}
