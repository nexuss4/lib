cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

# 🔍 Get all source files
file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

# Debug (you can remove later)
message(STATUS "BIO SOURCES: ${BIO_SOURCES}")

# 🔹 Build library
add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})

target_include_directories(bio
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(bio PUBLIC
    BIO_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    BIO_COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
    BIO_PLATFORM_NAME="${CMAKE_SYSTEM_NAME}"
    BIO_PLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}"
    BIO_VERSION="1.0.0"
)

# 🔥 WASM executable
add_executable(bio_wasm src/main.cpp)

# ✅ Force include entire library
target_link_libraries(bio_wasm PRIVATE
    "-Wl,--whole-archive"
    bio
    "-Wl,--no-whole-archive"
)

# ⚙️ Emscripten settings
if(EMSCRIPTEN)
    set_target_properties(bio_wasm PROPERTIES SUFFIX ".js")

    target_link_options(bio_wasm PRIVATE
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXPORTED_FUNCTIONS=['_main']"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
        "-sNO_EXIT_RUNTIME=1"
    )
endif()
