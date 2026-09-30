#include "hyper/Parser.h"
#include <string>

namespace hyper::parse {

struct RegexLiteral {
    std::string pattern;
    std::string options;
};

bool looksLikeRegexLiteral(const std::string& text) {
    return text.size() >= 2 && text.front() == '/' && text.find('/', 1) != std::string::npos;
}

RegexLiteral splitRegexLiteral(const std::string& text) {
    RegexLiteral result;
    if (!looksLikeRegexLiteral(text)) {
        return result;
    }
    const auto end = text.find_last_of('/');
    result.pattern = text.substr(1, end - 1);
    if (end + 1 < text.size()) {
        result.options = text.substr(end + 1);
    }
    return result;
}

} // namespace hyper::parse
