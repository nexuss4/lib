cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

# ❌ Remove main.cpp from library
list(FILTER BIO_SOURCES EXCLUDE REGEX ".*main.cpp$")

# 🔥 Handle problematic file correctly
if(EMSCRIPTEN)
    list(FILTER BIO_SOURCES EXCLUDE REGEX ".*StringConverter.cpp$")
    list(APPEND BIO_SOURCES src/util/string/StringConverter_stub.cpp)
endif()

add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

target_include_directories(bio PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 🔥 ✅ ADD THIS BACK (CRITICAL FIX)
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
        "-sEXPORTED_FUNCTIONS=['_main']"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    )
endif()