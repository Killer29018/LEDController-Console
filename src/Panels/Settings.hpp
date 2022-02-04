#ifndef PANELS_SETTINGS_HPP
#define PANELS_SETTINGS_HPP

#include "../ImGui/ImGuiPanel.hpp"

class Settings : public ImGuiPanel
{
public:
    Settings() = default;
    ~Settings() = default;

    void renderImGui();
};

#endif
