cmake_minimum_required(VERSION 3.10.2)
project(AAD_Nasional)

set(CMAKE_CXX_STANDARD 11)

# Tambahkan opsi -pthread pada saat linking
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(server Server.cpp)
add_executable(client Client.cpp)
