#pragma once

#include <iostream>
#include <vector>

#include "EffectManager.hpp"
#include "System/Socket.hpp"

enum class CommandEnum : uint8_t
{
    CHANGE_EFFECT,
    CHANGE_IP,
    CHANGE_PORT,
};

class Console
{
public:
    static void init(EffectManager* effectManager, Socket* socket,
            std::string ip, int port);

    static void start();

    static void printCommands();
    static void runCommand(CommandEnum command);

    static void printLine(const std::string& msg);

    static size_t printOptions(const std::vector<const char*>& options);

private:
    static std::vector<const char*> s_Commands;

    static EffectManager* s_EffectManager;
    static Socket* s_Socket;

    static int s_Port;
    static std::string s_Ip;
};
