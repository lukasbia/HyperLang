#pragma once
#include <cstddef>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "hyper/Parse/Lexer.h"
namespace hyper {
enum class SyntaxKind {
 TranslationUnit,Declaration,ImportDeclaration,FunctionDeclaration,VariableDeclaration,TypeDeclaration,StructDeclaration,ClassDeclaration,
 EnumDeclaration,ProtocolDeclaration,ExtensionDeclaration,ParameterClause,Parameter,GenericParameterClause,GenericParameter,ReturnClause,
 Attribute,Modifier,Statement,CompoundStatement,ExpressionStatement,IfStatement,WhileStatement,ForStatement,DoStatement,GuardStatement,
 SwitchStatement,CaseStatement,BreakStatement,ContinueStatement,ReturnStatement,ThrowStatement,DeferStatement,DeclarationStatement,
 Expression,IdentifierExpression,LiteralExpression,PrefixExpression,PostfixExpression,BinaryExpression,CallExpression,MemberExpression,
 SubscriptExpression,AssignmentExpression,TupleExpression,ArrayExpression,DictionaryExpression,ClosureExpression,StringExpression,
 TypeAnnotation,NamedType,OptionalType,FunctionType,TupleType,ArrayType,GenericType,Unknown
};
struct SyntaxNode { SyntaxKind kind=SyntaxKind::Unknown; std::string text; SourceLocation location; std::vector<std::unique_ptr<SyntaxNode>> children; explicit SyntaxNode(SyntaxKind k):kind(k){} };
struct ParserOptions { bool allowTopLevelCode=true; bool parseGenerics=true; bool parseAttributes=true; bool recoverErrors=true; std::size_t maximumDiagnostics=256; };
struct ParseDiagnostic { enum class Severity { Note,Warning,Error,Fatal }; Severity severity=Severity::Error; SourceLocation location; std::string message; std::string fixIt; };
class Parser {
public:
 explicit Parser(std::string_view); explicit Parser(std::vector<Token>);
 std::unique_ptr<SyntaxNode> parse(); std::unique_ptr<SyntaxNode> parseTranslationUnit();
 const std::vector<ParseDiagnostic>& diagnostics() const noexcept; bool hasErrors() const noexcept; void clearDiagnostics();
 void setOptions(const ParserOptions&); const ParserOptions& options() const noexcept; bool atEnd() const noexcept;
private:
 std::vector<Token> tokens_; std::size_t index_=0; ParserOptions options_{}; std::vector<ParseDiagnostic> diagnostics_;
 const Token& current()const; const Token& previous()const; const Token& peek(std::size_t=0)const;
 bool check(TokenKind)const; bool checkAny(std::initializer_list<TokenKind>)const; bool consume(TokenKind); bool consumeAny(std::initializer_list<TokenKind>);
 const Token& advance(); bool expect(TokenKind,std::string_view); void diagnose(SourceLocation,ParseDiagnostic::Severity,std::string);
 void synchronize(); void synchronizeStatement(); void synchronizeDeclaration();
 std::unique_ptr<SyntaxNode> parseDeclaration(); std::unique_ptr<SyntaxNode> parseImportDeclaration(); std::unique_ptr<SyntaxNode> parseFunctionDeclaration();
 std::unique_ptr<SyntaxNode> parseVariableDeclaration(); std::unique_ptr<SyntaxNode> parseTypeDeclaration(); std::unique_ptr<SyntaxNode> parseStructDeclaration();
 std::unique_ptr<SyntaxNode> parseClassDeclaration(); std::unique_ptr<SyntaxNode> parseEnumDeclaration(); std::unique_ptr<SyntaxNode> parseProtocolDeclaration();
 std::unique_ptr<SyntaxNode> parseExtensionDeclaration(); std::unique_ptr<SyntaxNode> parseStatement(); std::unique_ptr<SyntaxNode> parseCompoundStatement();
 std::unique_ptr<SyntaxNode> parseIfStatement(); std::unique_ptr<SyntaxNode> parseWhileStatement(); std::unique_ptr<SyntaxNode> parseForStatement();
 std::unique_ptr<SyntaxNode> parseDoStatement(); std::unique_ptr<SyntaxNode> parseGuardStatement(); std::unique_ptr<SyntaxNode> parseSwitchStatement();
 std::unique_ptr<SyntaxNode> parseReturnStatement(); std::unique_ptr<SyntaxNode> parseThrowStatement(); std::unique_ptr<SyntaxNode> parseDeferStatement();
 std::unique_ptr<SyntaxNode> parseExpressionStatement(); std::unique_ptr<SyntaxNode> parseExpression(); std::unique_ptr<SyntaxNode> parseAssignmentExpression();
 std::unique_ptr<SyntaxNode> parseBinaryExpression(int); std::unique_ptr<SyntaxNode> parsePrefixExpression(); std::unique_ptr<SyntaxNode> parsePostfixExpression();
 std::unique_ptr<SyntaxNode> parsePrimaryExpression(); std::unique_ptr<SyntaxNode> parseIdentifierExpression(); std::unique_ptr<SyntaxNode> parseLiteralExpression();
 std::unique_ptr<SyntaxNode> parseCallExpression(std::unique_ptr<SyntaxNode>); std::unique_ptr<SyntaxNode> parseMemberExpression(std::unique_ptr<SyntaxNode>);
 std::unique_ptr<SyntaxNode> parseSubscriptExpression(std::unique_ptr<SyntaxNode>); std::unique_ptr<SyntaxNode> parseClosureExpression();
 std::unique_ptr<SyntaxNode> parseTupleExpression(); std::unique_ptr<SyntaxNode> parseArrayExpression(); std::unique_ptr<SyntaxNode> parseDictionaryExpression();
 std::unique_ptr<SyntaxNode> parseType(); std::unique_ptr<SyntaxNode> parseNamedType(); std::unique_ptr<SyntaxNode> parseGenericType(std::unique_ptr<SyntaxNode>);
 std::unique_ptr<SyntaxNode> parseFunctionType(); std::unique_ptr<SyntaxNode> parseTupleType(); std::unique_ptr<SyntaxNode> parseTypeAnnotation();
 std::unique_ptr<SyntaxNode> parseParameterClause(); std::unique_ptr<SyntaxNode> parseParameter(); std::unique_ptr<SyntaxNode> parseGenericParameterClause();
 std::unique_ptr<SyntaxNode> parseAttribute(); std::unique_ptr<SyntaxNode> parseModifier();
 static bool isDeclarationStart(TokenKind)noexcept; static bool isStatementStart(TokenKind)noexcept; static bool isLiteral(TokenKind)noexcept;
 static bool isTypeStart(TokenKind)noexcept; static bool isAssignmentOperator(TokenKind)noexcept; static int precedence(TokenKind)noexcept;
 static SyntaxKind syntaxForToken(TokenKind)noexcept; static std::string tokenText(const Token&);
};
const char* syntaxKindName(SyntaxKind)noexcept;
} // namespace hyper
