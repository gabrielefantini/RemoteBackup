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
    if(setup_users(user_map)!=0) {
        std::cout<<"error during users setup.\n";
        return -1;
    }
    //debug
    print_user_map(user_map);
    
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
            std::pair<std::string,int> b_setup=get_backup_dir(user_map,cur_client,cur_dir);
            std::string backup_dir_name=b_setup.first;
            int id=b_setup.second;
            std::cout << "backup path name: " << backup_dir_name<< "\n";

            //prepara la directory di backup partendo dal nome precedentemente generato
            int res_setup=s.setup_dir(backup_dir_name,cur_client,id);

            //prepara la directory temporanea (per gli upload futuri)
            std::string tmp_dir_name=get_tmp_dir(backup_dir_name);
            std::cout<<"files will go there: "<<tmp_dir_name<<std::endl;

            //localMap: hash map lato server
            std::map<std::string,std::string> localMap; //empty
            if(res_setup==1){
                std::cout<<"directory found -> check for map file\n";
            }else{
                std::cout<<"new directory -> empty map\n";
            }

            //compara le map per richiedere i file necessari
            std::map<std::string,std::string> clientMap=s.get_clientMap();
            std::set<std::string> files;
            files=check_for_file(clientMap,localMap);
            for (std::set<std::string>::iterator it=files.begin(); it!=files.end(); ++it) {
                std::string file=*it;
                std::cout<<"chiedo: "<<file<<std::endl;
                std::string hash=clientMap.find(file)->second;
                char *cstr = new char[file.length() + 1];
                strcpy(cstr, file.c_str());
                s.ask_file(cstr,hash,tmp_dir_name);
                delete [] cstr;
            }
            s.send_ok();

            //aggiorno la cartella di backup
            updateBackupFolder(clientMap,localMap,tmp_dir_name,backup_dir_name,cur_dir);
        }
        else
            std::cout<<"Communication failed!\n";
    }
    return 0;
}