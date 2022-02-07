#include "Logger.hpp"

#include <iostream>

ImGuiTextBuffer Logger::m_Buf;
ImGuiTextFilter Logger::m_Filter;
ImVector<int> Logger::m_LineOffsets;
bool Logger::m_AutoScroll = true;

Logger::Logger()
{
    clear();
}

void Logger::clear()
{
    m_Buf.clear();
    m_LineOffsets.clear();
    m_LineOffsets.push_back(0);
}

void Logger::log(LoggerType type, const char* fmt, ...)
{
#ifdef DEBUG_MODE
    int oldSize = m_Buf.size();
    std::string newString;
    switch (type)
    {
    case LoggerType::LOG_INFO:  newString += "[info] ";     break;
    case LoggerType::LOG_WARN:  newString += "[warn] ";     break;
    case LoggerType::LOG_ERROR: newString += "[error] ";    break;
    }
    newString += fmt;

    va_list args;
    va_start(args, newString.c_str());
    m_Buf.appendfv(newString.c_str(), args);
    va_end(args);

    for (int newSize = m_Buf.size(); oldSize < newSize; oldSize++)
    {
        if (m_Buf[oldSize] == '\n')
            m_LineOffsets.push_back(oldSize + 1);
    }
#endif
}

void Logger::renderImGui()
{
#ifdef DEBUG_MODE
    if (ImGui::Begin("Logger"))
    {
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        m_Filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Logger::clear();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = m_Buf.begin();
        const char* bufEnd = m_Buf.end();

        if (m_Filter.IsActive())
        {
            for (int lineNo = 0; lineNo < m_LineOffsets.Size; lineNo++)
            {
                const char* lineStart = buf + m_LineOffsets[lineNo];
                const char* lineEnd = (lineNo + 1 < m_LineOffsets.Size) ? (buf + m_LineOffsets[lineNo + 1] - 1) : bufEnd;
                if (m_Filter.PassFilter(lineStart, lineEnd))
                    ImGui::TextUnformatted(lineStart, lineEnd);
            }
        }
        else
        {
            ImGuiListClipper clipper;

            clipper.Begin(m_LineOffsets.Size);
            while (clipper.Step())
            {
                for (int lineNo = clipper.DisplayStart; lineNo < clipper.DisplayEnd; lineNo++)
                {
                    const char* lineStart = buf + m_LineOffsets[lineNo];
                    const char* lineEnd = (lineNo + 1 < m_LineOffsets.Size) ? (buf + m_LineOffsets[lineNo + 1] - 1) : bufEnd;
                    ImGui::TextUnformatted(lineStart, lineEnd);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
        
        ImGui::EndChild();
    }
    ImGui::End();
#endif
}
