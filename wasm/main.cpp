#include <iostream>

#ifdef __EMSCRIPTEN__
    #include <emscripten/emscripten.h>
#endif

// 🔥 FIXED includes (important)
#include <LCE/libLCEExports.h>
#include <BinaryIO/Exports.h>

#include "tests/formats.h"
#include "tests/vfs.h"
#include <util.h>

// ⚠️ filesystem is limited in WASM
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
    std::cout << "[WASM] Skipping filesystem directory creation\n";
#endif

    // 🔥 KEEP TESTS (prevents dead-code stripping)
    ADD_TESTS(RUN_FORMATS_TESTS, lce::tests::formats::run);
    ADD_TESTS(RUN_VFS_TESTS, lce::tests::vfs::run);

#ifdef __EMSCRIPTEN__
    std::cout << "[WASM] Execution complete\n";
#endif

    return 0;
}
