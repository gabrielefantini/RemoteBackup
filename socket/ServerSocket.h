//
// Created by mattia on 21/08/20.
//

#ifndef SOCKET_SERVERSOCKET_H
#define SOCKET_SERVERSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "Socket.h"

// dichiarata friend di socket, devo chiamare il suo costruttore privato
// ereditarietà privata, necessario?
class ServerSocket: private Socket {
public:
    // quando viene invocato questo costruttore chiama implicitamente il costruttore di Socket (superclasse privata),
    // che crea un oggetto che finisce nel campo sockfd
    ServerSocket(int port) {
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
        if (::listen(sockfd, 0) != 0)
            throw std::runtime_error("Error listen");
        else
            std::cout << "listen ok" << std::endl;
    }

    // accept mi dà la possibilità di avere un socket per chi vuole connettersi
    Socket accept (struct sockaddr_in* addr, unsigned int* len) {
        int fd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), len);
        if (fd < 0) throw std::runtime_error("Cannot accept socket");
        return Socket(fd);
    }

};


#endif //SOCKET_SERVERSOCKET_H
