#pragma once
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h> //close
#include <sys/epoll.h>
#include <fcntl.h>
#include <thread>

#include <arpa/inet.h> //inet_ntop
#include <netinet/tcp.h> //TCP_NODELAY



namespace hft {
    namespace networklib {
        

        class BSDSocket {
            public:
                enum class SocketType {
                    TCP,
                    UDP
                };

                BSDSocket() {
                    init();
                }
                ~BSDSocket() {
                    //freeaddrinfo(res);
                    //freeaddrinfo(&hints);
                    std::cout << "Socket destructor called\n";
                    /*if (close(_sockfd) == -1) {
                        std::cerr << "close error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }*/
                }

                BSDSocket(const BSDSocket&) = delete;
                BSDSocket(BSDSocket&&) = delete;
                BSDSocket& operator=(const BSDSocket&) = delete;
                BSDSocket& operator=(BSDSocket&&) = delete;

                explicit BSDSocket(const std::string& port) : socketport(port){
                    init();
                }

                inline void init() {
                    std::cout << "Socket constructor called\n";
                    memset(&hints,0,sizeof hints);
                    hints.ai_family = AF_UNSPEC;
                    hints.ai_flags = AI_PASSIVE;
                    hints.ai_protocol = 0;   
                }

                void setSockType(BSDSocket::SocketType type) {
                    switch(type) {
                        case SocketType::UDP:
                            hints.ai_socktype = SOCK_DGRAM;
                            break;
                        default:
                            hints.ai_socktype = SOCK_STREAM;
                            break;
                    }
                }
                
                void getAddressInfo(const std::string& ip = std::string{}) {
                    int status;
                    switch (ip.empty()){
                        case false:
                            if ((status = getaddrinfo(ip.c_str(), socketport.c_str(),&hints,&res)) != 0) {
                                std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
                                exit(EXIT_FAILURE);
                            }
                            break;
                        
                        default:
                            if ((status = getaddrinfo(NULL, socketport.c_str(),&hints,&res)) != 0) {
                                std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
                                exit(EXIT_FAILURE);
                            }
                            break;
                         
                    }        
                }

                inline int createSocket() {
                    //Creat Socket and set it to non-blocking
                    /*if ((_sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol)) == -1) {
                        std::cerr << "socket error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }*/
                    return socket(res->ai_family,res->ai_socktype,res->ai_protocol);
                }
        
                inline void setSockOpt() {
                    //Set non-blocking, reuse address and Nagale's algorithm
                    setSockNonBlock(_sockfd);
                    _setSockReuseAddr();
                    _setSockNagaleDisable();
                    
                }

                inline int bindSocket(const struct addrinfo* p) {
                    /*if (bind(_sockfd,p->ai_addr,p->ai_addrlen) == -1) {
                        std::cerr << "bind error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }*/
                    return bind(_sockfd,p->ai_addr,p->ai_addrlen);
                }

               
            
                inline std::string getSocketPort() const {
                    return socketport;
                }

                inline void setSocketPort(const std::string& port) {
                    socketport = port;
                }

                inline struct addrinfo* getRes() const {
                    return res;
                }

