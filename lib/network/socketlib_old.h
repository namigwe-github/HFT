#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>


//Attempted with Inheritance 

namespace hft {
    namespace socklib {
        void createSocket(struct addrinfo *hints, struct addrinfo *res) {

        }

        class BSDSocket {
            protected:
                explicit BSDSocket(const std::string& port) : socketport(port){
                    std::cout << "Socket constructor called\n";
                    memset(&hints,0,sizeof hints);
                    hints.ai_family = AF_UNSPEC;
                    hints.ai_flags = AI_PASSIVE;
                    hints.ai_protocol = 0;   
                }

                void getAddressInfo() {
                    int status;
                    if ((status = getaddrinfo(NULL, socketport.c_str(),&hints,&res)) != 0) {
                        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
                        exit(EXIT_FAILURE);
                    } 
                }
                
                void getAddressInfo(std::string& ip) {
                    int status;
                    if ((status = getaddrinfo(ip.c_str(), socketport.c_str(),&hints,&res)) != 0) {
                        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
                        exit(EXIT_FAILURE);
                    } 
                }

                inline struct addrinfo getHints() const {
                    return hints;
                }

                inline struct addrinfo* getRes() const {
                    return res;
                }

                inline int getSockfd() const {
                    return sockfd;
                }

                inline void setSockfd(const int& sockfd) {
                    this->sockfd = sockfd;
                }
                
                inline std::string getSocketPort() const {
                    return socketport;
                }

                inline void setSocketPort(const std::string& port) {
                    socketport = port;
                }

                inline void setClientAddr(struct sockaddr_storage& client_addr) {
                    this->client_addr = client_addr;
                }

                inline struct sockaddr_storage getClientAddr() const {
                    return client_addr;
                }

                virtual void setSockType() = 0;
                struct addrinfo hints, *res;
                struct sockaddr_storage client_addr;
                std::string socketport;
                int sockfd;
        };

        class TCPSocket : public BSDSocket {
            protected:
                explicit TCPSocket(const std::string& port) : BSDSocket(port){
                    std::cout << "TCP Socket constructor called\n";
                    setSockType();
                }

                void serverSocketCreation() {
                    getAddressInfo();
                    int yes = 1;
                    struct addrinfo *p;
                    for (p = getRes(); p != NULL; p = p->ai_next) {
                        setSockfd(socket(p->ai_family,p->ai_socktype,p->ai_protocol));
                        if (getSockfd() == -1) {
                            std::cerr << "socket error\n";
                            continue;
                        }

                        if(setsockopt(getSockfd(),SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
                            std::cerr << "setsockopt error\n";
                            exit(EXIT_FAILURE);
                        }

                        if (bind(getSockfd(),p->ai_addr,p->ai_addrlen) == 0) {
                            break;
                        } 
                        std::cerr << "bind error\n";
                    }

                    if (p == NULL) {
                        std::cerr << "Failed to bind\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Socket created\n";

                }

                void serverListen(const int& backlog = 2) {
                    if (listen(getSockfd(),backlog) == -1) {
                        std::cerr << "listen error\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Socket listening...\n";
                }

                int serverAccept() {
                    socklen_t addr_size = sizeof client_addr;
                    int new_fd = accept(getSockfd(),(struct sockaddr *)&getClientAddr(),&addr_size);
                    if (new_fd == -1) {
                        std::cerr << "accept error\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Socket accepted\n";
                    return new_fd;
                }

                void clientInitConnection(std::string& ip) {
                    getAddressInfo(ip);
                    struct addrinfo *p;
                    for (p = res; p != NULL; p = p->ai_next) {
                        setSockfd(socket(p->ai_family,p->ai_socktype,p->ai_protocol));
                        if (getSockfd() == -1) {
                            std::cerr << "socket error\n";
                            continue;
                        }

                        if (connect(getSockfd(),p->ai_addr,p->ai_addrlen) == -1) {
                            std::cerr << "connect error\n";
                            continue;
                        }
                        break;
                    }

                    if (p == NULL) {
                        std::cerr << "Failed to connect\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Client socket connected\n";
                }

            inline void setSockType() override {
                hints.ai_socktype = SOCK_STREAM;
            }
        };

        class UDPSocket : public BSDSocket {
            public:
                explicit UDPSocket(const std::string& port) : BSDSocket(port){
                    std::cout << "UDP Socket constructor called\n";
                    setSockType();
                    //initSocket();
                }
            
            inline void setSockType() override {
                hints.ai_socktype = SOCK_DGRAM;
            }
        };

        
    }
}