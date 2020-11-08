//
// Created by mattia on 21/08/20.
//

#ifndef SOCKET_SERVERSOCKET_H
#define SOCKET_SERVERSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "Socket.h"

// dichiarata friend di socket, devo chiamare il suo costruttore privato
// ereditarietà privata, necessario?
class ServerSocket: private Socket {
public:
    // quando viene invocato questo costruttore chiama implicitamente il costruttore di Socket (superclasse privata),
    // che crea un oggetto che finisce nel campo sockfd
    ServerSocket(int port);

    // accept mi dà la possibilità di avere un socket per chi vuole connettersi
    Socket accept (struct sockaddr_in* addr, unsigned int* len);
};


#endif //SOCKET_SERVERSOCKET_H