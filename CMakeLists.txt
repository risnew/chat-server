cmake_minimum_required(VERSION 3.15)
project(ChatServer)

set(CMAKE_CXX_STANDARD 20)

# Paths
set(PROTO_DIR ${CMAKE_SOURCE_DIR}/proto)
set(PROTO_FILE ${PROTO_DIR}/chat.proto)
set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GRPC REQUIRED grpc++)
pkg_check_modules(PROTOBUF REQUIRED protobuf)

# Include generated and library headers
include_directories(${GENERATED_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${gRPC_INCLUDE_DIRS})

# Add executable and link everything
add_executable(chat_server
    src/server.cpp
    ${GENERATED_DIR}/chat.pb.cc
    ${GENERATED_DIR}/chat.grpc.pb.cc
)

add_executable(chat_client src/client.cpp
    ${GENERATED_DIR}/chat.pb.cc
    ${GENERATED_DIR}/chat.grpc.pb.cc)

target_link_libraries(chat_server
    ${GRPC_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
)

target_link_libraries(chat_client
    ${GRPC_LIBRARIES}
    ${PROTOBUF_LIBRARIES})