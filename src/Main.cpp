#define STB_IMAGE_IMPLEMENTATION

#include "Application.hpp"

static const unsigned int SCREEN_WIDTH  = 800;
static const unsigned int SCREEN_HEIGHT = 600;

int main()
{
#ifndef DEBUG_MODE
    ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif

    Application::init("LED Controller", { SCREEN_WIDTH, SCREEN_HEIGHT }, "192.168.0.99", 65506);
    Application::start();
}
