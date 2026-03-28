cmake_minimum_required(VERSION 3.16)
project(BIO)

set(CMAKE_CXX_STANDARD 17)

# 🔍 Collect library source files
file(GLOB_RECURSE BIO_SOURCES src/*.cpp)
file(GLOB_RECURSE BIO_HEADERS include/*.h)

# ❌ Remove main.cpp from library (CRITICAL)
list(FILTER BIO_SOURCES EXCLUDE REGEX ".*main.cpp$")

# Debug (optional)
message(STATUS "BIO SOURCES: ${BIO_SOURCES}")

# 🔹 Create static library
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

# 🔥 WASM executable (ENTRY POINT)
add_executable(bio_wasm wasm/main.cpp)

# ✅ Link library
target_link_libraries(bio_wasm PRIVATE bio)

# ⚙️ Emscripten settings
if(EMSCRIPTEN)

    # Ensure .js output (with .wasm)
    set_target_properties(bio_wasm PROPERTIES SUFFIX ".js")

    # Prevent stripping + allow JS interop
    target_link_options(bio_wasm PRIVATE
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXPORTED_FUNCTIONS=['_main']"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
        "-sNO_EXIT_RUNTIME=1"

        # 🔥 Force include full library (fixes 1KB wasm)
        "-Wl,--whole-archive"
        "$<TARGET_FILE:bio>"
        "-Wl,--no-whole-archive"
    )

endif()