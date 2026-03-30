#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <BinaryIO/Exports.h>

#include "tests/formats.h"
#include "tests/vfs.h"
#include <util.h>

#ifndef __EMSCRIPTEN__
#include <filesystem>
#endif

int main(int argc, char **argv) {

    std::cout << bio::bio_get_library_string() << std::endl;

#ifndef __EMSCRIPTEN__
    std::filesystem::create_directories(lce::tests::util::examples);
    std::filesystem::create_directories(lce::tests::util::output);
#endif

    ADD_TESTS(RUN_FORMATS_TESTS, lce::tests::formats::run);
    ADD_TESTS(RUN_VFS_TESTS, lce::tests::vfs::run);

    return 0;
}