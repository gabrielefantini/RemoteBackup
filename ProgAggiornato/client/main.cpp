#include <iostream>
#include <map>
#include "ClientSocket.h"
#include "hashManager/hashManager.h"
#include "fileWatcher/FileWatcher.h"


int main(int argc,char** argv) {

    // static variable for connection problems
    static int connectionProblem = 0;
    std::string password;
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


        try {
            std::cout << "Inserisci la password" << std::endl;
            std::cin >> password;
            ClientSocket cs{5000};
            int r = cs.notify(usr, backupDir, password, localMap);
            if (r == -1) {
                std::cout << "Non è stato possibile comunicare col server" << std::endl;
                return 0;
            }
            cs.WaitForSendingFile();
        }
        catch(std::runtime_error &e) {
            std::cout << "Errore: " << e.what() << std::endl;
            connectionProblem = 1;
        }
    }

    // notify()

    // cs esce dallo scope e viene chiusa la connessione

    //===================================================================
    //       avvio il fileWatcher... ogni volta che rileva una modifica
    //       crea una connessione con il server e gli invia la nuova map.
    //===================================================================

    Filewatcher fw{ backupDir,std::chrono::milliseconds(5000) };

    fw.start([&localMap, backupDir, password, usr] (std::string path_to_watch,FileStatus status) -> void {
        switch (status) {
            case FileStatus::created: {
                std::cout << path_to_watch << " created\n";
                if(filesys::is_regular(path_to_watch) && !filesys::is_empty(path_to_watch)){ // se il file non è vuoto
                    add(path_to_watch, localMap);
                } if(filesys::is_directory(path_to_watch)) { // se è una directory
                    add(path_to_watch, localMap);
                }
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
                try {
                    ClientSocket socket{5000};
                    int operationResult;
                    operationResult = socket.notify(usr, backupDir, password,localMap);
                    if(operationResult == -1){
                        connectionProblem = 1;
                        break;
                    }
                    operationResult = socket.WaitForSendingFile();
                    if(operationResult == -1){
                        connectionProblem = 1;
                        break;
                    }
                    // arriva alla fine e tutto è andato bene
                    connectionProblem = 0;
                }
                catch(std::exception &e) {
                    std::cout << "Errore: " << e.what() << std::endl;
                    connectionProblem = 1;
                }

                // notifico il server della modifica
                // notify()
                // esco dallo scope e quindi la connessione viene abbattuta
                break;
            }
            case FileStatus::default_status: {
                if(connectionProblem){
                    // tenta di rinviare l'aggiornamento

                    try {
                        ClientSocket socket{5000};
                        int operationResult;
                        operationResult = socket.notify(usr, backupDir, password, localMap);
                        if(operationResult == -1){
                            connectionProblem = 1;
                            break;
                        }
                        operationResult = socket.WaitForSendingFile();
                        if(operationResult == -1){
                            connectionProblem = 1;
                            break;
                        }
                        connectionProblem = 0;
                    }
                    catch(std::exception &e) {
                        std::cout << "Errore: " << e.what() << std::endl;
                        connectionProblem = 1;
                    }
                    // arriva alla fine ed è andato tutto bene
                }
            }
        }
    });
    return 0;
}