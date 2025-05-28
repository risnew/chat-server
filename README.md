## Chit chat server
To build this project, follow the steps below:
```
cd build
cmake ../
make
```
This will generate two executables, `chat_server` and `chat_client`, which you can run by `./chat_server` and `./chat_client` respectively.
If you want to update the files under /generated, do `protoc --proto_path=../proto --cpp_out=./generated --grpc_out=./generated --plugin=protoc-gen-gr
pc=<path_to_grpc_cpp_plugin> ../proto/chat.proto` where you can find <path_to_grpc_cpp_plugin> with `which grpc_cpp_plugin`. Most likely you will need to get protoc and other stuff as well. I did this: `sudo apt install -y protobuf-compiler libprotobuf-dev libprotoc-dev libgrpc++-dev grpc-proto`.

