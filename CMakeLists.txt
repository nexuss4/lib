cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

# ❌ Remove main.cpp from library
list(FILTER BIO_SOURCES EXCLUDE REGEX ".*main.cpp$")

# 🔥 Handle problematic file correctly
if(EMSCRIPTEN)
    # remove broken implementation
    list(FILTER BIO_SOURCES EXCLUDE REGEX ".*StringConverter.cpp$")

    # ✅ add stub replacement
    list(APPEND BIO_SOURCES src/util/string/StringConverter_stub.cpp)
endif()

add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

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