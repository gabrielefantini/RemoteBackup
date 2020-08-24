//
// Created by mattia on 21/08/20.
//

#ifndef CLIENT_CLIENTSOCKET_H
#define CLIENT_CLIENTSOCKET_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
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
    // funzione per inviare al server il nuovo hash
    /// questa funzione potrebbe anche essere sostituita da una semplice write
    void sendHash(char* buffer) {
        /// per ora options = 0, da definire
        this->write(buffer, sizeof(buffer), 0);
    }


    /// solo il client invia file, funzione inserita in ClientSocket per questo
    int sendFile(char* name) {
        int bw = write(name, sizeof(name)+sizeof("\0"), 0);
        if (bw == -1) {
            perror("Error while sending name.");
            exit(EXIT_FAILURE);
        }
        std::cout << "Nome spedito, byte inviati: "<< bw << std::endl;
        std::cout << "Sizeof(nome): "<< sizeof(name) << std::endl;
        // così poi non posso più inviare nulla sul socket
        //bw = write("\0", sizeof("\0"), 0);
        int bytes_read, bytes_written;
        char buffer[1024];
        FILE* fs = fopen(name, "r");
        if (fs) {
            do {
                std::cout << "Leggo da file" << std::endl;
                bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fs);
                if(ferror(fs))
                {
                    perror("Error while reading from file.");
                    fclose(fs);
                    exit(EXIT_FAILURE);
                }
                /// per ora options = 0, è la write implementata in Socket.h
                std::cout << "Scrivo al socket" << std::endl;
                bytes_written = write(buffer, bytes_read, 0);
                printf("Write operation status: %s; bytes sent: %d\n", strerror(errno), bytes_written);
            } while(!feof(fs) && bytes_written != -1);
        }
        return bytes_written;

    }
};


#endif //CLIENT_CLIENTSOCKET_H
