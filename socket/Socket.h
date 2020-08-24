//
// Created by mattia on 21/08/20.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <zconf.h>
#include <cstring>

// paradigma RAII: rilascio corretto delle risorse
class Socket {
    // file descriptor del socket
    int sockfd;

    // costruttore privato con file descriptor
    Socket(int fd) : sockfd(fd) {
        std::cout << "Socket " << sockfd << " created" << std::endl;
    }

    // costruttore di copia e operatore di assegnazione delete -> il file descriptor non deve essere duplicato
    Socket(const Socket&) = delete;
    Socket &operator=(const Socket&) = delete;

    friend class ServerSocket;
    friend class ClientSocket;
public:
    Socket() {
        sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        // con valore di ritorno negativo c'è stato un problema -> lancio eccezione
        if (sockfd < 0) throw std::runtime_error("Cannot create socket");
        std::cout << "Socket " << sockfd << " created" << std::endl;
    }

    ~Socket() {
        if (sockfd != 0) {
            std::cout << "Socket " << sockfd << " closed" << std::endl;
            close(sockfd);
        }
    }

    // costruttore di movimento e operatore di assegnazione per movimento
    Socket(Socket &&other) : sockfd(other.sockfd) {
        other.sockfd = 0;
    }

    Socket &operator=(Socket &&other) {
        if (sockfd != 0) close(sockfd);
        sockfd = other.sockfd;
        other.sockfd = 0;
        return *this;
    }

    // il file descriptor del socket è privato, dall'esterno non potrei leggere e scrivere dei byte
    ssize_t read(char* buffer, size_t len, int options) {
        std::cout << "read called" << std::endl;
        ssize_t res = recv(sockfd, buffer, len, options);
        if (res < 0) {
            std::cout << strerror(errno) << std::endl;
            throw std::runtime_error("Cannot receive from socket");
        }
        return res;
    }

    ssize_t write(const char* buffer, size_t len, int options) {
        ssize_t res = send(sockfd, buffer, len, options);
        if (res < 0) throw std::runtime_error("Cannot write to socket");
        return res;
    }

    // sockaddr_in va bene per IP_v4
    void connect(struct sockaddr_in *addr, unsigned int len) {
        if (::connect(sockfd, reinterpret_cast<struct sockaddr*>(addr), len)!=0)
            throw std::runtime_error("Cannot connect to remote socket");
    }

    /// solo il server riceve file
    void receiveFile() {
        char name[1024];

        int br, tot = 0;

        while((br = read(name, sizeof(name)-1, 0)) > 0) {
            std::cout << "Br durante: " << br <<std::endl;
            int written = write(name, br, 0);
            tot+=written;
        }
        std::cout << "Tot: " << tot << std::endl;
        name[tot]=0;

        std::cout << "Name: " << name << std::endl;
        std::cout << "receiveFile called" << std::endl;
        FILE* fr = fopen(name, "w");

        char buffer[1024];
        ssize_t bytes_read = 0;
        /// read di Socket.h
        while((bytes_read = read(buffer, sizeof(buffer)-1, 0)) > 0) {
            std::cout << "Dentro al while" << std::endl;
            printf("%d bytes read\n", bytes_read);
            std::cout<< "----------------------" <<std::endl;

            int written = fwrite(&buffer, sizeof(char), bytes_read, fr);
            if(ferror(fr))
            {
                perror("Error while writing to file");
                fclose(fr);
                exit(EXIT_FAILURE);
            }
            std::cout << "Written: " << written << " bytes" << std::endl;
        }
        buffer[bytes_read] = 0;
        if(bytes_read == -1)
            printf("%s\n", strerror(errno));
        std::cout << "receive finita" << std::endl;
        fclose(fr);
    }
};


#endif //SOCKET_SOCKET_H