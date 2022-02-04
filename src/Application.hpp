#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glm/glm.hpp>
#include "KRE/KRE.hpp"

#include "Window.hpp"
#include "Socket.hpp"

#include "Colour.hpp"
#include "LED/LEDMatrix.hpp"

#include "ImGui/ImGuiManager.hpp"
#include "ImGui/ImGuiPanel.hpp"

#include "LED/LEDMatrixRenderer.hpp"
#include "Panels/Settings.hpp"

#include "EffectManager.hpp"

class Application
{
public:
    static uint32_t updateFPS;

    static Window m_Window;
    static Socket m_Socket;
    static LEDMatrix m_Controller;
    static LEDMatrixRenderer m_MatrixRenderer;
    static Settings m_Settings_Panel;
    static EffectManager m_EffectManager;
private:
public:
    static void init(const char* name, glm::vec2 windowSize, const char* ip, uint32_t port);
    static void start();

    static void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void GLFWResizeCallback(GLFWwindow* window, int width, int height);
private:
    Application() = default;
    ~Application() = default;

    static void processKeys();
};

#endif
