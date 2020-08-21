//
// Created by mattia on 21/08/20.
//

#ifndef CLIENT_CLIENTSOCKET_H
#define CLIENT_CLIENTSOCKET_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include "../socket/Socket.h"

class ClientSocket: public Socket {
public:
    ClientSocket(int port) {
        std::cout << "Creazione client socket" << std::endl;
        struct sockaddr_in sockaddrIn;
        sockaddrIn.sin_family = AF_INET;
        std::cout << "dopo af" << std::endl;
        sockaddrIn.sin_addr.s_addr = inet_addr("127.0.0.1");
        sockaddrIn.sin_port = htons(port);
        // specifico il server con cui voglio comunicare

        std::cout << "Client socket creato" << std::endl;
        if (::connect(sockfd, reinterpret_cast<struct sockaddr*>(&sockaddrIn), sizeof(sockaddrIn))!=0)
            throw std::runtime_error("Cannot connect to remote socket");
    }
};


#endif //CLIENT_CLIENTSOCKET_H
