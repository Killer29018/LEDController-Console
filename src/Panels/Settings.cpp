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
        if (ImGui::InputScalar("##PORT", ImGuiDataType_U16, &m_Port, NULL, NULL, "%u", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Master Update Speed");
        uint8_t min = 0, max = 255;
        ImGui::SliderScalar("##FPS", ImGuiDataType_U8, &Application::updateFPS, &min, &max, "%u");

        ImGui::Text("Brightness");
        uint8_t brightness = Application::m_Controller.getBrightness();
        ImGui::SliderScalar("##Brightness", ImGuiDataType_U8, &brightness, &min, &max, "%u");
        Application::m_Controller.setBrightness(brightness);

        ImGui::Text("Size");
        uint32_t cols = Application::m_Controller.getColumns();
        uint32_t rows = Application::m_Controller.getRows();
        uint32_t elements[2] = { cols, rows };
        
        if (ImGui::InputScalarN("##SIZE", ImGuiDataType_U32, elements, 2, NULL, NULL, "%u"))
        {
            Application::m_Controller.changeSize(elements[0], elements[1]);
        }



        ImGui::PopItemWidth();

        ImGui::End();
    }
}
