#include "WindowManager.hpp"

std::vector<WindowPanel*> WindowManager::m_Windows;

void WindowManager::addWindow(WindowPanel* window)
{
    m_Windows.push_back(window);
}

void WindowManager::render()
{
    for (auto window : m_Windows) window->render();
}
