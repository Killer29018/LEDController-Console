#ifndef PANELS_SETTINGS_HPP
#define PANELS_SETTINGS_HPP

#include "../ImguiWindow.hpp"

class Settings : public ImguiWindow
{
public:
    Settings() = default;
    ~Settings() = default;

    void renderImgui();
};

#endif
