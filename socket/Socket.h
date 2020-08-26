//
// Created by mattia on 21/08/20.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <zconf.h>
#include <cstring>
#include <map>

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

    /// questa lista la vorrei mettere in ServerSocket ma la funzione per verificare il client va messa in Socket
    /// TODO: ragionare sull'utilità di ServerSocket
    std::map<char*, char*> client_dir;
    char* cur_client = nullptr;
    char* cur_dir = nullptr;

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
    ssize_t read(char* buffer, size_t len, int options);

    ssize_t write(const char* buffer, size_t len, int options);

    // sockaddr_in va bene per IP_v4
    /// per ora non usata
    void connect(struct sockaddr_in *addr, unsigned int len);

    /// solo il server riceve file
    /// inserita qui perché ritorno un socket e non un serversocket con la accept
    void receiveFile();

    bool vrfy();
};


#endif //SOCKET_SOCKET_H
