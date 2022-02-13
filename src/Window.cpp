#include "Window.hpp"

#include "Panels/Logger.hpp"

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setup(const char* name, glm::vec2 windowSize)
{
    m_WindowSize = windowSize;

    initGLFW(name);
    initOpengl();
}

void Window::setWindowSize(glm::vec2 windowSize)
{
    m_WindowSize = windowSize;
}

void Window::initGLFW(const char* name)
{
    if (!glfwInit())
        exit(-1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    window = glfwCreateWindow(m_WindowSize.x, m_WindowSize.y, name, NULL, NULL);

    if (!window)
    {
#ifdef DEBUG_MODE
        Logger::log(LoggerType::LOG_ERROR, "Failed to create window");
#endif
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);
}

void Window::initOpengl()
{
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version)
    {
#ifdef DEBUG_MODE
        Logger::log(LoggerType::LOG_ERROR, "Failed to initialize GLAD");
#endif
        exit(-1);
    }

#ifdef DEBUG_MODE
    Logger::log(LoggerType::LOG_INFO, "Loaded OpenGL %u.%u", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
#endif

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_WindowSize.x, m_WindowSize.y);
}
