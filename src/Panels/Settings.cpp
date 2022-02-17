#include "Settings.hpp"

#include "../Application.hpp"

#include "../Panels/Logger.hpp"

#include <algorithm>

#include "imgui_internal.h"

void Settings::setIpAndPort(const char* ip, int port)
{
    strcpy(m_IpBuf, ip);
    // m_IpBuf = ip;
    m_Port = port;
}

void Settings::renderImGui()
{
    if (ImGui::BeginMainMenuBar())
    {
        ImGui::PushItemWidth(-1);
        if (ImGui::BeginMenu("Settings"))
        {
            renderSettingsMenu();
            ImGui::EndMenu();
        }

        ImGui::PopItemWidth();
        ImGui::EndMainMenuBar();
    }
}

void Settings::renderSettingsMenu()
{
    ImGui::Text("Brightness");

    uint8_t min = 1, max = 255;

    uint8_t brightness = Application::m_Controller.getBrightness();
    ImGui::SliderScalar("##Brightness", ImGuiDataType_U8, &brightness, &min, &max, "%u");
    Application::m_Controller.setBrightness(brightness);

    if (ImGui::BeginMenu("Output Settings"))
    {
        ImGui::Text("Output");
        ImGui::Checkbox("##Output", &Application::m_Output);

        ImGui::Text("Ip");
        if (ImGui::InputText("##IP", m_IpBuf, m_IpBytes, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            Logger::log(LoggerType::LOG_INFO, "Changed target IP to: %s\n", m_IpBuf);
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Port");
        if (ImGui::InputScalar("##PORT", ImGuiDataType_U16, &m_Port, NULL, NULL, "%u", ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
        {
            Logger::log(LoggerType::LOG_INFO, "Changed target Port to: %u\n", m_Port);
            Application::m_Socket.resetIp(m_IpBuf, m_Port);
        }

        ImGui::Text("Master Update Speed");
        ImGui::SliderScalar("##FPS", ImGuiDataType_U8, &Application::updateFPS, &min, &max, "%u");

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Matrix Settings"))
    {
        ImGui::Text("Size");
        uint32_t cols = Application::m_Controller.getColumns();
        uint32_t rows = Application::m_Controller.getRows();
        uint32_t elements[2] = { cols, rows };

        if (ImGui::InputScalarN("##Size", ImGuiDataType_U32, elements, 2, NULL, NULL, "%u"))
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
        ImGui::EndMenu();
    }

    if (ImGui::Button("Palette Settings", ImVec2(-1.0f, 0.0f))) ImGui::OpenPopup("PaletteModal");

    ImGui::SetNextWindowSize(ImVec2(300, 300));
    if (ImGui::BeginPopupModal("PaletteModal", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize))
    {
        renderPaletteMenu();
        ImGui::EndPopup();
    }
}

void Settings::renderPaletteMenu()
{
    const ImGuiColorEditFlags colorEditFlags = ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoSmallPreview;

    // ColourPalette& palette = Palettes::AllPalettes[Application::m_Controller.getPalette()];
    ColourPalette& palette = Palettes::AllPalettes[Palettes::CUSTOM];
    // cHSV& inputHSV = palette[0].colour;
    cRGB paletteColour;
    ImVec4 imHSVColour;
    ImVec4 imRGBColour;

    bool sortArray = false;
    int32_t deleteIndex = -1;

    static uint8_t inputHue = 0;
    static ImVec4 inputHSV = ImVec4(0.0f, 1.0f, 1.0f, 0.0f);

    uint32_t width = ImGui::GetContentRegionAvail().x / 2 - ImGui::GetStyle().ItemSpacing.x;
    ImGui::PushItemWidth(width);
    uint8_t min = 0, max = 255;
    ImGui::DragScalar("##InputHue", ImGuiDataType_U8, &inputHue, 1.0f, &min, &max, "Hue: %u", ImGuiInputTextFlags_CharsDecimal);
    ImGui::SameLine();
    ImGui::ColorEdit3("##InputHSV", (float*)&inputHSV, ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_DisplayHSV | colorEditFlags);
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(-1);
    if (ImGui::Button("Add Hue", ImVec2(-1, 0)))
    {
        palette.emplace_back(inputHue, cHSV(inputHSV.x * 255, inputHSV.y * 255, inputHSV.y * 255));
        sortArray = true;
    }
    ImGui::PopItemWidth();

    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoClip | ImGuiTableFlags_SizingStretchProp;
    if (ImGui::BeginTable("Palette", 2, flags))
    {
        ImGui::TableSetupColumn("Input Hue");
        ImGui::TableSetupColumn("Output HSV", 0, -1);
        ImGui::TableHeadersRow();
        for (size_t row = 0; row < palette.size(); row++)
        {
            ImGui::PushID(row * 10);
            ImGui::PushItemWidth(-1);

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal;

            if (row == 0 || row == palette.size() - 1)
                flags |= ImGuiInputTextFlags_ReadOnly;

            if (ImGui::InputScalar("##HUE", ImGuiDataType_U8, &palette[row].targetHue, NULL, NULL, "%u", inputFlags))
            {
                sortArray = true;
            }

            ImGui::SameLine();
            if (row == 0 || row == palette.size() - 1)
            {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button("X"))
            {
                deleteIndex = row;
            }
            if (row == 0 || row == palette.size() - 1)
            {
                ImGui::EndDisabled();
            }

            {
                ImGui::TableSetColumnIndex(1);
                cHSV inputHSV = palette[row].colour;
                paletteColour = inputHSV;

                imHSVColour = ImVec4(inputHSV.h / 255.0f, inputHSV.s / 255.0f, inputHSV.v / 255.0f, 0.0f);
                imRGBColour = ImVec4(paletteColour.r / 255.0f, paletteColour.g / 255.0f, paletteColour.b / 255.0f, 0.0f);

                ImGui::ColorButton("##PColourDisplay", imRGBColour, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip | colorEditFlags);

                ImGui::SameLine();

                ImGui::ColorEdit3("##PHSVInput", (float*)&imHSVColour, ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_DisplayHSV | colorEditFlags);

                inputHSV.h = imHSVColour.x * 255.0f;
                inputHSV.s = imHSVColour.y * 255.0f;
                inputHSV.v = imHSVColour.z * 255.0f;

                palette[row].colour = inputHSV;
            }

            ImGui::PopItemWidth();
            ImGui::PopID();
        }

        if (deleteIndex >= 0) palette.erase(palette.begin() + deleteIndex);
        if (sortArray) std::sort(palette.begin(), palette.end(), [](ColourPaletteItem a, ColourPaletteItem b){ return a.targetHue < b.targetHue; });

        ImGui::EndTable();
    }

    if (ImGui::Button("Close", ImVec2(-1.0f, 0.0f))) { ImGui::CloseCurrentPopup(); }
}
