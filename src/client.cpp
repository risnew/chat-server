#include <iostream>
#include <string>
#include <string_view>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

constexpr auto SERVER_IP = "127.0.0.1";
constexpr auto PORT = 8080;

class ClientImpl
{
public:
    ClientImpl(std::string_view name) : 
        m_socket(socket(AF_INET, SOCK_STREAM, 0))
    {
        connectUser(name);
    }

    void connectUser(std::string_view name)
    {
        if (establishConnection())
        {
            onSuccessfulConnection();
        }
        else
        {
            onConnectionFailed();
        }
    }

    bool establishConnection()
    {
        sockaddr_in SERVER_NAME{};
        SERVER_NAME.sin_family = AF_INET;
        SERVER_NAME.sin_addr.s_addr = inet_addr(SERVER_IP);
        SERVER_NAME.sin_port = htons(PORT);

        if (connect(m_socket, (sockaddr*)&SERVER_NAME, sizeof(SERVER_NAME)) < 0)
        {
            onConnectionFailed();
            return false;
        }
        return true;
    }

    void onConnectionFailed()
    {
        std::cerr << "Connection establishment failed\n";
        std::abort();
    }

    void onSuccessfulConnection()
    {
        std::cout << "Connected to the server on " << SERVER_IP << ":" << PORT << "\n";
        receiveMessage();
    }

private:
    void receiveMessage()
    {
        char buffer[1024];
        int bytesReceived = recv(m_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived < 0) {
            std::cerr << "Error in receiving data from server\n";
            std::abort();
        }

        buffer[bytesReceived] = '\0';  // Null terminate the string
        std::cout << "Message from server: " << buffer << std::endl;
    }

    int m_socket;
};

int main(int numberOfArguments, char ** args) {
    if (numberOfArguments != 2u)
    {
        std::cerr << "Not supported number of arguments \n"; 
        std::abort();
    }
    std::string name{args[1]};
    std::cout << "Client starting..." << name << std::endl;
    ClientImpl client(name);
    return 0;
}