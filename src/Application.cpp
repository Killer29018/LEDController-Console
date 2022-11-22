#include "Application.hpp"

#include "System/FreeType.hpp"

#include <chrono>
#include <thread>
#include <iostream>

uint8_t Application::updateFPS = 60;
bool Application::m_CloseWindow = false;

Socket Application::m_Socket;
LEDMatrix Application::m_Controller;
// LEDMatrixRenderer Application::m_MatrixRenderer;
// Settings Application::m_Settings_Panel;
EffectManager Application::m_EffectManager;
Logger Application::m_Logger;

bool Application::m_Output = false;

void Application::init(const char* name, const char* ip, uint32_t port)
{
    m_Socket.resetIp(ip, port);
    // m_Settings_Panel.setIpAndPort(ip, port);

    m_Controller.setup(36, 21, StartPosition::BOTTOM_LEFT, StartDirection::SNAKE_ROW);
    
    FreeType::init();
    FreeType::loadFont("res/fonts/CascadiaMonoPL.ttf", m_Controller.getRows() / 2);

    m_Controller.setBrightness(255);

    // m_MatrixRenderer.init(&m_Controller, 2);

    m_EffectManager.init(m_Controller);
    m_EffectManager.setEffect(EffectEnum::NONE);

    // ImGuiManager::init(m_Window.window);
    // ImGuiManager::addWindow(&m_MatrixRenderer);
    // ImGuiManager::addWindow(&m_Settings_Panel);
    // ImGuiManager::addWindow(&m_EffectManager);
    // ImGuiManager::addWindow(&m_Logger);
}

void Application::start()
{
    static float deltaTotal = 0;
    static auto previous = std::chrono::high_resolution_clock::now();

    while (!m_CloseWindow)
    {
        auto current = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double, std::milli>(current-previous).count();
        previous = current;

        // ImGuiManager::render();

        if (deltaTotal >= (1/(float)updateFPS))
        {
            if (m_Output) m_Controller.upload(m_Socket);

            deltaTotal = 0;
        }

        // std::cout << dt << "\n";

        deltaTotal += (float)dt;
    }
}
