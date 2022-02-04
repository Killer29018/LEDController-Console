#ifndef IMGUI_MANAGER_HPP
#define IMGUI_MANAGER_HPP

#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>

#include "ImGuiPanel.hpp"

class ImGuiManager
{
public:
    static ImGuiIO* io;
private:
    static std::vector<ImGuiPanel*> m_Windows;
    static GLFWwindow* m_Window;
public:
    static void init(GLFWwindow* window);

    static void addWindow(ImGuiPanel* window);

    static void preRender();
    static void render();
    static void postRender();
private:
    ImGuiManager() = default;
    ~ImGuiManager() = default;

    static void initImGui();

    static void showDockspace(bool* open);
    static void setupStyle();
};

#endif
