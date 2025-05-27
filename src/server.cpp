#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "../build/generated/chat.grpc.pb.h"

class ChatServiceImpl final : public chat::ChatService::ChatService::Service
{
public:
    grpc::Status SendMessage(grpc::ServerContext* context, const chat::ChatMessage* request, chat::ChatReply* reply) override 
    {
        std::cout << "Received from " << request->name() << ": " << request->message() << "\n";
        reply->set_response("Echo: " + request->message());
        return grpc::Status::OK;
    }
};

class ChatServer 
{
public:
    ChatServer(std::string address) : m_address(address) {}

    void start()
    {
        grpc::ServerBuilder serverBuilder;
        serverBuilder
        .AddListeningPort(m_address, grpc::InsecureServerCredentials())
        .RegisterService(&m_service);

        m_server = serverBuilder.BuildAndStart();
        std::cout << "Server listening on " << m_address << "\n";
        m_server->Wait();
    }

private:
    std::string m_address;
    ChatServiceImpl m_service;
    std::unique_ptr<grpc::Server> m_server;

};

int main()
{
    ChatServer server{"0.0.0.0:8080"};
    server.start();
    return 0;
}