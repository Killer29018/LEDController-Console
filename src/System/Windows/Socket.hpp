#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class Socket
{
public:
    sockaddr_in m_Server;
    SOCKET m_Socket;
    WSADATA m_Data;
private:
public:
    Socket();
    Socket(const char* ip, int port);
    ~Socket();

    void resetIp(const char* ip, int port);

    void sendData(uint8_t* buffer, int size);
private:
    void createSocket(const char* ip, int port);
    void setupWSA();
};

#endif
