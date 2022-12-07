#include "Application.hpp"

#include "System/FreeType.hpp"


#include <chrono>
#include <thread>
#include <iostream>

#include "Console.hpp"

uint8_t Application::updateFPS = 40;
bool Application::closeWindow = false;

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

    m_Controller.setup(7, 6, StartPosition::TOP_LEFT, StartDirection::SNAKE_COLUMN);
    
    FreeType::init();
    FreeType::loadFont("res/fonts/CascadiaMonoPL.ttf", m_Controller.getRows() / 2);

    m_Controller.setBrightness(255);

    m_EffectManager.init(m_Controller);
    m_EffectManager.setEffect(EffectEnum::NONE);

    WindowManager::addWindow(&m_EffectManager);

    Console::init(&m_EffectManager, &m_Socket, ip, port);
}

void Application::start()
{
    static float deltaTotal = 0;
    static auto previous = std::chrono::high_resolution_clock::now();

    std::thread console(Console::start);

    while (!closeWindow)
    {
        auto current = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(current-previous).count();
        previous = current;

        EffectManager::dt = (float)dt;

        WindowManager::render();

        if (deltaTotal >= (1.0f/(float)updateFPS))
        {
            if (m_Output) m_Controller.upload(m_Socket);
            // std::cout << "Update\n";

            deltaTotal = 0;
        }

        deltaTotal += (float)dt;
    }
    console.join();
}
