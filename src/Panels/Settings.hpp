#ifndef PANELS_SETTINGS_HPP
#define PANELS_SETTINGS_HPP

#include "../ImGui/ImGuiPanel.hpp"

#include <cstdint>

class Settings : public ImGuiPanel
{
private:
    static const int m_IpBytes = 13;
    char m_IpBuf[m_IpBytes];
    uint16_t m_Port = 0;
public:
    Settings() = default;
    ~Settings() = default;

    void setIpAndPort(const char* ip, int port);

    void renderImGui();
};

#endif
