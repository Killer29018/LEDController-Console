#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <vector>

#include "WindowPanel.hpp"

class WindowManager
{
private:
    static std::vector<WindowPanel*> m_Windows;
public:
    static void addWindow(WindowPanel* window);

    static void render();
private:
    WindowManager() = default;
    ~WindowManager() = default;
};

#endif
