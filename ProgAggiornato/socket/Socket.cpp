//
// Created by mattia on 21/08/20.
//

#include "Socket.h"
#include "jsonUtils.h"
#include "serverUtils/serverUtils.h"
#include <boost/filesystem.hpp>
#include <nlohmann/json.hpp>



int Socket::askFile(char* path,std::string hash,std::string &dir){
    int len = 0;
    int j = 0;

    while(path[j]!='\0') {len++;j++;}
    len++;

    char len_char[4];
    sprintf(len_char,"%d",len);

    std::cout<<path<<" : "<<len_char<<std::endl;

    // PRIMA WRITE: dimensione
    int bytes_written;
    try {
        bytes_written = write(len_char, sizeof(len_char), 0);
    } catch (std::exception &e) {
        return -1;
    }

    if (bytes_written == -1) {
        //perror("Error while sending dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }

    // SECONDA WRITE: nome
    bytes_written = write(path, len, 0);

    if (bytes_written == -1) {
        //perror("Error while sending name");
        //exit(EXIT_FAILURE);
        return -1;
    }

    std::cout << "Spedito: " << path << " bytes ("<<bytes_written<<" B)"<< std::endl;

    int risultato = receiveFile(hash,dir);

    if (risultato == -1) {
        //perror("Error while receiving file");
        //exit(EXIT_FAILURE);
        return -1;
    }

    return 1;
}

bool Socket::sendOk(){
    char len_char[4];
    int len,j,bytes_written = 0;
    std::string msg = "ok";
    char* ok = const_cast<char *>(msg.c_str());
    ///ULTIMA WRITE -> OK
    len = 0; j = 0;
    while(ok[j]!='\0') {len++;j++;}
    len++;
    sprintf(len_char,"%d",len);

    bytes_written = write(len_char, sizeof(len_char), 0);
    if (bytes_written == -1) {
        //perror("Error while sending dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    bytes_written = write(ok, len, 0);
    if (bytes_written == -1) {
        //perror("Error while sending dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    std::cout<<"sent ok to client.\n";
    return true;
}

int Socket::receiveFile(std::string name,std::string &dir) {
    int bytes_received;
    char len[10];
    /// PRIMA READ: leggo la dimensione del titolo
    bytes_received = read(len, sizeof(len), 0);
    if (bytes_received == -1 || bytes_received == 0) {
        perror("Error while reading size");
        //exit(EXIT_FAILURE);
        return -1;
    }
    std::cout << "received len: " << atoi(len) << std::endl;

    std::string newfile=std::string(dir + "/" + name);
    char *cstr = new char[newfile.length() + 1];
    strcpy(cstr, newfile.c_str());
    std::cout<< "creating: " << cstr << std::endl;
    FILE* fr = fopen(cstr, "w");

    char buffer[1024];
    int len_tot=atoi(len);
    ssize_t bytes_read = 0;
    ssize_t tot = 0;
    bytes_read = read(buffer, sizeof(buffer)-1, 0);
    if (bytes_read == -1) return -1;
    while(bytes_read > 0 ) {
        tot+=bytes_read;

        if (bytes_read == -1) {
            perror("Error while reading");
            //exit(EXIT_FAILURE);
            return -1;
        }

        std::cout << "Dentro al while" << std::endl;
        std::cout << buffer << std::endl;
        printf("%d / %d bytes read\n", tot,len_tot);
        std::cout<< "----------------------" <<std::endl;

        int written = fwrite(&buffer, sizeof(char), bytes_read, fr);
        if(ferror(fr))
        {
            perror("Error while writing to file");
            fclose(fr);
            exit(EXIT_FAILURE);
        }
        std::cout << "Written: " << written << " bytes" << std::endl;
        if(tot == len_tot){
            std::cout << "Ricevuto tutto" << std::endl;
            break;
        }

        bytes_read = read(buffer, sizeof(buffer)-1, 0);
        if (bytes_read == -1) {
            perror("Error while reading");
            //exit(EXIT_FAILURE);
            return -1;
        }
    }
    buffer[bytes_read] = 0;
    if(bytes_read == -1)
        return -1;
    std::cout << "receive finita" << std::endl;
    /// chiudo il file
    fclose(fr);
    delete[] cstr;

    return 1;
}

void Socket::connect(struct sockaddr_in *addr, unsigned int len)  {
    if (::connect(sockfd, reinterpret_cast<struct sockaddr*>(addr), len)!=0)
        throw std::runtime_error("Cannot connect to remote socket");
}

ssize_t Socket::write(const char *buffer, size_t len, int options)  {
    ssize_t res = send(sockfd, buffer, len, MSG_NOSIGNAL);
    if (res < 0) //throw std::runtime_error("Cannot write to socket");
        return -1;
    return res;
}

ssize_t Socket::read(char *buffer, size_t len, int options)  {
    struct timeval tv;
    tv.tv_sec = 4; // setta il timeout massimo
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    std::cout << "read called" << std::endl;
    ssize_t res = recv(sockfd, buffer, len, options);
    if (res < 0) {
        std::cout << strerror(errno) << std::endl;
        //throw std::runtime_error("Cannot receive from socket");
        return -1;
    }
    return res;
}


/*
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


    //for (auto elem : client_dir) {
    //    std::cout << "Scansione lista client..." << std::endl;
    //    if (strcmp(elem.first, name) == 0) {
    //        cur_client = name;
    //        cur_dir = dir;
    //        std::cout << "Bentornato " << cur_client <<", sto controllando "<< cur_dir << std::endl;
    //    }
    //}

    /// nuovo client
    //if (cur_client == nullptr) {
    //    std::cout << "Nuovo client!" << std::endl;
    //    std::pair<char*, char*> elem = {name, dir};
    //    client_dir.insert(elem);
    //    cur_client = name;
    //    cur_dir = dir;
    //    std::cout << "Nuovo utente! Benvenuto " << cur_client <<", controllerò "<< cur_dir << std::endl;
    //}

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
}*/

// da mainUtil.h
namespace fs=boost::filesystem;
//setup_dir: cerca se la directory relativa al path di un certo utente esiste già (return 1), altrimenti la crea (return 0)
int Socket::setupDir(const std::string &new_path,std::string &client,int index){
    if(fs::exists(new_path) && fs::is_directory(new_path)) {
        std::cout << new_path << " found in backups.\n";
        return 1;
    }else{
        std::cout<<new_path<<" missing!\n";
        // <relative pos>/backup/{client}/{id}/folder
        //<relative pos>/backup/{client}
        size_t pos=new_path.find(std::string("backup/"+client))+std::string("backup/"+client).length();
        std::string tmp=new_path.substr(0,pos);
        fs::create_directory(tmp);
        //<relative pos>/backup/{client}/{id}
        pos+=1+std::to_string(index).length();
        tmp=new_path.substr(0,pos);
        fs::create_directory(tmp);
        //<relative pos>/backup/{client}/{id}/folder
        fs::create_directory(new_path);
        std::cout<<new_path<<" created correctly in backups.\n\n";
        return 0;
    }
}

int Socket::ResToNotify() {
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

    int len_path = atoi(len)-1+atoi(len_name);
    std::cout << "Len_path: " << len_path << std::endl;
    char path_complessivo[len_path];

    for (int i = 0; i<atoi(len_name); i++) path_complessivo[i] = name[i];
    //path_complessivo[atoi(len_name)-1] = '/';
    for (int i = 0; i<atoi(len); i++) path_complessivo[i+atoi(len_name)-1] = dir[i];
    std::cout << "Percorso creato: " << path_complessivo << std::endl;

    cur_client = name;
    cur_dir = dir;

    /// RICEVO MAP

    // 1. ricevo char
    /// PRIMA READ: leggo la dimensione del nome
    bytes_received = read(len, sizeof(len), 0);
    /// atoi è una funzione che mi permette di convertire un char in un intero
    std::cout << "Dimensione map: " << atoi(len) << std::endl;
    /// alloco una struttura per contenere il nome della lunghezza del nome
    char char_map[atoi(len)];
    /// SECONDA READ: leggo il nome
    bytes_received = read(char_map, sizeof(char_map), 0);
    /// inserisco il terminatore di stringa come ultimo carattere del titolo
    char_map[atoi(len)-1] = '\0';

    // 2. converto in stringa e poi in map
    std::string s = std::string(char_map, len);
    nlohmann::json json_map = json::parse(s);
    std::cout << "Parse effettuato" << std::endl;
    std::map<std::string, std::string> localMap = std::map<std::string, std::string>();
    from_json(localMap, json_map);

    std::cout << "--- Mappa ricevuta ---" << std::endl;
    for (auto x : localMap)
        std::cout << x.first << ", " << x.second << std::endl;

    /// POSSIBILE CONFRONTO: la funzione non è bool ma restituisce la mappa e nel main si fa il confronto
    clientMap = localMap;
    bytes_received = read(len, sizeof(len), 0);
    char ok[atoi(len)];
    bytes_received = read(ok, sizeof(ok), 0);
    ok[atoi(len)-1] = '\0';
    std::cout << "Messaggio ricevuto: " << ok << std::endl;

    // a questo punto il server sa che il client è riuscito a inviare tutto correttamente e invia il suo ok
    int bytes_written = write(len, sizeof(len), 0);
    if (bytes_written == -1) {
        //perror("Error while sending dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    bytes_written = write(ok, sizeof(ok), 0);
    if (bytes_written == -1) {
        //perror("Error while sending dimension");
        //exit(EXIT_FAILURE);
        return -1;
    }
    return 0;
}