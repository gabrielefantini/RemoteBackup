#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "hashManager/hashManager.h"
#include "main_util/mainUtilities.h"
#include "socketUtil/ServerSocket.h"
#include "socketUtil/Socket.h"

namespace fs=boost::filesystem;

ServerSocket ss(5000);

int main() {
    //debug: in questa release si vuole provare a mantenere aggiornata la cartella server di usr1 rispetto alla sua cartella client (che per testing è nello stesso dispositivo)
    /*
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
    */

    //user:
    //usr1 - ./../../server/client_dir/client1
    //fake_login("usr1","./../../server/client_dir/client1");
    //backups_for_usr("usr1");

    while(true){
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

        /*
        std::cout << "Before read from socket" << std::endl;
        s.receiveFile();
        std::cout << "Connection closed" << std::endl;
        */

        s.vrfy();
    }

    return 0;
}
