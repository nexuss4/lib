cmake_minimum_required(VERSION 3.26)
project(LCE VERSION 1.2.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")

include(FetchContent)

# =========================================================
# 🔗 Fetch BIO (your cleaned repo)
# =========================================================
FetchContent_Declare(
    BIO
    GIT_REPOSITORY https://github.com/nexuss4/lib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(BIO)

# =========================================================
# 🧱 External sources
# =========================================================
file(GLOB_RECURSE LODEPNG_FILES external/lodepng/*.cpp)
file(GLOB_RECURSE ZLIB_FILES external/zlib/*.c)

# =========================================================
# 📦 Headers + Sources
# =========================================================
file(GLOB_RECURSE HEADERS include/LCE/*.h)
file(GLOB_RECURSE FILES src/*.cpp)

# =========================================================
# 🧠 Debug define
# =========================================================
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCMAKE_BUILD_DEBUG)
endif()

# =========================================================
# 💻 Native build
# =========================================================
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    add_library(LCE SHARED
        ${HEADERS}
        ${FILES}
        ${LODEPNG_FILES}
        ${ZLIB_FILES}
    )

    target_include_directories(LCE
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(LCE PRIVATE bio)

endif()

# =========================================================
# 🌐 WASM build (THIS is the important part)
# =========================================================
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    # --- core library ---
    add_library(LCE STATIC
        ${HEADERS}
        ${FILES}
        ${LODEPNG_FILES}
    )

    target_include_directories(LCE
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(LCE PRIVATE bio)

    # --- WASM entry point ---
    add_executable(LCE_wasm
        src/bindings/EmscriptenBindings.cpp
    )

    target_link_libraries(LCE_wasm PRIVATE LCE)

    # Output as JS module
    set_target_properties(LCE_wasm PROPERTIES
        OUTPUT_NAME "LCE"
        SUFFIX ".js"
    )

    # Emscripten flags
    target_link_options(LCE_wasm PRIVATE
        "-sMODULARIZE=1"
        "-sEXPORT_ES6=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
        "-sENVIRONMENT=web"
    )

endif()

# =========================================================
# 🧾 Metadata
# =========================================================
target_compile_definitions(LCE PUBLIC
    LIBLCE_COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
    LIBLCE_PLATFORM_NAME="${CMAKE_SYSTEM_NAME}"
    LIBLCE_PLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}"
    LIBLCE_VERSION="${PROJECT_VERSION}"
)
