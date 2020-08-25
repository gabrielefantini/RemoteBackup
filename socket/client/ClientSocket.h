//
// Created by mattia on 21/08/20.
//

#ifndef CLIENT_CLIENTSOCKET_H
#define CLIENT_CLIENTSOCKET_H

#include <stdio.h>
#include <stdlib.h>

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
    int sendFile(const char *name) {
        int len = 0;
        int j = 0;
        /// calcolo il numero di elementi del char*
        while(name[j]!='\0') {len++;j++;}
        /// spazio per \0
        len++;
        std::cout << "Name: " << name << " , lunghezza: "<<len << std::endl;
        /// variabile per scrivere la lunghezza come char*, valore accettato dalla write
        char len_char[4];
        /// conversione int -> char per la write
        sprintf(len_char,"%d",len);
        std::cout << "len_char: " << len_char << std::endl;
        /// PRIMA WRITE: invio la dimensione del titolo
        int bytes_written = write(len_char, sizeof(len_char), 0);
        if (bytes_written == -1) {
            perror("Error while sending name");
            exit(EXIT_FAILURE);
        }

        /// SECONDA WRITE: invio il titolo
        bytes_written = write(name, len, 0);
        std::cout << "Dopo la write del nome ho scritto: " << bytes_written << " bytes" << std::endl;
        if (bytes_written == -1) {
            perror("Error while sending name");
            exit(EXIT_FAILURE);
        }
        std::cout << "Nome spedito, byte inviati: "<< bytes_written << std::endl;
        int bytes_read;
        char buffer[1024];
        /// apro il file in lettura e invio il contenuto
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
                std::cout << "Write operation status:" << strerror(errno) << "; bytes sent: " << bytes_written;
            } while(!feof(fs) && bytes_written != -1);
        }
        return bytes_written;
    }
};


#endif //CLIENT_CLIENTSOCKET_H
