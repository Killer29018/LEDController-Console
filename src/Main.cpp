#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "Socket.hpp"
#include "LEDController.hpp"
#include "LEDMatrix.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Window.hpp"

#define MAX_BYTES 1400

static const unsigned int SCREEN_WIDTH = 800;
static const unsigned int SCREEN_HEIGHT = 600;
static KRE::Camera camera(glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    Window window;
    window.setup("LED Controller", { SCREEN_WIDTH, SCREEN_HEIGHT });

    glfwSetKeyCallback(window.window, keyCallback);

    Socket socket("192.168.0.99", 65506);

    LEDMatrix controller;
    controller.setup(36, 21, StartPosition::BOTTOM_LEFT);

    controller.setBrightness(255);

    controller.fillRainbow(cHSV(0, 255, 255), 2);

    uint8_t temp = 0;
    float deltaTotal = 0;
    float delta = 0;
    while (!glfwWindowShouldClose(window.window))
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

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
