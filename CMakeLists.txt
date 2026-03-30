cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 🔥 GLOBAL include fix
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

list(FILTER BIO_SOURCES EXCLUDE REGEX ".*main.cpp$")

add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

target_compile_definitions(bio PUBLIC
    BIO_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    BIO_COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
    BIO_PLATFORM_NAME="${CMAKE_SYSTEM_NAME}"
    BIO_PLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}"
    BIO_VERSION="1.0.0"
)

add_executable(bio_wasm wasm/main.cpp)

target_link_libraries(bio_wasm PRIVATE bio)

if(EMSCRIPTEN)
    set_target_properties(bio_wasm PROPERTIES SUFFIX ".js")

    target_link_options(bio_wasm PRIVATE
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sNO_EXIT_RUNTIME=1"
        "-sASSERTIONS=1"
        "-sEXPORTED_FUNCTIONS=['_main']"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    )
endif()