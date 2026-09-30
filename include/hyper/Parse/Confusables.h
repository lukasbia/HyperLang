#pragma once

#include <string_view>

namespace hyper::parse {

class ConfusableIdentifierChecker {
public:
    static bool containsConfusable(std::string_view identifier) noexcept;
    static bool containsMixedScript(std::string_view identifier) noexcept;
    static bool isSafeIdentifier(std::string_view identifier) noexcept;
    static std::string_view canonicalForm(std::string_view identifier) noexcept;
};

} // namespace hyper::parse
