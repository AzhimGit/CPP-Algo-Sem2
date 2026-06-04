#set vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

cmake_minimum_required(VERSION 3.16)
project(PosyanduCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)
find_package(libpqxx CONFIG REQUIRED) 

add_executable(posyandu main.cpp)


target_link_libraries(posyandu PRIVATE 
    PostgreSQL::PostgreSQL 
    libpqxx::pqxx  
)


if(MSVC)
    target_compile_options(posyandu PRIVATE /W4 /permissive-)
else()
    target_compile_options(posyandu PRIVATE -Wall -Wextra -Wpedantic)
endif()