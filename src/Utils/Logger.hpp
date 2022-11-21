#ifndef PANELS_LOGGER_HPP
#define PANELS_LOGGER_HPP

enum class LoggerType
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
};

// Apply printf style formatting
#define FMTARGS(FMT) __attribute__((format(gnu_printf, FMT, FMT+1)))

class Logger
{
private:
public:
    Logger();
    ~Logger() = default;
 
    static void log(LoggerType type, const char* fmt, ...) FMTARGS(2);
    void renderImGui();
};

#endif
