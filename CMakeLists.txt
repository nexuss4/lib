cmake_minimum_required(VERSION 3.16)
project(BIO VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files (adjust if paths differ)
set(BIO_SOURCES
    src/buffer/BinaryBuffer.cpp
    src/buffer/SizedBinaryBuffer.cpp
)

set(BIO_HEADERS
    include/BinaryIO/buffer/BinaryBuffer.h
    include/BinaryIO/buffer/SizedBinaryBuffer.h
)

# Build STATIC for WASM, SHARED otherwise
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_library(bio STATIC ${BIO_SOURCES} ${BIO_HEADERS})
else()
    add_library(bio SHARED ${BIO_SOURCES} ${BIO_HEADERS})
endif()

# Include directories
target_include_directories(bio
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional: define export macro if you use one
target_compile_definitions(bio PRIVATE BIO_BUILD)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    add_executable(LCE_wasm
        src/bindings/EmscriptenBindings.cpp
    )

    target_link_libraries(LCE_wasm PRIVATE LCE)

    set_target_properties(LCE_wasm PROPERTIES
        OUTPUT_NAME "LCE"
        SUFFIX ".js"
    )

    target_link_options(LCE_wasm PRIVATE
        "-sMODULARIZE=1"
        "-sEXPORT_ES6=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
    )

endif()
