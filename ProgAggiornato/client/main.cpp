#include <iostream>
#include <map>
#include "ClientSocket.h"
#include "hashManager/hashManager.h"
#include "fileWatcher/FileWatcher.h"

int main() {
    //inizializzo la map all'avvio del programma
    std::map<std::string,std::string> localMap;
    //definisco la path sulla quale voglio eseguire il backup
    //N.B. va sostituita con gli argomenti passati al main!!!
    std::string backupDir = "/home/gabriele/Desktop/ClientFolder";
    //analizzo tutta la directory e inserisco i valori nella map
    create_backup_initial(backupDir, localMap);

    //===================================================================
    //       qui andrebbe inserito il primo messaggio al server
    //===================================================================
    ClientSocket cs {5000};

    /*std::cout << "Inizio a spedire il file" << std::endl;
    int bytes_written = cs.sendFile("prova2.txt");
    std::cout << "Bytes_written: " << bytes_written << std::endl;
    */

    cs.auth("client1", "dir1");


    std::cout << "Connection closed" << std::endl;

    //===================================================================
    //       avvio il fileWatcher... ogni volta che rileva una modifica
    //       crea una connessione con il server e gli invia la nuova map.
    //===================================================================
    Filewatcher fw{ backupDir,std::chrono::milliseconds(5000) };

    fw.start([&localMap, backupDir] (std::string path_to_watch,FileStatus status) -> void {

        //ignora i casi in cui si riceve la modifica di una cartella (avviene quando un file al suo interno viene modificato, apparentemente)
        /*if(!fs::is_regular_file(fs::path(path_to_watch)) && status==FileStatus::modified){
            return;
        }*/

        switch (status) {
            case FileStatus::created:
                std::cout<<path_to_watch<<" created\n";
                add(path_to_watch, localMap);
                break;
            case FileStatus::modified:
                modify(path_to_watch, localMap);
                std::cout<<path_to_watch<<" modified\n";
                break;
            case FileStatus::erased:
                std::cout<<path_to_watch<<" erased\n";
                remove(path_to_watch, localMap);
                //std::cout<<"map updated:\n";
                break;
            case FileStatus::do_update:
                std::cout<<"è successo qualcosa";
                break;
            default:
                std::cout<<"Error: unknown file status.\n";
        }
    });
    return 0;
}
