cmake_minimum_required(VERSION 3.26)
project(LCE VERSION 1.2.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")

include(FetchContent)

# --- Fetch BIO (your fixed repo) ---
FetchContent_Declare(
    BIO
    GIT_REPOSITORY https://github.com/nexuss4/lib.git
    GIT_TAG main
)
FetchContent_MakeAvailable(BIO)

# --- Debug define ---
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCMAKE_BUILD_DEBUG)
endif ()

# --- External files ---
set(LODEPNG_FILES
    external/lodepng/lodepng.cpp
    external/lodepng/lodepng_util.cpp
)

set(ZLIB_FILES
    external/zlib/inflate.c
    external/zlib/adler32.c
    external/zlib/crc32.c
    external/zlib/zutil.c
    external/zlib/inffast.c
    external/zlib/inftrees.c
)

# --- Headers ---
set(HEADERS
    include/LCE/libLCE.h
    include/LCE/libLCEExports.h
    include/LCE/save/SaveFile.h
    include/LCE/save/SaveFileOld.h
    include/LCE/save/SaveFileCommons.h
    include/LCE/save/Thumb.h
    include/LCE/io/Serializable.h
    include/LCE/archive/Archive.h
    include/LCE/color/Color.h
    include/LCE/color/ColorFile.h
    include/LCE/localization/LocalizationFile.h
    include/LCE/localization/Language.h
    include/LCE/soundbank/Soundbank.h
    include/LCE/soundbank/BinkaFile.h
    include/LCE/compression/Compression.h
    include/LCE/util/StringUtilities.h
    include/LCE/filesystem/FSObject.h
    include/LCE/filesystem/Directory.h
    include/LCE/filesystem/Filesystem.h
    include/LCE/filesystem/File.h
    include/LCE/bindings/EmscriptenBindings.h
)

# --- Source files ---
set(FILES
    src/libLCEExports.cpp
    src/bindings/EmscriptenBindings.cpp
    src/save/SaveFile.cpp
    src/filesystem/Filesystem.cpp
    src/archive/Archive.cpp
    src/color/Colors/Color.cpp
    src/color/Colors/WorldColor.cpp
    src/color/ColorFile.cpp
    src/color/ColorFileOld.cpp
    src/localization/LocalizationFile.cpp
    src/localization/Language.cpp
    src/soundbank/Soundbank.cpp
    src/save/SaveFileOld.cpp
    src/save/SaveFileCommons.cpp
    src/compression/Compression.cpp
    src/save/Thumb.cpp
    src/filesystem/FSObject.cpp
    src/filesystem/File.cpp
    src/filesystem/Directory.cpp
    src/io/Serializable.cpp
    src/util/StringUtilities.cpp
)

# =========================================================
# 🔧 NORMAL BUILD (native platforms)
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
# 🌐 WASM BUILD (Emscripten)
# =========================================================
if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    # Build core library STATIC
    add_library(LCE STATIC
        ${HEADERS}
        ${FILES}
        ${LODEPNG_FILES}
    )

    target_include_directories(LCE
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(LCE PRIVATE bio)

    # 🔥 WASM ENTRY POINT
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
        "-sENVIRONMENT=web"
    )

endif()

# =========================================================
# 🔧 Compile definitions
# =========================================================
target_compile_definitions(LCE PUBLIC
    LIBLCE_COMPILER_NAME="${CMAKE_CXX_COMPILER_ID}"
    LIBLCE_PLATFORM_NAME="${CMAKE_SYSTEM_NAME}"
    LIBLCE_PLATFORM_ARCH="${CMAKE_SYSTEM_PROCESSOR}"
    LIBLCE_VERSION="${PROJECT_VERSION}"
)
