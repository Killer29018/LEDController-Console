#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glm/glm.hpp>

#include "System/Socket.hpp"

#include "Colour.hpp"
#include "LED/LEDMatrix.hpp"

#include "Utils/Logger.hpp"
#include "Window/WindowManager.hpp"

#include "EffectManager.hpp"

#include <thread>

class Application
{
public:
    static bool closeWindow;
    static uint8_t updateFPS;

    // static Window m_Window;
    static Socket m_Socket;
    static LEDMatrix m_Controller;
    // static LEDMatrixRenderer m_MatrixRenderer;
    // static Settings m_Settings_Panel;
    static EffectManager m_EffectManager;
    static Logger m_Logger;

    static bool m_Output;
private:
public:
    static void init(const char* name, const char* ip, uint32_t port);
    static void start();
private:
    Application() = default;
    ~Application() = default;
};

#endif
