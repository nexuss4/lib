cmake_minimum_required(VERSION 3.16)
project(LCE)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# --- Fetch BIO (library only) ---
FetchContent_Declare(
    BIO
    GIT_REPOSITORY https://github.com/nexuss4/lib.git
    GIT_TAG main
)

# Important for WASM
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
endif()

FetchContent_MakeAvailable(BIO)

# --- Your LCE sources ---
file(GLOB_RECURSE LCE_SOURCES src/*.cpp)
file(GLOB_RECURSE LCE_HEADERS include/*.h)

# --- Build LCE as STATIC (important for WASM) ---
add_library(LCE STATIC ${LCE_SOURCES} ${LCE_HEADERS})

target_include_directories(LCE PUBLIC include)

target_link_libraries(LCE PRIVATE bio)

# =========================================================
# 🔥 THIS IS THE PART YOU WERE MISSING (WASM EXECUTABLE)
# =========================================================
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    message(STATUS "Building WASM executable")

    add_executable(LCE_wasm
        src/bindings/EmscriptenBindings.cpp
    )

    target_link_libraries(LCE_wasm PRIVATE LCE)

    # Output = LCE.js + LCE.wasm
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
