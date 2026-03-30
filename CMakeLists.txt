cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

list(FILTER BIO_SOURCES EXCLUDE REGEX ".*main.cpp$")

add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

# ✅ ONLY THIS NEEDED
target_include_directories(bio PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(bio_wasm wasm/main.cpp)

target_link_libraries(bio_wasm PRIVATE bio)

if(EMSCRIPTEN)
    set_target_properties(bio_wasm PROPERTIES SUFFIX ".js")

    target_link_options(bio_wasm PRIVATE
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sNO_EXIT_RUNTIME=1"
        "-sEXPORTED_FUNCTIONS=['_main']"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    )
endif()