#include "socketlib.h"

namespace hft {
    namespace networklib {
        class TCPClient {
            public:
                TCPClient() : _client{TCPSocket::TCPSocketType::CLIENT} {
                    std::cout << "TCPClient constructor called\n";
                }

                TCPClient(const std::string& ip, const std::string& port) : _client{TCPSocket::TCPSocketType::CLIENT,port} {
                    std::cout << "TCPClient constructor called\n";
                    _client.connectToServer(ip);
                    _connection_initiated = true;
                    //_server_IP = _client.getServerIP();
                }

                ~TCPClient() {
                    std::cout << "TCPClient destructor called\n";
                }

                bool isConnectionInitiated() {
                    return _connection_initiated;
                }

                void sendMsg(const std::string& msg) {
                    _client.sendMsg(msg);
                }

                


            private:
                TCPSocket _client;
                std::string _server_IP;
                bool _connection_initiated = false;
        };
    }
}