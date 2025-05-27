#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "../build/generated/chat.grpc.pb.h"

class ChatServiceImpl final : public chat::ChatService::ChatService::Service
{
public:
    grpc::Status Chat(grpc::ServerContext* context, grpc::ServerReaderWriter<chat::ChatReply, chat::ChatMessage>* stream) override 
    {  
        {        
            std::lock_guard<std::mutex> lock(m_mutex);
            m_streams.insert(stream);
        }

        chat::ChatMessage message;
        while(stream->Read(&message))
        {
            std::string name = message.name();
            std::string text = message.message();

            std::cout << name << ": " << text << "\n";

            chat::ChatReply reply;
            reply.set_response(name);
            reply.set_message(text);

            broadcastMessage(reply);
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_streams.erase(stream);
        }

        return grpc::Status::OK;
    }


private:

    void broadcastMessage(chat::ChatReply reply)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for(auto* stream : m_streams)
        {
            stream->Write(reply);
        }
    }

    std::set<grpc::ServerReaderWriter<chat::ChatReply, chat::ChatMessage>*> m_streams;
    std::mutex m_mutex;
};

class ChatServer 
{
public:
    ChatServer(const std::string& address) : m_address(address) {}

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

    const std::string m_address;
    ChatServiceImpl m_service;
    std::unique_ptr<grpc::Server> m_server;

};

int main()
{
    ChatServer server{"0.0.0.0:8080"};
    server.start();
    return 0;
}