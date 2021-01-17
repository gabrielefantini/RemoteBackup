#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Socket.h"
#include "ServerSocket.h"
#include "serverUtils/serverUtils.h"

ServerSocket ss (5000);

int main() {
    //all'avvio: inizializzo una map di corrispondenze user-path -> indice
    //necessarie per localizzare in seguito la giusta cartella
    std::map<std::pair<std::string,std::string>,int> user_map;
    if(setupUsers(user_map)!=0) {
        std::cout<<"error during users setup.\n";
        return -1;
    }
    //debug
    printUserMap(user_map);
    int flag = 0;
    while(true) {
        struct sockaddr_in addr;
        unsigned int len = sizeof(addr);
        std::cout << "Waiting for incoming connections..." << std::endl;
        // mi blocco finché non arriva un client
        Socket s = ss.accept(&addr, &len);
        // in addr a questo punto ci saranno i dettagli del client
        // stampo l'indirizzo di rete
        // name contiene fino a 16 caratteri, il numero massimo di caratteri per un indirizzo IP_v4
        char name[16];
        if (inet_ntop(AF_INET, &addr.sin_addr, name, sizeof(name)) == nullptr)
            throw std::runtime_error("Cannot convert address");
        // la porta è scritta in addr nel formato network, devo convertirla
        std::cout << "Got a connection from " << name << ": " << ntohs(addr.sin_port) << std::endl;

        int res=s.ResToNotify();
        if(res==0){
            //cur_client: client che ha eseguito la richiesta
            //cur_dir   : directory client di cui eseguire il backup
            std::string cur_client=s.get_cur_client();
            std::string cur_dir=s.get_cur_dir();
            std::cout<< "client: "<<cur_client<<std::endl;
            std::cout<< "directory: "<<cur_dir<<std::endl;

            //b_setup->first=backup_dir_name:   path della cartella di backup   (nome)
            //b_setup->second=id:               id della directory di backup    (in relazione al client)
            std::pair<std::string,int> b_setup=getBackupDir(user_map,cur_client,cur_dir);
            std::string backup_dir_name=b_setup.first;
            int id=b_setup.second;
            std::cout << "backup path name: " << backup_dir_name<< "\n";

            //prepara la directory di backup partendo dal nome precedentemente generato
            int res_setup=s.setupDir(backup_dir_name,cur_client,id);

            //prepara la directory temporanea (per gli upload futuri)
            std::string tmp_dir_name=getTmpDir(backup_dir_name);
            std::cout<<"files will go there: "<<tmp_dir_name<<std::endl;

            //localMap: hash map lato server
            //cerco se ho una copia della map in <relative pos>/backup/{client}/{id}/localMap.txt
            //altrimenti ho una map vuota
            std::map<std::string,std::string> localMap;
            std::string localMapPath=getMapPath(backup_dir_name);

            if(res_setup==1){
                std::cout<<"cartella di backup trovata -> ricerca file map\n";
                int res_map=setupLocalMap(localMapPath,localMap);
                if(res_map==1)
                    std::cout<<"file map trovato -> map caricata\n";
                else
                    std::cout<<"file map non trovato -> map vuota\n";
            }else
                std::cout<<"cartella di backup nuova -> map vuota\n";

            //compara le map per richiedere i file necessari
            std::map<std::string,std::string> clientMap=s.get_clientMap();
            std::cout<<"CLIENT MAP:\n";
            for (auto& x: clientMap)
                std::cout<<x.first<<" : "<<x.second<<std::endl;
            std::cout<<"\nSERVER MAP:\n";
            for (auto& x: localMap)
                std::cout<<x.first<<" : "<<x.second<<std::endl;
            std::cout<<"\n\n";
            std::set<std::string> files;
            files=checkForFile(clientMap,localMap);
            for (std::set<std::string>::iterator it=files.begin(); it!=files.end(); /*++it*/) {
                std::string file=*it;
                std::cout<<"chiedo: "<<file<<std::endl;
                std::string hash=clientMap.find(file)->second;
                char *cstr = new char[file.length() + 1];
                strcpy(cstr, file.c_str());
                int risultato = s.askFile(cstr,hash,tmp_dir_name);
                delete [] cstr;
                if (risultato == -1) {
                    //perror("Error while sending name");
                    //exit(EXIT_FAILURE);
                    if (flag == 0) {
                        std::cout << "A problem to receive the file has been detected" << std::endl;
                        std::cout << "Try to ask again the file, maybe it was a temporary error..." << std::endl;
                        //it = it--;
                        flag = 1;
                    }
                    else {
                        std::cout << "File asked two times. Probably it wasn't a temporary error, I close the communication" << std::endl;
                        flag = 2;
                        break;
                    }
                }
                else flag = 0;
                if (flag == 0) it++;
            }
            if (flag != 2) {
                s.sendOk();

                //aggiorno la cartella di backup
                updateBackupFolder(clientMap, localMap, tmp_dir_name, backup_dir_name, cur_dir);
                //aggiorno il file map
                //debug
                std::cout << "nuova SERVER MAP:\n";
                localMap = clientMap;
                for (auto &x: localMap)
                    std::cout << x.first << " : " << x.second << std::endl;
                std::cout << "\n\n";
                //
                int update_res = saveLocalMap(localMapPath, localMap);
                if (update_res == 0)
                    std::cout << localMapPath << " aggiornato correttamente.\n";
            }
            flushTmp(tmp_dir_name);

        }
        else
            std::cout<<"Communication failed!\n";
    }
    return 0;
}