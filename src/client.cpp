#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "chat.grpc.pb.h"

class ChatClient
{
public:
    ChatClient(std::shared_ptr<grpc::Channel> channel) : m_stub(chat::ChatService::NewStub(channel)) {}
    
    std::string SendMessage(const std::string& name, const std::string& message)
    {
        chat::ChatMessage request;
        request.set_name(name);
        request.set_message(message);

        chat::ChatReply reply;

        grpc::ClientContext context;

        grpc::Status status = m_stub->SendMessage(&context, request, &reply);

        if(status.ok())
        {
            return reply.response();
        } 
        else 
        {
            std::cerr << "RPC failed: " << status.error_message() << "\n";
            return "RPC failed";
        }
    }
private:
    std::unique_ptr<chat::ChatService::Stub> m_stub;
};

int main(int numberOfArguments, char ** argv) 
{
    if (numberOfArguments != 3u)
    {
        std::cerr << "Usage: client <name> <message>\n";
        std::abort();
    }

    std::string name = argv[1];
    std::string message = argv[2];

    ChatClient client(grpc::CreateChannel("0.0.0.0:8080", grpc::InsecureChannelCredentials()));
    std::string reply = client.SendMessage(name, message);
    std::cout << "Server replied: " << reply << "\n";
}