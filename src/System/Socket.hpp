#ifndef SYSTEM_SOCKET_HPP
#define SYSTEM_SOCKET_HPP

#include "SocketBase.hpp"

#ifdef _WIN32
    #include "Windows/Socket.hpp"
#elif __linux__
    #include "Linux/Socket.hpp"
#endif

#endif
