#if defined(__linux__)
#ifndef LINUX_SOCKET_HPP
#define LINUX_SOCKET_HPP

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "../SocketBase.hpp"

class Socket : public SocketBase
{
public:
    Socket();
    Socket(const char* ip, int port);
    ~Socket() override;

    void resetIp(const char* ip, int port) override;
    void sendData(uint8_t* buffer, int size) override;
private:
    int m_Sockfd;
    sockaddr_in m_Server;
    bool m_Started = false;
private:
    void createSocket(const char* ip, int port);
};

#endif
#endif
