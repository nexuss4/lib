#include <string>

namespace bio {
namespace util {
namespace string {

std::string to_utf8(const std::u16string& input) {
    std::string result;
    for (char16_t c : input) {
        result.push_back(static_cast<char>(c));
    }
    return result;
}

std::u16string from_utf8(const std::string& input) {
    std::u16string result;
    for (char c : input) {
        result.push_back(static_cast<char16_t>(c));
    }
    return result;
}

} // namespace string
} // namespace util
} // namespace bio