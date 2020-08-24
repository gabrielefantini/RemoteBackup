#include <iostream>
#include <map>
#include "hashManager/hashManager.h"
#include "mainUtil/mainUtilities.h"

namespace fs=boost::filesystem;


int main() {
    std::map<std::string,std::string> users;
    if(setup_users(users)!=0) {
        std::cout<<"error during users setup.\n";
        return -1;
    }
    std::cout<<"current online backups:\n";
    print_map(users);

    //debug: in questa release si vuole provare a mantenere aggiornata la cartella server di usr1 rispetto alla sua cartella client (che per testing è nello stesso dispositivo)
    std::string client_path=users.find("usr1")->second;
    std::string server_path=get_server_dir(client_path);

    std::map<std::string,std::string> digests_usr1_server;
    std::map<std::string,std::string> digests_usr1_client; //debug: questa in realtà verrà ricevuta dal socket

    digests_usr1_server=setup_map(server_path);
    digests_usr1_client=setup_map(client_path); //debug: questa in realtà verrà ricevuta dal socket
    std::cout<<"usr1_server digest map:\n";
    print_map(digests_usr1_server);
    std::cout<<"usr1_client digest map:\n";
    print_map(digests_usr1_client);

    std::cout<<"comp1:\n";
    compare_and_process(digests_usr1_client,digests_usr1_server,client_path,server_path);

    std::cout<<"\ncomp2:\n"; //debug: se la prima chiamata a compare_and_process va a buon fine, la seconda dovrebbe solo verificare che non siano necessari cambiamenti
    compare_and_process(digests_usr1_client,digests_usr1_server,client_path,server_path);

    return 0;
}
