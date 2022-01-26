#include "Application.hpp"
#include "Window.hpp"

Window Application::m_Window;
Socket Application::m_Socket;
LEDMatrix Application::m_Controller;

void Application::init(const char* name, glm::vec2 windowSize, const char* ip, uint32_t port)
{
    m_Window.setup(name, windowSize);
    glfwSetKeyCallback(m_Window.window, Application::GLFWKeyCallback);

    m_Socket.resetIp(ip, port);

    m_Controller.setup(36, 21, StartPosition::TOP_RIGHT);
    m_Controller.setBrightness(255);
}

void Application::start()
{
    static float deltaTotal = 0;
    static uint8_t hue = 0;

    while (!glfwWindowShouldClose(m_Window.window))
    {
        KRE::Clock::tick();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (deltaTotal >= (1/MAX_FPS))
        {
            m_Controller.fillRainbow(cHSV(hue++, 255, 255), 1);

            m_Controller.upload(m_Socket);

            std::cout << "FPS: " << (int)(1 / KRE::Clock::deltaTime) << "\t\r" << std::flush;
            deltaTotal = 0;
        }

        deltaTotal += KRE::Clock::deltaTime;

        glfwSwapBuffers(m_Window.window);
        glfwPollEvents();
    }
}

void Application::GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    switch (action)
    {
    case GLFW_PRESS: KRE::Keyboard::pressKey(key); break;
    case GLFW_RELEASE: KRE::Keyboard::unpressKey(key); break;
    }
}

void Application::processKeys()
{

}
