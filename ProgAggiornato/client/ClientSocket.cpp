//
// Created by mattia on 21/08/20.
//

#include <nlohmann/json.hpp>
#include "ClientSocket.h"
#include "../socket/Socket.h"
#include "jsonUtils.h"
using json = nlohmann::json;

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
    /// forse controlli inutili, li fa la funzione write...
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

int ClientSocket::sendHash(char *buffer) {
    /// per ora options = 0, da definire
    int bytes_written = write(buffer, sizeof(buffer), 0);
    return bytes_written;
}

bool ClientSocket::auth(char* name, char* dir) {
    int len = 0;
    int j = 0;

    /// calcolo il numero di elementi del char*
    while(name[j]!='\0') {len++;j++;}
    len++;

    char len_char[4];
    /// conversione int -> char per la write
    sprintf(len_char,"%d",len);

    /// PRIMA WRITE: invio la dimensione del nome
    int bytes_written;
    bytes_written = write(len_char, sizeof(len_char), 0);
    /// controllo fatto da write

    /// SECONDA WRITE: nome

    bytes_written = write(name, len, 0);
    len = 0; j = 0;
    while(dir[j]!='\0') {len++;j++;}
    len++;


    std::cout << "Spediti " << bytes_written << " bytes, inviato: " << name << std::endl;

    /// TERZA WRITE: dimensione dir
    sprintf(len_char,"%d",len);
    bytes_written = write(len_char, sizeof(len_char), 0);
    /// QUARTA WRITE: dir
    bytes_written = write(dir, len, 0);
    std::cout << "Spediti " << bytes_written << " bytes" << "\n" << "Attesa risposta..." << std::endl;

    /// Aspetto risposta cartella presente o no
    /*
     * dir1
     *      a   -> dir1/a
     *          file1           ->
     *      b   -> dir1/b
     *
     */


}

bool ClientSocket::notify(std::string n, std::string d, std::map<std::string,std::string> localMap) {
    int len = 0;
    int j = 0;
    char* name = const_cast<char *>(n.c_str());
    char* dir = const_cast<char *>(d.c_str());
    /// calcolo il numero di elementi del char*
    while(name[j]!='\0') {len++;j++;}
    len++;

    char len_char[4];
    /// conversione int -> char per la write
    sprintf(len_char,"%d",len);

    /// PRIMA WRITE: invio la dimensione del nome
    int bytes_written;
    bytes_written = write(len_char, sizeof(len_char), 0);
    /// controllo fatto da write

    /// SECONDA WRITE: nome

    bytes_written = write(name, len, 0);
    len = 0; j = 0;
    while(dir[j]!='\0') {len++;j++;}
    len++;


    std::cout << "Spediti " << bytes_written << " bytes, inviato: " << name << std::endl;

    /// TERZA WRITE: dimensione dir
    sprintf(len_char,"%d",len);
    bytes_written = write(len_char, sizeof(len_char), 0);
    /// QUARTA WRITE: dir
    bytes_written = write(dir, len, 0);
    std::cout << "Spediti " << bytes_written << " bytes" << "\n" << "Attesa risposta..." << std::endl;

    /// SEND MAP
    // 1. ottengo un char* per localMap

    json json_map = json();
    to_json(json_map,localMap);
    std::cout << "Json created" << std::endl;

    std::string string_map = json_map.dump(-1, ' ', false, json::error_handler_t::replace);
    std::cout << "String created" << std::endl;
    char* char_map = const_cast<char *>(string_map.c_str());

    len = 0; j = 0;
    while(char_map[j]!='\0') {len++;j++;}
    len++;

    /// TERZA WRITE: dimensione dir
    sprintf(len_char,"%d",len);

    /// PRIMA WRITE: invio la dimensione del localMap
    bytes_written = write(len_char, sizeof(len_char), 0);
    /// controllo fatto da write

    /// SECONDA WRITE: localMap

    bytes_written = write(char_map, len, 0);

    std::cout << "--- Mappa inviata ---" << std::endl;
    for (auto x : localMap)
        std::cout << x.first << ", " << x.second << std::endl;
}
