#include "Settings.hpp"

#include "../Application.hpp"

void Settings::setIpAndPort(const char* ip, int port)
{
    strcpy(m_IpBuf, ip);
    // m_IpBuf = ip;
    m_Port = port;
}

void Settings::renderImGui()
{
    if (ImGui::Begin("Settings"))
    {
        ImGui::PushItemWidth(-1);

        ImGui::Text("Output");
        ImGui::Checkbox("##Output", &Application::m_Output);
        
        ImGui::Text("Ip");
        if (ImGui::InputText("##IP", m_IpBuf, m_IpBytes, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Port");
        uint16_t one = 1;
        if (ImGui::InputScalar("##PORT", ImGuiDataType_U16, &m_Port, NULL, NULL, "%u", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Master Update Speed");
        int fps = Application::updateFPS;
        ImGui::SliderInt("##FPS", &fps, 10, 100);
        Application::updateFPS = fps;

        ImGui::Text("Brightness");
        int brightness = Application::m_Controller.getBrightness();
        ImGui::SliderInt("##Brightness", &brightness, 1, 255);
        Application::m_Controller.setBrightness((uint8_t)brightness);

        ImGui::PopItemWidth();

        ImGui::End();
    }
}
