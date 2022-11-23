#pragma once

#include <iostream>
#include <vector>

#include "EffectManager.hpp"

enum class CommandEnum : uint8_t
{
    CHANGE_EFFECT
};

class Console
{
public:
    static void init(EffectManager* effectManager);

    static void start();

    static void printCommands();
    static void runCommand(CommandEnum command);

    static void printLine(const std::string& msg);

    static size_t printOptions(const std::vector<const char*>& options);

private:

    static std::vector<const char*> s_Commands;

    static EffectManager* s_EffectManager;
};
