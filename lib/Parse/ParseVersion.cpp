#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct VersionComponent {
    unsigned value = 0;
};

struct VersionConstraint {
    std::vector<VersionComponent> components;
    std::string operatorSpelling;
};

bool isVersionComponent(char character) {
    return character >= '0' && character <= '9';
}

} // namespace hyper::parse
