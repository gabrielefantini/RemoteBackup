//
// Created by mattia on 21/08/20.
//

#include "ServerSocket.h"

ServerSocket::ServerSocket(int port)  {
    // il ServerSocket deve essere in grado di accettare connessioni sulla porta passata, alloco un oggetto di tipo sockaddr_in
    struct sockaddr_in sockaddrIn;
    sockaddrIn.sin_port = htons(port);
    sockaddrIn.sin_family = AF_INET;
    ///sockaddrIn.sin_len = sizeof(sockaddrIn);
    sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);

    if (::bind(sockfd, reinterpret_cast<struct sockaddr*>(&sockaddrIn), sizeof(sockaddrIn)) != 0)
        throw std::runtime_error("Cannot bind port");
    else
        std::cout << "bind ok" << std::endl;

    // il socket viene messo in modalità passiva sulla porta
    /// 8 -> fino a 8 connessioni possono essere messe in sospeso
    if (::listen(sockfd, 8) != 0)
        throw std::runtime_error("Error listen");
    else
        std::cout << "listen ok" << std::endl;
}

Socket ServerSocket:: accept (struct sockaddr_in* addr, unsigned int* len) {
    int fd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), len);
    if (fd < 0) throw std::runtime_error("Cannot accept socket");
    return Socket(fd);
}
