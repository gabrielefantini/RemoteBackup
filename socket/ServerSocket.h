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
        /// 8 -> fino a 8 connessioni possono essere messe in sospeso
        if (::listen(sockfd, 8) != 0)
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

    /// solo il server riceve file
    void receiveFile() {
        std::cout << "receiveFile called" << std::endl;
        char buffer[1024];
        ssize_t bytes_read = 0;
        /// read di Socket.h
        FILE* fr = fopen("prova.txt", "w");
        while((bytes_read = read(buffer, sizeof(buffer)-1, 0)) > 0)
        {
            std::cout << "Dentro al while" << std::endl;
            printf("%d bytes read\n", bytes_read);
            fwrite(buffer, sizeof(char), bytes_read, fr);
            if(ferror(fr))
            {
                perror("Error while writing to file");
                fclose(fr);
                exit(EXIT_FAILURE);
            }
        }
        buffer[-1] = 0;
        if(bytes_read == -1)
            printf("%s\n", strerror(errno));
    }

};


#endif //SOCKET_SERVERSOCKET_H
