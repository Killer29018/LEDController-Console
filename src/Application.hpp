#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glm/glm.hpp>
#include "KRE/KRE.hpp"

#include "Window.hpp"
#include "Socket.hpp"

#include "Colour.hpp"
#include "LEDMatrix.hpp"
#include "LEDMatrixRenderer.hpp"

#include "ImguiManager.hpp"

#define MAX_FPS 60

class Application
{
public:
private:
    static Window m_Window;
    static Socket m_Socket;
    static LEDMatrix m_Controller;
    static LEDMatrixRenderer m_MatrixRenderer;
public:
    static void init(const char* name, glm::vec2 windowSize, const char* ip, uint32_t port);
    static void start();

    static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
private:
    Application() = default;

    static void processKeys();
};

#endif
