#pragma once

#include <iostream>
#include <vector>

namespace Console
{
    void printCommands();

    void printLine(const std::string& msg);

    size_t printOptions(const std::vector<const char*>& options);
}
