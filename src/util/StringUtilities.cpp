//
// Created by DexrnZacAttack on 12/30/25 using zPc-i2.
//
#include "LCE/util/StringUtilities.h"

std::wstring_view
lce::util::StringUtilities::View::trimEndNullBytes(const std::wstring &in) {
    size_t len = in.size();
    while (len && in[len - 1] == L'\0')
        --len;

    // should be faster since afaik it's pretty much equiv of returning bounds
    // of our existing string
    return {in.data(), len};
}

void lce::util::StringUtilities::trimEndNullBytes(std::wstring &in) {
    size_t len = in.size();
    while (len && in[len - 1] == L'\0')
        --len;

    in.resize(len);
}
