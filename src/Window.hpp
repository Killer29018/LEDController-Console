#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "KRE/KRE.hpp"

#include <cstdlib>

class Window
{
public:
    GLFWwindow* window;
private:
    glm::vec2 m_WindowSize;
public:
    Window() = default;
    ~Window();

    void setup(const char* name, glm::vec2 windowSize);
private:
    void initGLFW(const char* name);
    void initOpengl();
};

#endif
