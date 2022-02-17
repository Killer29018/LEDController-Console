#if defined(__linux__)
#include "Socket.hpp"

Socket::Socket() {}
Socket::Socket(const char* ip, int port) {}
Socket::~Socket() {}

void Socket::resetIp(const char* ip, int port) {}
void Socket::sendData(uint8_t* buffer, int size) {}
#endif
