//===--- ParseSupport.h - HyperLang Parser Support ------------------------===//
#ifndef HYPERLANG_PARSE_PARSE_SUPPORT_H
#define HYPERLANG_PARSE_PARSE_SUPPORT_H
#include "hyperlang/Parse/TokenKinds.h"
#include <string_view>
namespace hyperlang::parse {
bool isDeclarationStarter(tok::Kind kind);
bool isDeclarationNameToken(tok::Kind kind);
bool isExpressionNameToken(tok::Kind kind);
bool isBuiltinTypeToken(tok::Kind kind);
bool isStatementStarter(tok::Kind kind);
bool isPatternToken(tok::Kind kind);
bool isGenericDelimiter(tok::Kind kind);
int expressionPrecedence(tok::Kind kind);
bool containsUnicodeConfusable(std::string_view text);
bool isVersionComponent(std::string_view text);
bool isIfConfigDirective(tok::Kind kind);
bool isRequestKeyword(tok::Kind kind);
bool isAssignmentOperator(tok::Kind kind);
bool isUnaryOperator(tok::Kind kind);
bool isRegexDelimiter(char ch);
}
#endif
