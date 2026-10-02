#include "hyperlang/Parse/ASTDump.h"
#include "hyperlang/Parse/Lexer.h"
#include "hyperlang/Parse/Parser.h"
#include "hyperlang/Parse/TokenKinds.h"

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace {

enum class Mode {
  Parse,
  Tokens,
  AST,
};

struct Options {
  Mode mode = Mode::Parse;
  std::string input;
  std::string output;
};

void printUsage(std::ostream &out) {
  out << "usage: hyperlangc [--tokens | --ast] <file>\n";
}

bool parseOptions(int argc, char **argv, Options &options) {
  for (int i = 1; i < argc; ++i) {
    std::string_view argument(argv[i]);
    if (argument == "--tokens") {
      options.mode = Mode::Tokens;
      continue;
    }
    if (argument == "--ast") {
      options.mode = Mode::AST;
      continue;
    }
    if (argument == "-o") {
      if (i + 1 >= argc) {
        std::cerr << "hyperlangc: '-o' requires an output path\n";
        return false;
      }
      options.output = argv[++i];
      continue;
    }
    if (argument == "--help" || argument == "-h") {
      printUsage(std::cout);
      options.input.clear();
      return false;
    }
    if (!argument.empty() && argument.front() == '-') {
      std::cerr << "hyperlangc: unknown option '" << argument << "'\n";
      return false;
    }
    if (!options.input.empty()) {
      std::cerr << "hyperlangc: multiple input files are not supported yet\n";
      return false;
    }
    options.input = std::string(argument);
  }

  if (options.input.empty()) {
    printUsage(std::cerr);
    return false;
  }
  return true;
}

bool readFile(const std::string &path, std::string &source) {
  std::ifstream input(path, std::ios::binary);
  if (!input)
    return false;

  source.assign(std::istreambuf_iterator<char>(input),
                std::istreambuf_iterator<char>());
  return true;
}

const char *severityName(hyperlang::Diagnostic::Severity severity) {
  switch (severity) {
  case hyperlang::Diagnostic::Severity::Note:
    return "note";
  case hyperlang::Diagnostic::Severity::Warning:
    return "warning";
  case hyperlang::Diagnostic::Severity::Error:
    return "error";
  }
  return "error";
}

void printDiagnostics(const hyperlang::DiagnosticList &diagnostics,
                      std::string_view path) {
  for (const auto &diagnostic : diagnostics) {
    std::cerr << path << ':' << diagnostic.location.line << ':'
              << diagnostic.location.column << ": "
              << severityName(diagnostic.severity) << ": "
              << diagnostic.message << '\n';
  }
}

int runTokens(std::string_view source, std::string_view path) {
  hyperlang::Lexer lexer(source);
  lexer.setRetainComments(true);

  for (;;) {
    hyperlang::Token token = lexer.lex();
    std::cout << token.range.start.line << ':'
              << token.range.start.column << '-'
              << token.range.end.line << ':'
              << token.range.end.column << "  "
              << hyperlang::tok::getTokenName(token.kind) << "  "
              << token.text << '\n';

    if (token.kind == hyperlang::tok::Kind::EndOfFile)
      break;
  }

  printDiagnostics(lexer.diagnostics(), path);
  return lexer.hasErrors() ? 1 : 0;
}

int runParser(std::string_view source, std::string_view path, bool dumpAST,
              std::string_view outputPath) {
  hyperlang::Parser parser(source);
  hyperlang::ParserResult result = parser.parse();

  if (dumpAST && result.sourceFile) {
    if (outputPath.empty()) {
      hyperlang::ast::dump(*result.sourceFile, std::cout);
    } else {
      std::ofstream output(std::string(outputPath), std::ios::binary);
      if (!output) {
        std::cerr << "hyperlangc: cannot create '" << outputPath << "'\n";
        return 1;
      }
      hyperlang::ast::dump(*result.sourceFile, output);
    }
  }

  printDiagnostics(result.diagnostics, path);
  return result.hasErrors() ? 1 : 0;
}

} // namespace

int main(int argc, char **argv) {
  Options options;
  if (!parseOptions(argc, argv, options))
    return options.input.empty() && argc > 1 &&
                   (std::string_view(argv[1]) == "--help" ||
                    std::string_view(argv[1]) == "-h")
               ? 0
               : 2;

  std::string source;
  if (!readFile(options.input, source)) {
    std::cerr << "hyperlangc: cannot read '" << options.input << "'\n";
    return 1;
  }

  if (options.mode == Mode::Tokens)
    return runTokens(source, options.input);

  return runParser(source, options.input, options.mode == Mode::AST,
                    options.output);
}
