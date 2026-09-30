#include "hyper/Parser.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace hyper::parse {

struct ConfusableEntry {
    char32_t canonical;
    char32_t alternative;
};

static const std::vector<ConfusableEntry> confusableEntries = {
    {U'a', U'а'},
    {U'c', U'с'},
    {U'e', U'е'},
    {U'o', U'о'},
    {U'p', U'р'},
    {U'x', U'х'},
};

bool isConfusableCharacter(char32_t value) {
    for (const auto& entry : confusableEntries) {
        if (entry.alternative == value) {
            return true;
        }
    }
    return false;
}

char32_t canonicalConfusableCharacter(char32_t value) {
    for (const auto& entry : confusableEntries) {
        if (entry.alternative == value) {
            return entry.canonical;
        }
    }
    return value;
}

} // namespace hyper::parse
