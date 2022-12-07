// #define STB_IMAGE_IMPLEMENTATION
#include "Application.hpp"

#include "Console.hpp"
#include "Effects/EffectsExtra.hpp"

int main()
{
    Application::init("LED Controller", "192.168.0.96", 65506);
    Application::start();
}
