#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_DIAGNOSTICS_H
#define HYPER_LIB_PARSE_LEXER_DIAGNOSTICS_H
#include "hyper/lib/Parse/SourceLocation.h"
#include <cstdint>
#include <functional>
#include <string>
namespace hyper::parse {
enum class DiagnosticSeverity : std::uint8_t { Note, Warning, Error, Fatal };
struct LexerDiagnostic {
    DiagnosticSeverity severity = DiagnosticSeverity::Error;
    SourceRange range{};
    std::string message{};
};
}
#endif
