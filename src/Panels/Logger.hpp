#ifndef PANELS_LOGGER_HPP
#define PANELS_LOGGER_HPP

#include "../ImGui/ImGuiPanel.hpp"

enum class LoggerType
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

// Apply printf style formatting
#define FMTARGS(FMT) __attribute__((format(gnu_printf, FMT, FMT+1)))

class Logger : public ImGuiPanel
{
private:
    static ImGuiTextBuffer m_Buf;
    static ImGuiTextFilter m_Filter;
    static ImVector<int> m_LineOffsets;
    static bool m_AutoScroll;
public:
    Logger();
    ~Logger() = default;
 
    static void clear();
    static void log(LoggerType type, const char* fmt, ...) FMTARGS(2);
    void renderImGui();
};

#endif
