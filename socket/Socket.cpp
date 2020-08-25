//
// Created by mattia on 21/08/20.
//

#include "Socket.h"

void Socket::receiveFile() {
    int bytes_received;
    char len[4];
    /// PRIMA READ: leggo la dimensione del titolo
    bytes_received = read(len, sizeof(len), 0);
    if (bytes_received == -1) {
        perror("Error while reading size");
        exit(EXIT_FAILURE);
    }
    /// atoi è una funzione che mi permette di convertire un char in un intero
    std::cout << atoi(len) << std::endl;
    /// alloco una struttura per contenere il nome della lunghezza del nome
    char name[atoi(len)];
    /// SECONDA READ: leggo il nome
    bytes_received = read(name, sizeof(name), 0);
    if (bytes_received == -1) {
        perror("Error while reading size");
        exit(EXIT_FAILURE);
    }
    /// inserisco il terminatore di stringa come ultimo carattere del titolo
    name[atoi(len)-1] = '\0';

    std::cout << name << std::endl;
    /// apro il file in modalità "w": se non esiste lo crea, se esiste ne sovrascrive il contenuto
    FILE* fr = fopen(name, "w");

    /// TERZA READ: leggo il contenuto del file ricevuto e lo scrivo nel file appena creato
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
    /// chiudo il file
    fclose(fr);
}

void Socket::connect(struct sockaddr_in *addr, unsigned int len)  {
    if (::connect(sockfd, reinterpret_cast<struct sockaddr*>(addr), len)!=0)
        throw std::runtime_error("Cannot connect to remote socket");
}

ssize_t Socket::write(const char *buffer, size_t len, int options)  {
    ssize_t res = send(sockfd, buffer, len, options);
    if (res < 0) throw std::runtime_error("Cannot write to socket");
    return res;
}

ssize_t Socket::read(char *buffer, size_t len, int options)  {
    std::cout << "read called" << std::endl;
    ssize_t res = recv(sockfd, buffer, len, options);
    if (res < 0) {
        std::cout << strerror(errno) << std::endl;
        throw std::runtime_error("Cannot receive from socket");
    }
    return res;
}
