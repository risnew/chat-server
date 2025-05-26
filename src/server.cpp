#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

constexpr int PORT{8080};

class Server
{
public:
    Server() :
        m_socket(socket(AF_INET, SOCK_STREAM, 0))
    {}
    
    ~Server()
    {
        close(m_socket);
    }


    void start()
    {
        //publish itself
        sockaddr_in address{};
        address.sin_addr.s_addr=INADDR_ANY;
        address.sin_family=AF_INET;
        address.sin_port=htons(PORT);

        if (bind(m_socket, (sockaddr*)&address, sizeof(address)) < 0)
        {
            std::cerr << "The server cannot be started\n";
            std::abort();
        }

        if(listen(m_socket, SOMAXCONN) < 0)
        {
            std::cerr << "The server cannot listen for connections.\n";
            close(m_socket);
            std::abort();
        }

        sockaddr_in clientAddress;
        socklen_t clientSize = sizeof(clientAddress); 
        int clientSocket = accept(m_socket, (sockaddr*)&clientAddress, &clientSize);
        if(clientSocket < 0)
        {
            std::cerr << "Failed to accept client connection.\n";
            std::abort();
        }
        const char* msg = "Welcome";
        send(clientSocket, msg, strlen(msg), 0);
        close(clientSocket);
        std::cout << "Server started on port " << PORT << "\n";
    }

private:
    int m_socket;
};

int main() {
    std::cout << "Server starting..." << std::endl;
    Server server;
    server.start();
    return 0;
}