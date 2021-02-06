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


#include <boost/asio/deadline_timer.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::asio;

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
    std::map<std::string, std::string> clientMap;

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

    int askFile(char* path, std::string hash, std::string &dir);
    bool sendOk();
    /// solo il server riceve file
    /// inserita qui perché ritorno un socket e non un serversocket con la accept
    int receiveFile(std::string name,std::string &dir);

    //bool vrfy();
    int setupDir(const std::string &path,std::string &client,int index);

    int ResToNotify();

    std::string get_cur_client(){
        return std::string(cur_client);
    }

    std::string get_cur_dir(){
        return std::string(cur_dir);
    }

    std::map<std::string, std::string> get_clientMap(){
        return clientMap;
    }

    std::map<std::string, std::string> db_password {{"client1", "password1"}, {"client2", "password2"}};
};


#endif //SOCKET_SOCKET_H

