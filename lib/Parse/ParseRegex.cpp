#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
bool isRegexDelimiter(char ch) {
  return ch == '/' || ch == 96;
}
}
