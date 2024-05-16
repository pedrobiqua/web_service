cmake_minimum_required(VERSION 3.22.1)
project(web_service CXX)

# Define a versão mais recente do C++
set(CMAKE_CXX_STANDARD 20)  # Você pode alterar para a versão desejada, como 20
set(CMAKE_TOOLCHAIN_FILE "/home/pedro/projects/web_service/build/build/Release/generators/conan_toolchain.cmake")

find_package(Crow REQUIRED)
find_package(CURL REQUIRED)

# Adiciona o executável 'web_service' usando o arquivo 'main.cpp'
add_executable(web_service src/main.cpp)

target_link_libraries(web_service Crow::Crow)
target_link_libraries(web_service CURL::libcurl)
