#ifndef IMGUI_PANEL_HPP
#define IMGUI_PANEL_HPP

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiPanel
{
public:
    ImGuiPanel() = default;
    virtual ~ImGuiPanel() {}
    virtual void renderImGui() {}
};

#endif
