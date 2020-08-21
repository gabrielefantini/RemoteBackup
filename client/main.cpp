#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ClientSocket.h"


int main() {
    ClientSocket cs (5000);

    int a = 0;
    for (int i = 0; i< 999999999999; i++) a++;

    std::cout << "Connection closed" << std::endl;

    return 0;
}
