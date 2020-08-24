#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ClientSocket.h"


int main() {
    ClientSocket cs (5000);

    std::cout << "Inizio a spedire il file" << std::endl;
    int bytes_written = cs.sendFile("prova2.txt");
    std::cout << "Bytes_written: " << bytes_written << std::endl;
    std::cout << "Connection closed" << std::endl;

    return 0;
}
