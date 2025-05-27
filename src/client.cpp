#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpcpp/alarm.h>
#include "chat.grpc.pb.h"

class ChatClient
{
public:
    ChatClient(std::shared_ptr<grpc::Channel> channel, const std::string& name) : m_stub(chat::ChatService::NewStub(channel)), m_name(name) {}

    void Run()
    {
        grpc::ClientContext context;
        auto stream = m_stub->Chat(&context);

        std::thread reader([&stream]() {
            chat::ChatReply reply;
            while (stream->Read(&reply)) {
                std::cout << reply.response() << ": " << reply.message() << std::endl;
            }
        });

        std::cout << "Enter messages (type 'exit' to quit):\n";

        while (true) 
        {
            std::string text;
            std::getline(std::cin, text);
            if (text == "exit") break;
       
            chat::ChatMessage message;
            message.set_name(m_name);
            message.set_message(text);

            if(!stream->Write(message))
            {
                std::cerr << "Failed to send the message.\n";
                break;
            }
        }

        stream->WritesDone();
        stream->Finish();
        reader.join();

    }
private:

    std::string m_name;
    std::unique_ptr<chat::ChatService::Stub> m_stub;
};

int main(int numberOfArguments, char ** argv) 
{
    std::string name = "User";
    if (numberOfArguments > 1)
    {
        name=argv[1];
    }

    ChatClient client(grpc::CreateChannel("0.0.0.0:8080", grpc::InsecureChannelCredentials()), name);
    client.Run();
    return 0;
}