#include <iostream>
#include <sys/socket.h>
#include <zconf.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// paradigma RAII: rilascio corretto delle risorse
class Socket {
    // file descriptor del socket
    int sockfd;

    // costruttore privato con file descriptor
    Socket(int fd) : sockfd(fd) {
        std::cout << "Socket " << sockfd << " created" << std::endl;
    }

    // costruttore di copia e operatore di assegnazione delete -> il file descriptor non deve essere duplicato
    Socket(const Socket&) = delete;
    Socket &operator=(const Socket&) = delete;

    friend class ServerSocket;
public:
    Socket() {
        sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        // con valore di ritorno negativo c'è stato un problema -> lancio eccezione
        if (sockfd < 0) throw std::runtime_error("Cannot create socket");
        std::cout << "Socket " << sockfd << " created" << std::endl;
    }

    ~Socket() {
        if (sockfd != 0) {
            std::cout << "Socket " << sockfd << "closed" << std::endl;
            close(sockfd);
        }
    }

    // costruttore di movimento e operatore di assegnazione per movimento
    Socket(Socket &&other) : sockfd(other.sockfd) {
        other.sockfd = 0;
    }

    Socket &operator=(Socket &&other) {
        if (sockfd != 0) close(sockfd);
        sockfd = other.sockfd;
        other.sockfd = 0;
        return *this;
    }

    // il file descriptor del socket è privato, dall'esterno non potrei leggere e scrivere dei byte
    ssize_t read(char* buffer, size_t len, int options) {
        ssize_t res = recv(sockfd, buffer, len, options);
        if (res < 0) throw std::runtime_error("Cannot receive from socket");
        return res;
    }

    ssize_t write(const char* buffer, size_t len, int options) {
        ssize_t res = send(sockfd, buffer, len, options);
        if (res < 0) throw std::runtime_error("Cannot write to socket");
        return res;
    }

    // sockaddr_in va bene per IP_v4
    void connect(struct sockaddr_in *addr, unsigned int len) {
        if (::connect(sockfd, reinterpret_cast<struct sockaddr*>(addr), len)!=0)
            throw std::runtime_error("Cannot connect to remote socket");
    }
};

// dichiarata friend di socket, devo chiamare il suo costruttore privato
// ereditarietà privata, necessario?
class ServerSocket: private Socket {
public:
    // quando viene invocato questo costruttore chiama implicitamente il costruttore di Socket (superclasse privata),
    // che crea un oggetto che finisce nel campo sockfd
    ServerSocket(int port) {
        // il ServerSocket deve essere in grado di accettare connessioni sulla porta passata, alloco un oggetto di tipo sockaddr_in
        struct sockaddr_in sockaddrIn;
        sockaddrIn.sin_port = htons(port);
        sockaddrIn.sin_family = AF_INET;
        ///sockaddrIn.sin_len = sizeof(sockaddrIn);
        sockaddrIn.sin_addr.s_addr = htonl(INADDR_ANY);

        if (::bind(sockfd, reinterpret_cast<struct sockaddr*>(&sockaddrIn), sizeof(sockaddrIn)) != 0)
            throw std::runtime_error("Cannot bind port");
        else
            std::cout << "bind ok" << std::endl;

        // il socket viene messo in modalità passiva sulla porta
        if (::listen(sockfd, 0) != 0)
            throw std::runtime_error("Error listen");
        else
            std::cout << "listen ok" << std::endl;
    }

    // accept mi dà la possibilità di avere un socket per chi vuole connettersi
    Socket accept (struct sockaddr_in* addr, unsigned int* len) {
        int fd = ::accept(sockfd, reinterpret_cast<struct sockaddr*>(addr), len);
        if (fd < 0) throw std::runtime_error("Cannot accept socket");
        return Socket(fd);
    }

};

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
        std::cout << "Connection closed" << std::endl;
    }
    return 0;
}

/*
 * prima di creare il client posso testare la connessione con "telnet localhost 5000" da terminale
 */