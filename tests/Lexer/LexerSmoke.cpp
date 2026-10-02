#include "hyperlang/Parse/Lexer.h"

#include <iostream>
#include <string>

int main() {
  const std::string source =
      "@import Foundation\n"
      "func main() {\n"
      "  let message = \"Hello, 世界\"\n"
      "  output.print(message)\n"
      "  let value = 0x2A + 10\n"
      "}\n";

  hyperlang::Lexer lexer(source);

  for (;;) {
    const hyperlang::Token token = lexer.lex();

    std::cout << hyperlang::tok::getTokenName(token.kind)
              << " : " << token.text << '\n';

    if (token.kind == hyperlang::tok::Kind::EndOfFile)
      break;
  }

  return lexer.hasErrors() ? 1 : 0;
}
