#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "Socket.hpp"
#include "LEDController.hpp"
#include "LEDMatrix.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include "KRE/KRE.hpp"

#define MAX_BYTES 1400

static const unsigned int SCREEN_WIDTH = 800;
static const unsigned int SCREEN_HEIGHT = 600;
static KRE::Camera camera(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LED Controller", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    int version = gladLoadGL(glfwGetProcAddress);
    if (!version)
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << "\n";

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Socket socket("192.168.0.99", 65506);

    LEDMatrix controller;
    controller.setup(36, 21, StartPosition::BOTTOM_LEFT);

    controller.setBrightness(255);

    controller.fillRainbow(cHSV(0, 255, 255), 2);

    uint8_t temp = 0;
    float deltaTotal = 0;
    float delta = 0;
    while (!glfwWindowShouldClose(window))
    {
        KRE::Clock::tick();
        delta = KRE::Clock::deltaTime;

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (deltaTotal >= 0.5)
        {
            controller.fillRainbow(cHSV(temp, 255, 255), 1);
            temp += 1;
            controller.upload(socket);
            std::cout << "FPS: " << (int)(1 / delta) << " : " << "\t\r" << std::flush;
            deltaTotal = 0; 
        }

        deltaTotal += delta;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}
