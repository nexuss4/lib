//
// Created by DexrnZacAttack on 12/30/25 using zPc-i2.
//
#ifndef LCE_STRINGUTILITIES_H
#define LCE_STRINGUTILITIES_H
#include <string>

namespace lce::util {
    class StringUtilities {
      public:
        static void trimEndNullBytes(std::wstring &in);

        class View {
          public:
            static std::wstring_view trimEndNullBytes(const std::wstring &in);
        };
    };
} // namespace lce::util

#endif // LCE_STRINGUTILITIES_H