# === WAJIB: Set vcpkg toolchain (harus sebelum project()) ===
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

cmake_minimum_required(VERSION 3.16)
project(PosyanduCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === Cari dependency ===
find_package(PostgreSQL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)  # ← CONFIG penting untuk vcpkg

add_executable(posyandu main.cpp)

# === Link library ===
target_link_libraries(posyandu PRIVATE 
    PostgreSQL::PostgreSQL 
    libpqxx::pqxx  # ← Nama target yang benar sesuai output vcpkg
)

# === Optional: Tambahkan warning flags untuk development ===
if(MSVC)
    target_compile_options(posyandu PRIVATE /W4 /permissive-)
else()
    target_compile_options(posyandu PRIVATE -Wall -Wextra -Wpedantic)
endif()