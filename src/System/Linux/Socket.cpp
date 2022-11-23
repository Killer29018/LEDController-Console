#if defined(__linux__)
#include "Socket.hpp"

#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../Utils/Logger.hpp"

Socket::Socket() {}
Socket::Socket(const char* ip, int port)
{
    createSocket(ip, port);
}

Socket::~Socket()
{
    close(m_Sockfd);
}

void Socket::resetIp(const char* ip, int port) 
{
    if (m_Started) close(m_Sockfd);

    createSocket(ip, port);
}

void Socket::sendData(uint8_t* buffer, int size)
{
    if (send(m_Sockfd, buffer, size, 0) < 0)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to write to socket");
    }
}

void Socket::createSocket(const char* ip, int port)
{
    m_Sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_Sockfd == -1)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to create socket");
        exit(-1);
    }

    m_Server.sin_family = AF_INET;
    m_Server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &m_Server.sin_addr);

    if (connect(m_Sockfd, (sockaddr*)&m_Server, sizeof(m_Server)) < 0)
    {
        Logger::log(LoggerType::LOG_ERROR, "Failed to connect");
        exit(-1);
    }

    Logger::log(LoggerType::LOG_INFO, "Connected to %s : port %d", ip, port);

    m_Started = true;
}

#endif
