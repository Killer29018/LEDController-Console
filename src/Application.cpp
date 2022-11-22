#include "Application.hpp"

#include "System/FreeType.hpp"

#include <chrono>
#include <thread>
#include <iostream>

#include "Console.hpp"

uint8_t Application::updateFPS = 20;
bool Application::m_CloseWindow = false;

Socket Application::m_Socket;
LEDMatrix Application::m_Controller;
// LEDMatrixRenderer Application::m_MatrixRenderer;
// Settings Application::m_Settings_Panel;
EffectManager Application::m_EffectManager;
Logger Application::m_Logger;

bool Application::m_Output = true;

void Application::init(const char* name, const char* ip, uint32_t port)
{
    m_Socket.resetIp(ip, port);
    // m_Settings_Panel.setIpAndPort(ip, port);

    m_Controller.setup(36, 21, StartPosition::BOTTOM_LEFT, StartDirection::SNAKE_ROW);
    
    FreeType::init();
    FreeType::loadFont("res/fonts/CascadiaMonoPL.ttf", m_Controller.getRows() / 2);

    m_Controller.setBrightness(255);

    m_EffectManager.init(m_Controller);
    m_EffectManager.setEffect(EffectEnum::PLASMA);

    WindowManager::addWindow(&m_EffectManager);

    Console::init(&m_EffectManager);

    Console::printCommands();
}

void Application::start()
{
    static float deltaTotal = 0;
    static auto previous = std::chrono::high_resolution_clock::now();

    while (!m_CloseWindow)
    {
        auto current = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(current-previous).count();
        previous = current;

        EffectManager::dt = (float)dt;

        WindowManager::render();

        if (deltaTotal >= (1.0f/(float)updateFPS))
        {
            if (m_Output) m_Controller.upload(m_Socket);

            deltaTotal = 0;
        }

        // std::cout << dt << "\n";

        deltaTotal += (float)dt;
    }
}
