#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool containsUnicodeConfusable(std::string_view text) {
  static constexpr std::string_view suspicious[] = {
    "\xD0\xB0", "\xD0\xB5", "\xD0\xBE", "\xD1\x80",
    "\xD1\x81", "\xD1\x85", "\xD1\x83"
  };
  for (auto value : suspicious)
    if (text.find(value) != std::string_view::npos) return true;
  return false;
}
}
