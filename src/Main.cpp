// #define STB_IMAGE_IMPLEMENTATION
#include "Application.hpp"

int main()
{
#ifdef WIN32
#ifndef DEBUG_MODE
    ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif
#endif

    // srand(1);

    Application::init("LED Controller", "192.168.0.99", 65506);
    Application::start();
}
