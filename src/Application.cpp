#include "Application.hpp"

#include "System/FreeType.hpp"

uint8_t Application::updateFPS = 60;

// Window Application::m_Window;
Socket Application::m_Socket;
LEDMatrix Application::m_Controller;
// LEDMatrixRenderer Application::m_MatrixRenderer;
// Settings Application::m_Settings_Panel;
EffectManager Application::m_EffectManager;
Logger Application::m_Logger;

bool Application::m_Output = false;

void Application::init(const char* name, glm::vec2 windowSize, const char* ip, uint32_t port)
{
    // m_Window.setup(name, windowSize);
    // glfwSetKeyCallback(m_Window.window, Application::GLFWKeyCallback);
    // glfwSetWindowSizeCallback(m_Window.window, Application::GLFWResizeCallback);

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

    while (true)
    // while (!glfwWindowShouldClose(m_Window.window))
    {
        // KRE::Clock::tick();

        // ImGuiManager::preRender();
        // ImGuiManager::render();
        // ImGuiManager::postRender();

        // glfwSwapBuffers(m_Window.window);
        // glfwPollEvents();

        if (deltaTotal >= (1/(float)updateFPS))
        {
            if (m_Output) m_Controller.upload(m_Socket);

            deltaTotal = 0;
        }

        // deltaTotal += KRE::Clock::deltaTime;
        deltaTotal += 0.1f;
    }
}

// void Application::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
// {
//     // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//     //     glfwSetWindowShouldClose(window, true);

//     switch (action)
//     {
//     case GLFW_PRESS: KRE::Keyboard::pressKey(key); break;
//     case GLFW_RELEASE: KRE::Keyboard::unpressKey(key); break;
//     }
// }

// void Application::GLFWResizeCallback(GLFWwindow* window, int width, int height)
// {
//     glViewport(0, 0, width, height);
//     m_Window.setWindowSize({ width, height } );
// }

void Application::processKeys()
{

}
