//
// Created by mattia on 21/08/20.
//

#include "Socket.h"
#include <boost/filesystem.hpp>

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

bool Socket::vrfy() {
    int bytes_received;
    char len_name[4];

    // nome client

    /// PRIMA READ: leggo la dimensione del nome
    bytes_received = read(len_name, sizeof(len_name), 0);
    /// atoi è una funzione che mi permette di convertire un char in un intero
    std::cout <<"Dimensione nome client: " <<  atoi(len_name) << std::endl;
    /// alloco una struttura per contenere il nome della lunghezza del nome
    char name[atoi(len_name)];
    /// SECONDA READ: leggo il nome
    bytes_received = read(name, sizeof(name), 0);
    /// inserisco il terminatore di stringa come ultimo carattere del titolo
    name[atoi(len_name)-1] = '\0';
    std::cout << "Nome client: " << name << std::endl;

    // nome dir

    char len[4];
    /// PRIMA READ: leggo la dimensione del nome
    bytes_received = read(len, sizeof(len), 0);
    /// atoi è una funzione che mi permette di convertire un char in un intero
    std::cout << "Dimensione nome dir: " << atoi(len) << std::endl;
    /// alloco una struttura per contenere il nome della lunghezza del nome
    char dir[atoi(len)];
    /// SECONDA READ: leggo il nome
    bytes_received = read(dir, sizeof(dir), 0);
    /// inserisco il terminatore di stringa come ultimo carattere del titolo
    dir[atoi(len)-1] = '\0';
    std::cout << "Nome dir: " << dir << std::endl;

    /*
    for (auto elem : client_dir) {
        std::cout << "Scansione lista client..." << std::endl;
        if (strcmp(elem.first, name) == 0) {
            cur_client = name;
            cur_dir = dir;
            std::cout << "Bentornato " << cur_client <<", sto controllando "<< cur_dir << std::endl;
        }
    }

    /// nuovo client
    if (cur_client == nullptr) {
        std::cout << "Nuovo client!" << std::endl;
        std::pair<char*, char*> elem = {name, dir};
        client_dir.insert(elem);
        cur_client = name;
        cur_dir = dir;
        std::cout << "Nuovo utente! Benvenuto " << cur_client <<", controllerò "<< cur_dir << std::endl;
    }
    */
    int len_path = atoi(len)-1+atoi(len_name);
    std::cout << "Len_path: " << len_path << std::endl;
    char path_complessivo[len_path];

    for (int i = 0; i<atoi(len_name); i++) path_complessivo[i] = name[i];
    path_complessivo[atoi(len_name)-1] = '/';
    for (int i = 0; i<atoi(len); i++) path_complessivo[i+atoi(len_name)] = dir[i];
    std::cout << "Percorso creato: " << path_complessivo << std::endl;

    if (Socket::setup_dir(path_complessivo)) {
        std::cout << "Utente riconosciuto e directory esistente! Bentornato " << name << std::endl;

    }
    else {
        std::cout << "Ciao " << name <<"! Ho aggiunto " << dir <<" alle directory controllate" << std::endl;
    }

    cur_client = name;
    cur_dir = dir;

    //TODO: per ora restituisco sempre true, ci sono casi in cui devo restituire false? Possono esserci errori?
    return true;
}

// da mainUtil.h
namespace fs=boost::filesystem;
//setup_dir: cerca se la directory relativa al path di un certo utente esiste già (return 1), altrimenti la crea (return 0)
int Socket::setup_dir(const std::string &path){
    std::string new_path = std::string("../Backup/")+path;
    if(fs::exists(new_path) && fs::is_directory(new_path)) {
        std::cout << new_path << " found in backups.\n\n";
        return 1;
    }else{
        std::cout<<path<<" missing!\n";
        fs::create_directory(std::string("../Backup/")+fs::path(path).parent_path().string());
        fs::create_directory(std::string("../Backup/")+path);
        std::cout<<path<<" created correctly in backups.\n\n";
        return 0;
    }
}
