//
// Created by mattia on 21/08/20.
//

#include "ClientSocket.h"
#include "../socket/Socket.h"

int ClientSocket::sendFile(const char *name) {
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

void ClientSocket::sendHash(char *buffer) {
    /// per ora options = 0, da definire
    this->write(buffer, sizeof(buffer), 0);
}
