#pragma once
#include "hyperlang/Lexer/Token.h"
#include <string_view>
#include <vector>
namespace hyperlang::lexer {
class Lexer {
public:
 explicit Lexer(std::string_view source);
 std::vector<Token> tokenize();
private:
 std::string_view source_; std::size_t index_=0; SourceLocation location_{};
 char peek(std::size_t n=0) const; char advance(); void skipWhitespace();
 Token identifier(); Token number(); Token string(); Token directive(); Token symbol();
};
} // namespace hyperlang::lexer
