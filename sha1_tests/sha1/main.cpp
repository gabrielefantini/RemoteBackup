#include <iostream>
#include <stdio.h>
#include <fstream>
#include <openssl/sha.h>
using namespace std;


std::string do_sha1_file(char *name){
    FILE *f;
    unsigned char buf[8192];
    unsigned char out[SHA_DIGEST_LENGTH];

    SHA_CTX sc;
    int err;

    //apre il file
    f = fopen(name, "rb");
    if (f == NULL) {
        //aggiungere gestione dell'errore se non riesce ad aprire il file
        std::cout<<"file could not be opened"<<std::endl;
    }
    //Inizializza la struttura SHA_CTX
    SHA1_Init(&sc);

    //Eseguito fino a che non si arriva alla fine del file
    while(true) {
        size_t len;

        len = fread(buf, 1, sizeof buf, f);
        if (len == 0)
            break;
        //Chiamato più volte con pezzi del file che devono esssere processati
        SHA1_Update(&sc, buf, len);
    }

    err = ferror(f);
    fclose(f);

    if (err) {
        /* Aggiungere gestione degli errori di I/O */
        std::cout<<"I/O error"<<std::endl;
    }
    //Inserisce il digest del file in out (deve avere almeno lunghezza 20) ed elimina SHA_CTX
    SHA1_Final(out, &sc);
    std::string digest( out, out + sizeof out / sizeof out[0] );
    return digest;
}

int main(){
    unsigned char hash[SHA_DIGEST_LENGTH];

    ofstream outfile;
    std::string test = do_sha1_file("/home/gabriele/Desktop/test.txt");
    outfile.open("/home/gabriele/Desktop/out.bin");

    outfile<<test;
    outfile.close();
    return  0;
}

//considerazioni finali
/*
 *  Ogni digest ha la dimensione di 20 bytes(160bit).
 *  Assumendo cartelle con 10^6 (1 milione) di file avrò un'occupazione causata dai digest di 10^6*20 bytes, ovvero 20MB.
 *  Si può ritenere accettabile tenere tale struttura all'interno della Ram lato client.
 *  Lato server? Considerando 100 utenti si arriva gia a 2GB... tenere in considerazione il salvataggio su filesystem!
 *
 *
 *
 */