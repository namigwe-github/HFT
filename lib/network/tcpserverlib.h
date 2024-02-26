#include "socketlib.h"
#include <vector>
#include <unordered_map>



namespace hft {
    namespace networklib {
        class TCPServer {
            public:
                TCPServer() : _server{TCPSocket::TCPSocketType::SERVER} {
                    std::cout << "TCPServer constructor called\n";
                    init();
                }

                explicit TCPServer(const std::string& port, const int& backlog = 10) : _server{TCPSocket::TCPSocketType::SERVER,port,backlog} {
                    std::cout << "TCPServer constructor called\n";
                    init();
                }

                ~TCPServer() {
                    std::cout << "TCPServer destructor called\n";
                }

                TCPServer(const TCPServer& other) = delete;
                TCPServer(TCPServer&& other) = delete;
                TCPServer& operator=(const TCPServer& other) = delete;
                TCPServer& operator=(TCPServer&& other) = delete;

                inline void init() {
                    _server.serverBindSocket();
                    _server.listenIncomingConnections();
                    _running = true;
                }

                inline void acceptConnection() {
                    _server.pollEvents();
                    //_clientfds.push_back(fd);
                    //_clients[fd] = true;
                    

                }

                inline bool isRunning() {
                    return _running;
                }

                inline void sendMsg(const std::string& msg) {
                    _server.sendMsg(msg);
                }

                
                /*inline void printClients() {
                    for (auto& client : _clients) {
                        std::cout << _server.getClientIP()<< "\n";
                    }
                }

                inline void getMyIp() {
                    std::cout << _server.getServerIP() << "\n";
                }*/


            private:
                TCPSocket _server;
                //std::vector<int> _clientfds;
                bool _running = false;
                
                std::unordered_map<int, bool> _clients;
                
        };
    }
}