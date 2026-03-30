#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

// 🔥 SAFE includes (work with your structure now)
#include "libLCEExports.h"
#include <BinaryIO/Exports.h>

#include "tests/formats.h"
#include "tests/vfs.h"
#include <util.h>

#ifndef __EMSCRIPTEN__
#include <filesystem>
#endif

int main(int argc, char **argv) {

    std::cout << bio::bio_get_library_string() << std::endl;
    std::cout << lce::lce_get_library_string() << std::endl;

#ifndef __EMSCRIPTEN__
    std::filesystem::create_directories(lce::tests::util::examples);
    std::filesystem::create_directories(lce::tests::util::output);
#else
    std::cout << "[WASM] filesystem disabled\n";
#endif

    // 🔥 IMPORTANT: keeps code from being stripped
    ADD_TESTS(RUN_FORMATS_TESTS, lce::tests::formats::run);
    ADD_TESTS(RUN_VFS_TESTS, lce::tests::vfs::run);

    return 0;
}