#include "hyper/Sema/Sema.h"
#include <string>

namespace hyper::sema {

bool validateDeclarationName(std::string_view name) {
    if (name.empty()) {
        return false;
    }
    if (!(name.front() == '_' ||
          (name.front() >= 'A' && name.front() <= 'Z') ||
          (name.front() >= 'a' && name.front() <= 'z'))) {
        return false;
    }
    for (char character : name) {
        if (!(character == '_' ||
              (character >= 'A' && character <= 'Z') ||
              (character >= 'a' && character <= 'z') ||
              (character >= '0' && character <= '9'))) {
            return false;
        }
    }
    return true;
}

} // namespace hyper::sema
