#include "Logger.hpp"

#include <iostream>

#include <cstdarg>

Logger::Logger() { }


void Logger::log(LoggerType type, const char* fmt, ...)
{
// #ifdef DEBUG_MODE
    // int oldSize = m_Buf.size();
    std::string tag;
    switch (type)
    {
    case LoggerType::LOG_INFO:  tag = "INFO";     break;
    case LoggerType::LOG_WARN:  tag = "WARN";     break;
    case LoggerType::LOG_ERROR: tag = "ERROR";    break;
    }

    const char* fmtTemplate = "[%s] %s\n";

    va_list args;
    
    size_t length = snprintf(NULL, 0, fmtTemplate, tag.c_str(), fmt);
    char* newFmt = (char*)malloc(length + 1);
    snprintf(newFmt, length + 1, fmtTemplate, tag.c_str(), fmt);

    va_start(args, fmt);
    vprintf(newFmt, args);
    // m_Buf.appendfv(newFmt, args);
    va_end(args);

    // std::cout << ""

    // for (int newSize = m_Buf.size(); oldSize < newSize; oldSize++)
    // {
    //     if (m_Buf[oldSize] == '\n')
    //         m_LineOffsets.push_back(oldSize + 1);
    // }

    // std::cout << ""
// #endif
}
