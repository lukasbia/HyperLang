#include "hyperlang/Parse/ParseSupport.h"
#include <cctype>
namespace hyperlang::parse {
bool isVersionComponent(std::string_view text) {
  if (text.empty()) return false;
  for (unsigned char c : text)
    if (!std::isdigit(c)) return false;
  return true;
}
}
