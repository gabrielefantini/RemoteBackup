#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Socket.h"
#include "ServerSocket.h"

ServerSocket ss (5000);

int main() {
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

        std::cout << "Before read from socket" << std::endl;
        s.receiveFile();
        std::cout << "Connection closed" << std::endl;

    }
    return 0;
}

/*
 * prima di creare il client posso testare la connessione con "telnet localhost 5000" da terminale
 */