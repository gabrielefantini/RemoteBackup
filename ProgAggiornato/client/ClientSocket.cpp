//
// Created by mattia on 21/08/20.
//

#include <nlohmann/json.hpp>
#include "ClientSocket.h"
#include "../socket/Socket.h"
#include "jsonUtils.h"
using json = nlohmann::json;

int get_file_size(std::string filename) // path to file
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

int ClientSocket::sendFile(const char *name) {
    int len = get_file_size(name);
    char len_char[10];
    /// conversione int -> char per la write
    sprintf(len_char,"%d",len);
    std::cout << "len_char: " << len_char << std::endl;
    /// PRIMA WRITE: invio la dimensione del titolo
    int bytes_written = write(len_char, sizeof(len_char), 0);
    if (bytes_written == -1) {
        perror("Error while sending name");
        //exit(EXIT_FAILURE);
        return -1;
    }

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
                //exit(EXIT_FAILURE);
                return -1;
            }
            /// per ora options = 0, è la write implementata in Socket.h
            std::cout << "Scrivo al socket" << std::endl;
            bytes_written = write(buffer, bytes_read, 0);
            std::cout << "Write operation status:" << strerror(errno) << "; bytes sent: " << bytes_written;
            if (bytes_written == -1) {
                perror("Error while sending file");
                //exit(EXIT_FAILURE);
                return -1;
            }
        } while(!feof(fs) && bytes_written != -1);
    }

    return bytes_written;
}
/*
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
}
*/
int ClientSocket::notify(std::string n, std::string d, std::map<std::string,std::string> localMap) {
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
    if (bytes_written == -1) {
        perror("Error while sending name dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }

    /// SECONDA WRITE: nome

    bytes_written = write(name, len, 0);
    if (bytes_written == -1) {
        perror("Error while sending name");
        //exit(EXIT_FAILURE);
        return -1;
    }
    len = 0; j = 0;
    while(dir[j]!='\0') {len++;j++;}
    len++;

    std::cout << "Spediti " << bytes_written << " bytes, inviato: " << name << std::endl;
    /// TERZA WRITE: dimensione dir
    sprintf(len_char,"%d",len);

    bytes_written = write(len_char, sizeof(len_char), 0);
    if (bytes_written == -1) {
        perror("Error while sending dir dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    /// QUARTA WRITE: dir
    bytes_written = write(dir, len, 0);
    if (bytes_written == -1) {
        perror("Error while sending dir");
        //exit(EXIT_FAILURE);
        return -1;
    }
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
    if (bytes_written == -1) {
        perror("Error while sending localMap dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    /// controllo fatto da write

    /// SECONDA WRITE: localMap

    bytes_written = write(char_map, len, 0);
    if (bytes_written == -1) {
        perror("Error while sending localMap");
        //exit(EXIT_FAILURE);
        return -1;
    }
    std::cout << "--- Mappa inviata ---" << std::endl;
    for (auto x : localMap)
        std::cout << x.first << ", " << x.second << std::endl;


    std::string msg = "ok";
    char* ok = const_cast<char *>(msg.c_str());
    ///ULTIMA WRITE -> OK
    len = 0; j = 0;
    while(ok[j]!='\0') {len++;j++;}
    len++;

    sprintf(len_char,"%d",len);

    bytes_written = write(len_char, sizeof(len_char), 0);
    /// controllo fatto da write
    if (bytes_written == -1) {
        perror("Error while sending OK dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    bytes_written = write(ok, len, 0);
    if (bytes_written == -1) {
        perror("Error while sending OK");
        //exit(EXIT_FAILURE);
        return -1;
    }
    char len_res[4];
    int bytes_received = read(len_res, sizeof(len_res), 0);
    if(bytes_received == -1) {
        perror("Error while reading from server");
        //exit(EXIT_FAILURE);
        return -1;
    }
    char res[atoi(len_res)];
    bytes_received = read(res, sizeof(res), 0);
    if(bytes_received == -1) {
        perror("Error while reading from server");
        //exit(EXIT_FAILURE);
        return -1;
    }
    res[atoi(len_res)-1] = '\0';
    std::cout << "Messaggio ricevuto: " << res << std::endl;
    /// tutto ok
    return 1;
}

int ClientSocket::WaitForSendingFile() {
    while(1) {
        // all'inizio di ogni attesa devo settare un timer:
        // se alla fine del timer non ricevo nulla annullo
        // l'aggiornamento chiudendo il socket e setto la variabile
        // di aggiornamento fallito a 1
        std::cout<<"waitforfile"<<std::endl;
        int bytes_received;
        char len_name[4];
        bytes_received = read(len_name, sizeof(len_name), 0);
        if(bytes_received == -1) {
            perror("Error while reading from server");
            //exit(EXIT_FAILURE);
            return -1;
        }
        std::cout <<"Dimensione nome client: " <<  atoi(len_name) << std::endl;
        char name[atoi(len_name)];
        bytes_received = read(name, sizeof(name), 0);
        if(bytes_received == -1) {
            perror("Error while reading from server");
            //exit(EXIT_FAILURE);
            return -1;
        }
        name[atoi(len_name)-1] = '\0';

        /// CONTROLLO, se è "ok" si esce dal ciclo
        std::string ok = "ok";
        if (std::string(name) == ok) {
            std::cout << "Il server non ha bisogno di ulteriori file" << std::endl;
            return 1;
        }

        int operationResult = sendFile(name);
        if(operationResult == -1){
            perror("Error while sending file");
            //exit(EXIT_FAILURE);
            return -1;
        };
    }
}