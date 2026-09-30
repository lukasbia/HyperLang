#include "hyper/Parser.h"
#include <cctype>
#include <string>

namespace hyper::parse {

bool isValidDeclarationName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    if (!(std::isalpha(static_cast<unsigned char>(name.front())) || name.front() == '_')) {
        return false;
    }
    for (char character : name) {
        if (!(std::isalnum(static_cast<unsigned char>(character)) || character == '_')) {
            return false;
        }
    }
    return true;
}

std::string normalizeDeclarationName(const std::string& name) {
    if (isValidDeclarationName(name)) {
        return name;
    }
    return "_invalid_" + name;
}

} // namespace hyper::parse
