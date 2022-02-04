#include "Socket.hpp"

Socket::Socket()
{
    setupWSA();
}

Socket::Socket(const char* ip, int port)
{
    setupWSA();
    createSocket(ip, port);
}

Socket::~Socket()
{
    closesocket(m_Socket);
    WSACleanup();
}

void Socket::resetIp(const char* ip, int port)
{
    closesocket(m_Socket);
    createSocket(ip, port);
}

void Socket::sendData(uint8_t* buffer, int size)
{
    int sendOK = sendto(m_Socket, (char*)buffer, size, 0, (sockaddr*)&m_Server, sizeof(m_Server));
    if (sendOK == SOCKET_ERROR)
        std::cout << "Failed to send: " << WSAGetLastError() << "\n";
}

void Socket::createSocket(const char* ip, int port)
{
    m_Server.sin_family = AF_INET;
    m_Server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &m_Server.sin_addr);

    m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
}

void Socket::setupWSA()
{
    WORD version = MAKEWORD(2, 2);
    int wsOK = WSAStartup(version, &m_Data);

    if (wsOK != 0)
    {
        std::cout << "Can't start winsock: " << wsOK << "\n";
        exit(-1);
    }
}
