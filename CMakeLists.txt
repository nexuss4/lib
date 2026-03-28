cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

target_include_directories(bio
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
