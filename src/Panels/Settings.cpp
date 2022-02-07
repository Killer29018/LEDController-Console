#include "Settings.hpp"

#include "../Application.hpp"

#include "../Panels/Logger.hpp"

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
            Logger::log(LoggerType::LOG_INFO, "Changed target IP to: %s\n", m_IpBuf);
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Port");
        if (ImGui::InputScalar("##PORT", ImGuiDataType_U16, &m_Port, NULL, NULL, "%u", ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Logger::log(LoggerType::LOG_INFO, "Changed target Port to: %u\n", m_Port);
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Master Update Speed");
        uint8_t min = 1, max = 255;
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

        StartPosition position = Application::m_Controller.getStartPosition();
        int positionInt = static_cast<int>(position);
        static const char* positions[] = { "Top Left", "Top Right", "Bottom Left", "Bottom Right" };
        ImGui::Text("Start Position");
        if (ImGui::BeginCombo("##Position", positions[positionInt]))
        {
            for (int n = 0; n < 4; n++)
            {
                const bool isSelected = (positionInt == n);
                if (ImGui::Selectable(positions[n], isSelected))
                {
                    Application::m_Controller.setStartPosition(static_cast<StartPosition>(n));
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        StartDirection direction = Application::m_Controller.getStartDirection();
        int directionInt = static_cast<int>(direction);
        static const char* directions[] =  { "Snake Column Wise", "Snake Row Wise" };
        ImGui::Text("Start Direction");
        if (ImGui::BeginCombo("##Directions", directions[directionInt]))
        {
            for (int n = 0; n < 2; n++)
            {
                const bool isSelected = (directionInt == n);
                if (ImGui::Selectable(directions[n], isSelected))
                {
                    Application::m_Controller.setStartDirection(static_cast<StartDirection>(n));
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();
    }
    ImGui::End();
}
