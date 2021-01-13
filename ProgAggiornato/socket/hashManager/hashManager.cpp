//
// Created by gabriele on 21/08/20.
//

#include "hashManager.h"


int add(std::string path, std::map<std::string,std::string> &localStructure){

    try {
        // Create a Path object from given path string
        filesys::path pathObj(path);

        // Check if path exists and is of a regular file
        if (filesys::exists(pathObj) && filesys::is_regular_file(pathObj)){
            //pezzetti in cui verrà analizzato il file
            unsigned char buf[8192];
            //digest finale
            unsigned char final[SHA_DIGEST_LENGTH];
            //struttura di supporto per la creazione del digest
            SHA_CTX sc;
            FILE *f;
            int err;

            //apre il file
            char name[path.length()+1];
            strcpy(name, path.c_str());

            f = fopen(name, "rb");
            if (f == NULL) {
                //aggiungere gestione dell'errore se non riesce ad aprire il file
                std::cout<<"err opening file \n";
                return 1;
            }
            //Inizializza la struttura SHA_CTX
            SHA1_Init(&sc);


            size_t len;
            len = fread(buf, 1, sizeof buf, f);
            if (len == 0){
                //se il file è vuoto non calcolo l'hash ma metto "empty" come valore hash
                localStructure[path] = "empty";
                return 0;
            } else{
                // se invece il file non è vuoto, calcolo l'hash caricando un pezzo di file alla volta, fino ad arrivare alla fine
                while(len!=0) {
                    SHA1_Update(&sc, buf, len);
                    len = fread(buf, 1, sizeof buf, f);
                }

                err = ferror(f);
                fclose(f);

                if (err) {
                    /* Aggiungere gestione degli errori di I/O */
                    std::cout<<"I/O error \n";
                    return 1;
                }
                //Inserisce il digest del file in final (deve avere almeno lunghezza 20) ed elimina SHA_CTX
                SHA1_Final(final, &sc);
                std::string digest( final, final + sizeof final / sizeof final[0] );
                //inserisce il path con il relativo digest nella map
                localStructure[path] = digest;
                return 0;
            }
        }
        // Check if path exists and is of a directory
        if (filesys::exists(pathObj) && filesys::is_directory(pathObj)){
            //inserisce il path e NULL come digest fittizio
            localStructure[path] = "";

            return 0;
        }

    }
    catch (filesys::filesystem_error & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 1;
};

int remove(std::string path, std::map<std::string,std::string> &localStructure){
    localStructure.erase(path);
    return 0;
};
int modify(std::string path, std::map<std::string,std::string> &localStructure){
    remove(path, localStructure);
    add(path, localStructure);
    return 0;
};

int create_backup_initial(std::string path_from, std::map<std::string,std::string> &localStructure){
    for (auto &file : filesys::recursive_directory_iterator(path_from)) {
        std::string path=file.path().string();
        add(path, localStructure);
    }
    return 0;
}