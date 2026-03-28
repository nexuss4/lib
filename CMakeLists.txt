cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

# Collect sources
file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

# Build ONLY bio (nothing else)
add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

target_include_directories(bio
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Required defines (fixes your earlier compile errors)
target_compile_definitions(bio PUBLIC
    BIO_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    BIO_COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
    BIO_PLATFORM_NAME="${CMAKE_SYSTEM_NAME}"
    BIO_PLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}"
    BIO_VERSION="1.0.0"
)