                inline void closeSocket() {
                    if (close(_sockfd) == -1) {
                        std::cerr << "close error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }

                inline int getSockfd() const {
                    return _sockfd;
                }

                inline int setSockfd(const int& sockfd) {
                    _sockfd = sockfd;
                    return _sockfd;
                }
                
            inline void setSockNonBlock(const int fd) {
                int flags = fcntl(fd,F_GETFL,0);
                if (flags == -1) {
                    std::cerr << "fcntl error: " << strerror(errno) << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (fcntl(fd,F_SETFL,flags | O_NONBLOCK) == -1) {
                    std::cerr << "fcntl error: " << strerror(errno) << std::endl;
                    exit(EXIT_FAILURE);
                }    
            }
            private:

                inline void _setSockReuseAddr() {
                    int yes = 1;
                    if (setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
                        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }    
                }

                inline void _setSockNagaleDisable() {
                    int one = 1;
                    if (setsockopt(_sockfd,IPPROTO_TCP,TCP_NODELAY,reinterpret_cast<void *>(&one),sizeof(one)) == -1) {
                        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }    
                }
                struct addrinfo hints;
                struct addrinfo *res;
                std::string socketport;
                int _sockfd;
        };

        class TCPSocket {
            public:
                enum class TCPSocketType {
                    SERVER,
                    CLIENT
                };

                TCPSocket() = delete;
                TCPSocket(const TCPSocket&) = delete;
                TCPSocket(TCPSocket&&) = delete;
                TCPSocket& operator=(const TCPSocket&) = delete;
                TCPSocket& operator=(TCPSocket&&) = delete;


                explicit TCPSocket(const TCPSocket::TCPSocketType& type) : _type(type) {
                    init(std::string{"3094"});
                } 
                explicit TCPSocket(const TCPSocket::TCPSocketType& type, const std::string& port, const int& backlog = 10) : _backlog(backlog) , _type(type){
                    init(port);
                    _events = static_cast<struct epoll_event*> (new struct epoll_event[_backlog]);
                }

                ~TCPSocket() {
                    std::cout << "TCP Socket destructor called\n";
                    std::cout << "Socket closed\n";
                }

                void init(const std::string& port) {
                    std::cout << "TCP Socket constructor called\n";
                    _bsdsocket.setSockType(BSDSocket::SocketType::TCP);
                    _bsdsocket.setSocketPort(port);
                    _bsdsocket.getAddressInfo();
                }

                void serverBindSocket() {
                    struct addrinfo *p;
                    int sockfd;
                    for (p = _bsdsocket.getRes(); p != NULL; p = p->ai_next) {
                        sockfd = _bsdsocket.createSocket();
                        if (sockfd == -1) {
                            std::cerr << "socket error: " << strerror(errno) << "\n";
                            continue;
                        }
                        _bsdsocket.setSockfd(sockfd);
                        _bsdsocket.setSockOpt();

                        if (_bsdsocket.bindSocket(p) == 0) break;
                        std::cerr << "bind error: " << strerror(errno) << "\n";
                    }

                    if (p == NULL) {
                        std::cerr << "Failed to bind\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Socket created"<< p << "\n";
                    
                    _sock_addr = p;
                    char ipstr[INET6_ADDRSTRLEN];
                    
                    //inet_ntop(p->ai_family,&((struct sockaddr_in *)p->ai_addr)->sin_addr,ipstr,sizeof ipstr);
                    //std::cout << "Server IP: " << ipstr << "\n";
                }

                void pollEvents() {
                    setUpEventList();
                    //Spawn a thread to poll for events
                    auto poll = [&](){
                        struct epoll_event event;
                        int fd;
                        while(1) {
                                // Wait for events, returns only fds that are ready
                                std::cout << "Waiting for events\n";
                                //Block until an event occurs
                                int nfds = epoll_wait(_epollfd,_events,_backlog,-1);

                                if (nfds == -1) {
                                    std::cerr << "epoll_wait error: " << strerror(errno) << std::endl;
                                    exit(EXIT_FAILURE);
                                }
                                // Process the events
                                for (int i = 0; i < nfds; i++) {
                                    event = _events[i];
                                    fd = event.data.fd;
                                    //Check if event is for server socket
                                    if (fd == _bsdsocket.getSockfd()) {
                                        if (event.events & EPOLLIN) {
                                            //accept incoming connections
                                            std::cout << "Accepting incoming connections\n";
                                            acceptClientConnections();
                                        } 
                                        
                                    } else if (event.events & EPOLLIN) {
                                        // Read data from client
                                        //readFromClient(fd);
                                        std::cout << "Server received: " << recvMsg(fd) << "\n";
                                    
                                    } else if (event.events & EPOLLOUT) {
                                        // Write data to client
                                        //writeToClient(fd);
                                        std::cout << "Server sent hello\n";
                                        
                                    } else if ((event.events & EPOLLERR) || (event.events & EPOLLHUP)) {
                                        // client closed connection
                                        //deletePollEvent(fd);
                                        close(fd);
                                    }
                                    
                                }
                            }

                    };

                    //std::thread t1{poll};
                    //t1.join();
                    poll();

                }

                void sendMsg(const std::string& msg) {
                    if (send(_bsdsocket.getSockfd(),msg.c_str(),msg.size(),0) == -1) {
                        std::cerr << "send error: " << strerror(errno) << "\n";
                        exit(EXIT_FAILURE);
                    }
                }

                std::string recvMsg(const int& fd) {
                    char buf[1024];
                    int numbytes;
                    if ((numbytes = recv(fd,buf,sizeof buf,0)) == -1) {
                        std::cerr << "recv error: " << strerror(errno) << "\n";
                        exit(EXIT_FAILURE);
                    }
                    buf[numbytes] = '\0';
                    return std::string(buf);
                }

                inline void listenIncomingConnections() {
                    if (listen(_bsdsocket.getSockfd(),_backlog) == -1) {
                        std::cerr << "listen error\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Server listening\n";
                }

                

                /*std::string getServerIP() const {
                    switch (_type == TCPSocketType::SERVER)
                    {
                    case false:
                        return getIP(_other_addr);
                    default:
                        std::cout << _sock_addr << "\n";
                        return getIP(*(reinterpret_cast<struct sockaddr_storage *>(_sock_addr->ai_addr)));
                    }

                }*/

                

                void connectToServer(const std::string& ip) {
                    _bsdsocket.getAddressInfo(ip);
                    struct addrinfo *p;
                    int sockfd;
                    for (p = _bsdsocket.getRes(); p != NULL; p = p->ai_next) {
                        sockfd = _bsdsocket.setSockfd(_bsdsocket.createSocket());
                        if (sockfd == -1) {
                            std::cerr << "socket error\n";
                            continue;
                        }

                        if (connect(sockfd,p->ai_addr,p->ai_addrlen) == -1) {
                            std::cerr << "connect error\n";
                            continue;
                        }
                        break;
                    }

                    if (p == NULL) {
                        std::cerr << "Failed to connect: " << strerror(errno) << "\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "Client socket connected\n";
                    //_other_addr = *(reinterpret_cast<struct sockaddr_storage *>(p->ai_addr));
                }

                /*std::string getClientIP() const {
                    switch (_type == TCPSocketType::CLIENT)
                    {
                    case false:
                        return getIP(_other_addr);
                    default:
                        return getIP(*(reinterpret_cast<struct sockaddr_storage *>(_sock_addr->ai_addr)));
                    }
                
                }*/


            private:
                void acceptClientConnections() {
                    struct sockaddr_storage _other_addr;
                    socklen_t addr_size = sizeof _other_addr;
                    int new_fd;
                    while(1) {
                        // Accept incoming connections
                        new_fd = accept(_bsdsocket.getSockfd(),(struct sockaddr *)&_other_addr,&addr_size);

                        if (new_fd == -1) {
                            // No more incoming connections
                            if ((errno==EAGAIN) || (errno == EWOULDBLOCK)) {
                                break;
                            } else {
                                //Accept error 
                                std::cerr << "accept error \n";
                                exit(EXIT_FAILURE);
                            }
                        }

                        // Set the new socket to non-blocking
                        _bsdsocket.setSockNonBlock(new_fd);


                        // Add the new socket to the epoll event list
                        addPollEvent(new_fd);
                    }  
                }   

                inline void setUpEventList() {
                    _epollfd = epoll_create1(0);
                    if (_epollfd == -1) {
                        std::cerr << "epoll_create1 error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    addPollEvent(_bsdsocket.getSockfd());
                }

                inline void addPollEvent(int fd) {
                    struct epoll_event event;
                    event.events = EPOLLIN|EPOLLET;
                    event.data.fd = fd;
                    if (epoll_ctl(_epollfd,EPOLL_CTL_ADD,fd,&event) == -1) {
                        std::cerr << "epoll_ctl error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }

                inline void deletePollEvent(int fd) {
                    if (epoll_ctl(_epollfd,EPOLL_CTL_DEL,fd,NULL) == -1) {
                        std::cerr << "epoll_ctl error: " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }
                }

                std::string getIP(const struct sockaddr_storage& addr) const {
                    char ipstr[INET6_ADDRSTRLEN];
                    void *addrptr;
                    if (addr.ss_family == AF_INET) {
                        //&(reinterpret_cast<const struct sockaddr_in *>(&addr))->sin_port;
                        addrptr = &((struct sockaddr_in *)&addr)->sin_addr;
                    } else {
                        addrptr = &((struct sockaddr_in6 *)&addr)->sin6_addr;
                    }
                    inet_ntop(addr.ss_family,addrptr,ipstr,sizeof ipstr);
                    if(addrptr == NULL) {
                        std::cerr << "inet_ntop error\n";
                        exit(EXIT_FAILURE);
                    }
                    return std::string(ipstr);
                }

                inline void closeTCPSocket() {
                    _bsdsocket.closeSocket();   
                }

                BSDSocket _bsdsocket;
                //int _sockfd;
                int _backlog;
                struct addrinfo * _sock_addr;
                //struct sockaddr_storage _other_addr;
                TCPSocketType _type;
                int _epollfd;
                struct epoll_event* _events;



        };

    }
}