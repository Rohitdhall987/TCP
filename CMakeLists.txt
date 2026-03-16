cmake_minimum_required(VERSION 3.10)

project(TcpProject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")


add_subdirectory(server)
add_subdirectory(client)


