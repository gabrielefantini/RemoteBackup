#include <iostream>
#include <map>
#include "ClientSocket.h"
#include "hashManager/hashManager.h"
#include "fileWatcher/FileWatcher.h"

int main(int argc,char** argv) {
    // argomenti attesi: usr path
    if(argc!=3){
        std::cout<<"ERRORE: numero argomenti errato (specifica 'usr' e 'path')"<<std::endl;
        return -1;
    }
    // user
    std::string usr=argv[1];
    std::cout <<argv[2] << std::endl;
    // path: se il path specificato non esiste, viene lanciato un errore
    filesys::path backupPath=filesys::canonical(filesys::path(argv[2]));
    // inizializzo la map all'avvio del programma
    std::map<std::string,std::string> localMap;
    // definisco la path sulla quale voglio eseguire il backup
    std::string backupDir = backupPath.string();
    // analizzo tutta la directory e inserisco i valori nella map
    create_backup_initial(backupDir, localMap);


    //===================================================================
    //      invio il primo messaggio al server
    //===================================================================
    // creo il socket
    {
        ClientSocket cs{5000};

        try {
            cs.notify(usr, backupDir, localMap);
            cs.WaitForSendingFile();
        }
        catch(std::runtime_error &e) {
            std::cout << "Errore notify: " << e.what() << std::endl;
        }
    }

    // notify()

    // cs esce dallo scope e viene chiusa la connessione

    //===================================================================
    //       avvio il fileWatcher... ogni volta che rileva una modifica
    //       crea una connessione con il server e gli invia la nuova map.
    //===================================================================

    Filewatcher fw{ backupDir,std::chrono::milliseconds(5000) };

    fw.start([&localMap, backupDir, usr] (std::string path_to_watch,FileStatus status) -> void {

        switch (status) {
            case FileStatus::created: {
                std::cout << path_to_watch << " created\n";
                add(path_to_watch, localMap);
                break;
            }
            case FileStatus::modified: {
                modify(path_to_watch, localMap);
                std::cout << path_to_watch << " modified\n";
                break;
            }
            case FileStatus::erased: {
                std::cout << path_to_watch << " erased\n";
                remove(path_to_watch, localMap);
                //std::cout<<"map updated:\n";
                break;
            }
            case FileStatus::do_update: {
                std::cout << "è successo qualcosa\n";
                // mi connetto
                ClientSocket socket{5000};
                try {
                    socket.notify(usr, backupDir, localMap);
                    socket.WaitForSendingFile();
                }
                catch(std::exception &e) {
                    std::cout << "Errore notify: " << e.what() << std::endl;
                }

                // notifico il server della modifica
                // notify()
                // esco dallo scope e quindi la connessione viene abbattuta
                break;
            }
            default: {
                std::cout << "Error: unknown file status.\n";
            }
        }
    });
    return 0;
}