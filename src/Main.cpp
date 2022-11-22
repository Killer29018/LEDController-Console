// #define STB_IMAGE_IMPLEMENTATION
#include "Application.hpp"

#include "Console.hpp"
#include "Effects/EffectsExtra.hpp"

int main()
{
#ifdef WIN32
#ifndef DEBUG_MODE
    ShowWindow( GetConsoleWindow(), SW_HIDE );
#endif
#endif

    Application::init("LED Controller", "192.168.0.99", 65506);
    Application::start();
}
