import Foundation

// Lexer token definitions.
public enum TokenKind: Equatable {
    case identifier
    case integerLiteral
    case floatingLiteral
    case stringLiteral
    case characterLiteral

    case keyword(String)

    case plus
    case minus
    case star
    case slash
    case percent
    case equal
    case equalEqual
    case notEqual
    case less
    case lessEqual
    case greater
    case greaterEqual
    case and
    case or
    case not
    case plusEqual
    case minusEqual
    case starEqual
    case slashEqual
    case percentEqual
    case arrow
    case range
    case rangeEqual

    case leftParen
    case rightParen
    case leftBrace
    case rightBrace
    case leftBracket
    case rightBracket
    case comma
    case colon
    case semicolon
    case dot
    case question
    case exclamation
    case at
    case hash
    case backslash

    case endOfFile
    case unknown
}

// Source position information.
public struct SourcePosition: Equatable {
    public let offset: Int
    public let line: Int
    public let column: Int

    public init(offset: Int, line: Int, column: Int) {
        self.offset = offset
        self.line = line
        self.column = column
    }
}

// Token source range.
public struct SourceRange: Equatable {
    public let start: SourcePosition
    public let end: SourcePosition

    public init(start: SourcePosition, end: SourcePosition) {
        self.start = start
        self.end = end
    }
}

// Token data.
public struct Token: Equatable {
    public let kind: TokenKind
    public let lexeme: String
    public let range: SourceRange

    public init(kind: TokenKind, lexeme: String, range: SourceRange) {
        self.kind = kind
        self.lexeme = lexeme
        self.range = range
    }
}

// Lexer diagnostic severity.
public enum LexerDiagnosticSeverity {
    case error
    case warning
}

// Lexer diagnostic information.
public struct LexerDiagnostic {
    public let severity: LexerDiagnosticSeverity
    public let message: String
    public let range: SourceRange

    public init(
        severity: LexerDiagnosticSeverity,
        message: String,
        range: SourceRange
    ) {
        self.severity = severity
        self.message = message
        self.range = range
    }
}

// Lexer configuration.
public struct LexerConfiguration {
    public var allowUnicodeIdentifiers: Bool
    public var allowSingleQuotedCharacters: Bool
    public var allowNestedBlockComments: Bool

    public init(
        allowUnicodeIdentifiers: Bool = true,
        allowSingleQuotedCharacters: Bool = true,
        allowNestedBlockComments: Bool = true
    ) {
        self.allowUnicodeIdentifiers = allowUnicodeIdentifiers
        self.allowSingleQuotedCharacters = allowSingleQuotedCharacters
        self.allowNestedBlockComments = allowNestedBlockComments
    }
}

// Main source lexer.
public final class Lexer {
    public static let keywordNames: [String] = [
        "func",
        "var",
        "let",
        "type",
        "class",
        "struct",
        "enum",
        "template",
        "proto",
        "extend",
        "public",
        "private",
        "protected",
        "static",
        "final",
        "const",
        "if",
        "else",
        "guard",
        "match",
        "case",
        "default",
        "while",
        "for",
        "break",
        "continue",
        "return",
        "throw",
        "catch",
        "defer",
        "try",
        "new",
        "delete",
        "this",
        "move",
        "copy",
        "borrow",
        "async",
        "await",
        "task",
        "actor",
        "import",
        "export",
        "module",
        "true",
        "false",
        "nil",
        "int",
        "uint",
        "float",
        "double",
        "bool",
        "string",
        "char",
        "operator",
        "auto",
        "init",
        "deinit",
        "in",
        "self"
    ]

    private static let keywords: Set<String> = Set(keywordNames)

    private let source: String
    private let characters: [Character]
    private let configuration: LexerConfiguration

    private var index: Int = 0
    private var offset: Int = 0
    private var line: Int = 1
    private var column: Int = 1

    private var diagnostics: [LexerDiagnostic] = []

    public init(
        source: String,
        configuration: LexerConfiguration = LexerConfiguration()
    ) {
        self.source = source
        self.characters = Array(source)
        self.configuration = configuration
    }

    public func tokenize() -> [Token] {
        var tokens: [Token] = []

        while !isAtEnd {
            skipWhitespaceAndComments()

            if isAtEnd {
                break
            }

            if let token = lexToken() {
                tokens.append(token)
            }
        }

        let position = currentPosition
        tokens.append(
            Token(
                kind: .endOfFile,
                lexeme: "",
                range: SourceRange(start: position, end: position)
            )
        )

        return tokens
    }

    public func tokenizeWithDiagnostics() -> ([Token], [LexerDiagnostic]) {
        let tokens = tokenize()
        return (tokens, diagnostics)
    }

    public func getDiagnostics() -> [LexerDiagnostic] {
        return diagnostics
    }

    // Returns the current source position.
    private var currentPosition: SourcePosition {
        SourcePosition(
            offset: offset,
            line: line,
            column: column
        )
    }

    // Checks whether the input is exhausted.
    private var isAtEnd: Bool {
        index >= characters.count
    }

    // Reads the current character without consuming it.
    private func peek() -> Character? {
        guard !isAtEnd else {
            return nil
        }

        return characters[index]
    }

    // Reads the next character without consuming the current character.
    private func peekNext() -> Character? {
        let nextIndex = index + 1

        guard nextIndex < characters.count else {
            return nil
        }

        return characters[nextIndex]
    }

    // Reads the character at a relative position.
    private func peek(relativeToCurrent distance: Int) -> Character? {
        let target = index + distance

        guard target >= 0 && target < characters.count else {
            return nil
        }

        return characters[target]
    }

    // Consumes the current character.
    @discardableResult
    private func advance() -> Character? {
        guard !isAtEnd else {
            return nil
        }

        let character = characters[index]
        index += 1
        offset += 1

        if character == "\n" {
            line += 1
            column = 1
        } else {
            column += 1
        }

        return character
    }

    // Consumes a character when it matches.
    @discardableResult
    private func match(_ expected: Character) -> Bool {
        guard peek() == expected else {
            return false
        }

        advance()
        return true
    }

    // Reads a token from the current position.
    private func lexToken() -> Token? {
        let start = currentPosition
        let character = peek()

        guard let character else {
            return nil
        }

        if isIdentifierStart(character) {
            return lexIdentifierOrKeyword(start: start)
        }

        if isDigit(character) {
            return lexNumber(start: start)
        }

        switch character {
        case "\"":
            return lexString(start: start)

        case "'":
            return lexCharacter(start: start)

        case "+":
            return lexPlus(start: start)

        case "-":
            return lexMinus(start: start)

        case "*":
            return lexStar(start: start)

        case "/":
            return lexSlash(start: start)

        case "%":
            return lexPercent(start: start)

        case "=":
            return lexEqual(start: start)

        case "!":
            return lexExclamation(start: start)

        case "<":
            return lexLess(start: start)

        case ">":
            return lexGreater(start: start)

        case "&":
            return lexAmpersand(start: start)

        case "|":
            return lexPipe(start: start)

        case ".":
            return lexDot(start: start)

        case "(":
            return makeSingleCharacterToken(.leftParen, start: start)

        case ")":
            return makeSingleCharacterToken(.rightParen, start: start)

        case "{":
            return makeSingleCharacterToken(.leftBrace, start: start)

        case "}":
            return makeSingleCharacterToken(.rightBrace, start: start)

        case "[":
            return makeSingleCharacterToken(.leftBracket, start: start)

        case "]":
            return makeSingleCharacterToken(.rightBracket, start: start)

        case ",":
            return makeSingleCharacterToken(.comma, start: start)

        case ":":
            return makeSingleCharacterToken(.colon, start: start)

        case ";":
            return makeSingleCharacterToken(.semicolon, start: start)

        case "?":
            return makeSingleCharacterToken(.question, start: start)

        case "@":
            return makeSingleCharacterToken(.at, start: start)

        case "#":
            return makeSingleCharacterToken(.hash, start: start)

        case "\\":
            return makeSingleCharacterToken(.backslash, start: start)

        default:
            return lexUnknown(start: start)
        }
    }

    // Reads identifiers and reserved keywords.
    private func lexIdentifierOrKeyword(start: SourcePosition) -> Token {
        while let character = peek(), isIdentifierContinue(character) {
            advance()
        }

        let text = text(from: start.offset, to: offset)

        if Self.keywords.contains(text) {
            return Token(
                kind: .keyword(text),
                lexeme: text,
                range: makeRange(from: start)
            )
        }

        return Token(
            kind: .identifier,
            lexeme: text,
            range: makeRange(from: start)
        )
    }

    // Reads integer and floating-point literals.
    private func lexNumber(start: SourcePosition) -> Token {
        while let character = peek(), isDigit(character) {
            advance()
        }

        var kind: TokenKind = .integerLiteral

        if peek() == ".", isDigit(peekNext()) {
            kind = .floatingLiteral
            advance()

            while let character = peek(), isDigit(character) {
                advance()
            }
        }

        if let exponent = peek(), exponent == "e" || exponent == "E" {
            kind = .floatingLiteral
            advance()

            if let sign = peek(), sign == "+" || sign == "-" {
                advance()
            }

            while let character = peek(), isDigit(character) {
                advance()
            }
        }

        let text = text(from: start.offset, to: offset)

        return Token(
            kind: kind,
            lexeme: text,
            range: makeRange(from: start)
        )
    }

    // Reads a string literal.
    private func lexString(start: SourcePosition) -> Token {
        advance()

        var terminated = false

        while let character = peek() {
            if character == "\"" {
                advance()
                terminated = true
                break
            }

            if character == "\\" {
                advance()
                lexEscapeSequence(start: start)
                continue
            }

            if character == "\n" {
                addError(
                    "unterminated string literal",
                    start: start
                )
                break
            }

            advance()
        }

        if !terminated && isAtEnd {
            addError(
                "unterminated string literal",
                start: start
            )
        }

        return Token(
            kind: .stringLiteral,
            lexeme: text(from: start.offset, to: offset),
            range: makeRange(from: start)
        )
    }

    // Reads a character literal.
    private func lexCharacter(start: SourcePosition) -> Token {
        advance()

        var characterCount = 0
        var terminated = false

        while let character = peek() {
            if character == "'" {
                advance()
                terminated = true
                break
            }

            if character == "\\" {
                advance()
                lexEscapeSequence(start: start)
                characterCount += 1
                continue
            }

            if character == "\n" {
                addError(
                    "unterminated character literal",
                    start: start
                )
                break
            }

            advance()
            characterCount += 1
        }

        if characterCount == 0 {
            addError(
                "empty character literal",
                start: start
            )
        }

        if characterCount > 1 {
            addError(
                "character literal contains more than one character",
                start: start
            )
        }

        if !terminated && isAtEnd {
            addError(
                "unterminated character literal",
                start: start
            )
        }

        return Token(
            kind: .characterLiteral,
            lexeme: text(from: start.offset, to: offset),
            range: makeRange(from: start)
        )
    }

    // Reads an escaped character inside a literal.
    private func lexEscapeSequence(start: SourcePosition) {
        guard let character = peek() else {
            addError(
                "unterminated escape sequence",
                start: start
            )
            return
        }

        switch character {
        case "n", "r", "t", "0", "\\", "\"", "'", "b", "f", "v":
            advance()

        case "u":
            advance()
            lexUnicodeEscape(start: start)

        default:
            addError(
                "unknown escape sequence",
                start: start
            )
            advance()
        }
    }

    // Reads a Unicode escape sequence.
    private func lexUnicodeEscape(start: SourcePosition) {
        guard match("{") else {
            addError(
                "expected '{' after unicode escape",
                start: start
            )
            return
        }

        var digits = 0

        while let character = peek(), isHexDigit(character) {
            advance()
            digits += 1
        }

        if digits == 0 {
            addError(
                "unicode escape requires hexadecimal digits",
                start: start
            )
        }

        guard match("}") else {
            addError(
                "expected '}' after unicode escape",
                start: start
            )
            return
        }
    }

    // Reads the plus operator.
    private func lexPlus(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.plusEqual, start: start)
        }

        return makeToken(.plus, start: start)
    }

    // Reads the minus operator.
    private func lexMinus(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.minusEqual, start: start)
        }

        if match(">") {
            return makeToken(.arrow, start: start)
        }

        return makeToken(.minus, start: start)
    }

    // Reads the multiplication operator.
    private func lexStar(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.starEqual, start: start)
        }

        return makeToken(.star, start: start)
    }

    // Reads the division operator.
    private func lexSlash(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.slashEqual, start: start)
        }

        return makeToken(.slash, start: start)
    }

    // Reads the remainder operator.
    private func lexPercent(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.percentEqual, start: start)
        }

        return makeToken(.percent, start: start)
    }

    // Reads assignment and equality operators.
    private func lexEqual(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.equalEqual, start: start)
        }

        return makeToken(.equal, start: start)
    }

    // Reads negation and inequality operators.
    private func lexExclamation(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.notEqual, start: start)
        }

        return makeToken(.exclamation, start: start)
    }

    // Reads less-than operators.
    private func lexLess(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.lessEqual, start: start)
        }

        return makeToken(.less, start: start)
    }

    // Reads greater-than operators.
    private func lexGreater(start: SourcePosition) -> Token {
        advance()

        if match("=") {
            return makeToken(.greaterEqual, start: start)
        }

        return makeToken(.greater, start: start)
    }

    // Reads logical and operators.
    private func lexAmpersand(start: SourcePosition) -> Token {
        advance()

        if match("&") {
            return makeToken(.and, start: start)
        }

        return makeUnknownToken(start: start)
    }

    // Reads logical or operators.
    private func lexPipe(start: SourcePosition) -> Token {
        advance()

        if match("|") {
            return makeToken(.or, start: start)
        }

        return makeUnknownToken(start: start)
    }

    // Reads dots and range operators.
    private func lexDot(start: SourcePosition) -> Token {
        advance()

        if match(".") {
            if match("=") {
                return makeToken(.rangeEqual, start: start)
            }

            return makeToken(.range, start: start)
        }

        return makeToken(.dot, start: start)
    }

    // Creates a one-character token.
    private func makeSingleCharacterToken(
        _ kind: TokenKind,
        start: SourcePosition
    ) -> Token {
        advance()
        return makeToken(kind, start: start)
    }

    // Creates a token from the current source range.
    private func makeToken(
        _ kind: TokenKind,
        start: SourcePosition
    ) -> Token {
        Token(
            kind: kind,
            lexeme: text(from: start.offset, to: offset),
            range: makeRange(from: start)
        )
    }

    // Creates an unknown token.
    private func makeUnknownToken(start: SourcePosition) -> Token {
        makeToken(.unknown, start: start)
    }

    // Reports an unknown source character.
    private func lexUnknown(start: SourcePosition) -> Token {
        let character = advance() ?? "\0"

        addError(
            "unexpected character '\(character)'",
            start: start
        )

        return Token(
            kind: .unknown,
            lexeme: String(character),
            range: makeRange(from: start)
        )
    }

    // Skips whitespace and comments.
    private func skipWhitespaceAndComments() {
        var skippedSomething = true

        while skippedSomething && !isAtEnd {
            skippedSomething = false

            while let character = peek(), isWhitespace(character) {
                advance()
                skippedSomething = true
            }

            if peek() == "/", peekNext() == "/" {
                skipLineComment()
                skippedSomething = true
                continue
            }

            if peek() == "/", peekNext() == "*" {
                skipBlockComment()
                skippedSomething = true
                continue
            }
        }
    }

    // Skips a line comment.
    private func skipLineComment() {
        advance()
        advance()

        while let character = peek() {
            advance()

            if character == "\n" {
                break
            }
        }
    }

    // Skips a block comment.
    private func skipBlockComment() {
        let start = currentPosition

        advance()
        advance()

        var depth = 1

        while !isAtEnd && depth > 0 {
            if peek() == "/", peekNext() == "*" {
                if configuration.allowNestedBlockComments {
                    depth += 1
                }

                advance()
                advance()
                continue
            }

            if peek() == "*", peekNext() == "/" {
                depth -= 1
                advance()
                advance()
                continue
            }

            advance()
        }

        if depth != 0 {
            addError(
                "unterminated block comment",
                start: start
            )
        }
    }

    // Checks identifier-start characters.
    private func isIdentifierStart(_ character: Character) -> Bool {
        if character == "_" {
            return true
        }

        if character.isASCII {
            return character.isLetter
        }

        return configuration.allowUnicodeIdentifiers &&
            character.unicodeScalars.allSatisfy {
                $0.properties.isAlphabetic
            }
    }

    // Checks identifier continuation characters.
    private func isIdentifierContinue(_ character: Character) -> Bool {
        if isIdentifierStart(character) {
            return true
        }

        if character.isASCII {
            return character.isNumber
        }

        return character.unicodeScalars.allSatisfy {
            $0.properties.isAlphabetic || $0.properties.numericType != nil
        }
    }

    // Checks decimal digits.
    private func isDigit(_ character: Character?) -> Bool {
        guard let character else {
            return false
        }

        return character.isASCII && character.isNumber
    }

    // Checks hexadecimal digits.
    private func isHexDigit(_ character: Character) -> Bool {
        guard character.isASCII else {
            return false
        }

        switch character {
        case "0"..."9", "a"..."f", "A"..."F":
            return true
        default:
            return false
        }
    }

    // Checks source whitespace.
    private func isWhitespace(_ character: Character) -> Bool {
        character == " " ||
        character == "\t" ||
        character == "\n" ||
        character == "\r" ||
        character == "\u{000B}" ||
        character == "\u{000C}"
    }

    // Creates a range ending at the current position.
    private func makeRange(from start: SourcePosition) -> SourceRange {
        SourceRange(
            start: start,
            end: currentPosition
        )
    }

    // Extracts source text by UTF-8 offset.
    private func text(from start: Int, to end: Int) -> String {
        let lower = max(0, start)
        let upper = max(lower, end)

        var currentOffset = 0
        var result = ""

        for character in characters {
            let width = String(character).utf8.count

            if currentOffset >= lower && currentOffset < upper {
                result.append(character)
            }

            currentOffset += width

            if currentOffset >= upper {
                break
            }
        }

        return result
    }

    // Adds a lexer error.
    private func addError(
        _ message: String,
        start: SourcePosition
    ) {
        diagnostics.append(
            LexerDiagnostic(
                severity: .error,
                message: message,
                range: makeRange(from: start)
            )
        )
    }
}

// Keyword lookup utilities.
public enum KeywordTable {
    public static func contains(_ name: String) -> Bool {
        Lexer.keywordNames.contains(name)
    }

    public static func tokenKind(for name: String) -> TokenKind? {
        guard contains(name) else {
            return nil
        }

        return .keyword(name)
    }

    public static func allKeywords() -> [String] {
        Lexer.keywordNames
    }
}

// Token display utilities.
public extension TokenKind {
    var name: String {
        switch self {
        case .identifier:
            return "identifier"

        case .integerLiteral:
            return "integer literal"

        case .floatingLiteral:
            return "floating literal"

        case .stringLiteral:
            return "string literal"

        case .characterLiteral:
            return "character literal"

        case .keyword(let value):
            return "keyword(\(value))"

        case .plus:
            return "plus"

        case .minus:
            return "minus"

        case .star:
            return "star"

        case .slash:
            return "slash"

        case .percent:
            return "percent"

        case .equal:
            return "equal"

        case .equalEqual:
            return "equal equal"

        case .notEqual:
            return "not equal"

        case .less:
            return "less"

        case .lessEqual:
            return "less equal"

        case .greater:
            return "greater"

        case .greaterEqual:
            return "greater equal"

        case .and:
            return "and"

        case .or:
            return "or"

        case .not:
            return "not"

        case .plusEqual:
            return "plus equal"

        case .minusEqual:
            return "minus equal"

        case .starEqual:
            return "star equal"

        case .slashEqual:
            return "slash equal"

        case .percentEqual:
            return "percent equal"

        case .arrow:
            return "arrow"

        case .range:
            return "range"

        case .rangeEqual:
            return "range equal"

        case .leftParen:
            return "left parenthesis"

        case .rightParen:
            return "right parenthesis"

        case .leftBrace:
            return "left brace"

        case .rightBrace:
            return "right brace"

        case .leftBracket:
            return "left bracket"

        case .rightBracket:
            return "right bracket"

        case .comma:
            return "comma"

        case .colon:
            return "colon"

        case .semicolon:
            return "semicolon"

        case .dot:
            return "dot"

        case .question:
            return "question"

        case .exclamation:
            return "exclamation"

        case .at:
            return "at"

        case .hash:
            return "hash"

        case .backslash:
            return "backslash"

        case .endOfFile:
            return "end of file"

        case .unknown:
            return "unknown"
        }
    }
}

// Lexer source reader.
public struct LexerSource {
    public let text: String

    public init(_ text: String) {
        self.text = text
    }

    public func makeLexer(
        configuration: LexerConfiguration = LexerConfiguration()
    ) -> Lexer {
        Lexer(
            source: text,
            configuration: configuration
        )
    }
}

// Lexer test input helper.
public enum LexerTestSource {
    public static let basicDeclarations = """
    func add(value: int) -> int {
        return value
    }

    let answer = 42
    var count = 10
    """

    public static let controlFlow = """
    if value == 10 {
        return true
    } else {
        return false
    }
    """

    public static let allKeywords = Lexer.keywordNames.joined(separator: " ")

    public static let comments = """
    // line comment
    /*
       block comment
    */
    let value = 10
    """
}

// Lexer result container.
public struct LexerResult {
    public let tokens: [Token]
    public let diagnostics: [LexerDiagnostic]

    public init(
        tokens: [Token],
        diagnostics: [LexerDiagnostic]
    ) {
        self.tokens = tokens
        self.diagnostics = diagnostics
    }

    public var succeeded: Bool {
        !diagnostics.contains {
            $0.severity == .error
        }
    }
}

// Lexer convenience functions.
public enum Lex {
    public static func source(
        _ source: String,
        configuration: LexerConfiguration = LexerConfiguration()
    ) -> LexerResult {
        let lexer = Lexer(
            source: source,
            configuration: configuration
        )

        let (tokens, diagnostics) = lexer.tokenizeWithDiagnostics()

        return LexerResult(
            tokens: tokens,
            diagnostics: diagnostics
        )
    }

    public static func keywords() -> [String] {
        Lexer.keywordNames
    }
}

// Keyword constants.
public enum Keywords {
    public static let funcKeyword = "func"
    public static let varKeyword = "var"
    public static let letKeyword = "let"
    public static let typeKeyword = "type"
    public static let classKeyword = "class"
    public static let structKeyword = "struct"
    public static let enumKeyword = "enum"
    public static let templateKeyword = "template"
    public static let protoKeyword = "proto"
    public static let extendKeyword = "extend"

    public static let publicKeyword = "public"
    public static let privateKeyword = "private"
    public static let protectedKeyword = "protected"
    public static let staticKeyword = "static"
    public static let finalKeyword = "final"
    public static let constKeyword = "const"

    public static let ifKeyword = "if"
    public static let elseKeyword = "else"
    public static let guardKeyword = "guard"
    public static let matchKeyword = "match"
    public static let caseKeyword = "case"
    public static let defaultKeyword = "default"
    public static let whileKeyword = "while"
    public static let forKeyword = "for"
    public static let breakKeyword = "break"
    public static let continueKeyword = "continue"

    public static let returnKeyword = "return"
    public static let throwKeyword = "throw"
    public static let catchKeyword = "catch"
    public static let deferKeyword = "defer"
    public static let tryKeyword = "try"

    public static let newKeyword = "new"
    public static let deleteKeyword = "delete"
    public static let thisKeyword = "this"
    public static let moveKeyword = "move"
    public static let copyKeyword = "copy"
    public static let borrowKeyword = "borrow"

    public static let asyncKeyword = "async"
    public static let awaitKeyword = "await"
    public static let taskKeyword = "task"
    public static let actorKeyword = "actor"

    public static let importKeyword = "import"
    public static let exportKeyword = "export"
    public static let moduleKeyword = "module"

    public static let trueKeyword = "true"
    public static let falseKeyword = "false"
    public static let nilKeyword = "nil"

    public static let intKeyword = "int"
    public static let uintKeyword = "uint"
    public static let floatKeyword = "float"
    public static let doubleKeyword = "double"
    public static let boolKeyword = "bool"
    public static let stringKeyword = "string"
    public static let charKeyword = "char"

    public static let operatorKeyword = "operator"
    public static let autoKeyword = "auto"
    public static let initKeyword = "init"
    public static let deinitKeyword = "deinit"
    public static let inKeyword = "in"
    public static let selfKeyword = "self"
}

// Keyword validation utilities.
public enum KeywordValidator {
    public static func validate() -> Bool {
        let keywords = Lexer.keywordNames

        guard keywords.count == 60 else {
            return false
        }

        guard Set(keywords).count == keywords.count else {
            return false
        }

        return true
    }
}

// Token collection helpers.
public extension Array where Element == Token {
    var containsErrors: Bool {
        contains {
            $0.kind == .unknown
        }
    }

    var identifiers: [Token] {
        filter {
            if case .identifier = $0.kind {
                return true
            }

            return false
        }
    }

    var keywords: [Token] {
        filter {
            if case .keyword = $0.kind {
                return true
            }

            return false
        }
    }

    func kinds() -> [TokenKind] {
        map(\.kind)
    }
}

// Diagnostic formatting.
public enum LexerDiagnosticFormatter {
    public static func format(_ diagnostic: LexerDiagnostic) -> String {
        let severity: String

        switch diagnostic.severity {
        case .error:
            severity = "error"

        case .warning:
            severity = "warning"
        }

        let position = diagnostic.range.start

        return "\(severity): \(diagnostic.message) at \(position.line):\(position.column)"
    }

    public static func formatAll(
        _ diagnostics: [LexerDiagnostic]
    ) -> String {
        diagnostics
            .map(format)
            .joined(separator: "\n")
    }
}

// Lexer token formatting.
public enum TokenFormatter {
    public static func format(_ token: Token) -> String {
        let start = token.range.start
        let end = token.range.end

        return "\(token.kind.name) '\(token.lexeme)' " +
            "\(start.line):\(start.column)-\(end.line):\(end.column)"
    }

    public static func formatAll(_ tokens: [Token]) -> String {
        tokens
            .map(format)
            .joined(separator: "\n")
    }
}

// Lexer smoke tests.
public enum LexerSmokeTests {
    public static func run() -> Bool {
        testKeywordCount() &&
        testKeywordRecognition() &&
        testIdentifiers() &&
        testNumbers() &&
        testStrings() &&
        testOperators() &&
        testComments()
    }

    private static func testKeywordCount() -> Bool {
        Lexer.keywordNames.count == 60
    }

    private static func testKeywordRecognition() -> Bool {
        let result = Lex.source(LexerTestSource.allKeywords)

        guard result.succeeded else {
            return false
        }

        return result.tokens.filter {
            if case .keyword = $0.kind {
                return true
            }

            return false
        }.count == 60
    }

    private static func testIdentifiers() -> Bool {
        let result = Lex.source("alpha beta_value _hidden value2")

        guard result.succeeded else {
            return false
        }

        return result.tokens.identifiers.count == 4
    }

    private static func testNumbers() -> Bool {
        let result = Lex.source("10 20.5 3e4 5.2e-2")

        guard result.succeeded else {
            return false
        }

        return result.tokens.filter {
            $0.kind == .integerLiteral ||
            $0.kind == .floatingLiteral
        }.count == 4
    }

    private static func testStrings() -> Bool {
        let result = Lex.source(#""hello" 'x' "line\nvalue""#)

        return result.succeeded
    }

    private static func testOperators() -> Bool {
        let source = "+ += - -= * *= / /= == = != < <= > >= && || ->"
        let result = Lex.source(source)

        return result.succeeded
    }

    private static func testComments() -> Bool {
        let result = Lex.source(LexerTestSource.comments)

        return result.succeeded &&
            result.tokens.identifiers.count == 1
    }
}

// Lexer command-line demonstration.
public enum LexerDemo {
    public static func run(source: String) {
        let result = Lex.source(source)

        print(TokenFormatter.formatAll(result.tokens))

        if !result.diagnostics.isEmpty {
            print(LexerDiagnosticFormatter.formatAll(result.diagnostics))
        }
    }
}

// Public lexer API.
public enum ShiftLexer {
    public static func lex(
        _ source: String
    ) -> LexerResult {
        Lex.source(source)
    }

    public static func keywordList() -> [String] {
        Lexer.keywordNames
    }
}

// Lexer version information.
public enum LexerVersion {
    public static let major = 0
    public static let minor = 1
    public static let patch = 0

    public static var string: String {
        "\(major).\(minor).\(patch)"
    }
}

// Lexer feature information.
public enum LexerFeatures {
    public static let supportsKeywords = true
    public static let supportsIdentifiers = true
    public static let supportsIntegerLiterals = true
    public static let supportsFloatingLiterals = true
    public static let supportsStringLiterals = true
    public static let supportsCharacterLiterals = true
    public static let supportsLineComments = true
    public static let supportsBlockComments = true
    public static let supportsUnicodeIdentifiers = true
    public static let supportsNestedBlockComments = true
}

// Lexer configuration presets.
public enum LexerConfigurations {
    public static let standard = LexerConfiguration()

    public static let strict = LexerConfiguration(
        allowUnicodeIdentifiers: false,
        allowSingleQuotedCharacters: true,
        allowNestedBlockComments: false
    )

    public static let unicode = LexerConfiguration(
        allowUnicodeIdentifiers: true,
        allowSingleQuotedCharacters: true,
        allowNestedBlockComments: true
    )
}

// Keyword groups.
public enum KeywordGroups {
    public static let declarations: [String] = [
        "func", "var", "let", "type", "class",
        "struct", "enum", "template", "proto", "extend"
    ]

    public static let access: [String] = [
        "public", "private", "protected",
        "static", "final", "const"
    ]

    public static let controlFlow: [String] = [
        "if", "else", "guard", "match", "case",
        "default", "while", "for", "break", "continue"
    ]

    public static let errors: [String] = [
        "return", "throw", "catch", "defer", "try"
    ]

    public static let ownership: [String] = [
        "new", "delete", "this", "move", "copy", "borrow"
    ]

    public static let concurrency: [String] = [
        "async", "await", "task", "actor"
    ]

    public static let modules: [String] = [
        "import", "export", "module"
    ]

    public static let values: [String] = [
        "true", "false", "nil"
    ]

    public static let types: [String] = [
        "int", "uint", "float", "double",
        "bool", "string", "char"
    ]

    public static let language: [String] = [
        "operator", "auto", "init", "deinit", "in", "self"
    ]
}

// Keyword group validation.
public enum KeywordGroupValidator {
    public static func validate() -> Bool {
        let groups = [
            KeywordGroups.declarations,
            KeywordGroups.access,
            KeywordGroups.controlFlow,
            KeywordGroups.errors,
            KeywordGroups.ownership,
            KeywordGroups.concurrency,
            KeywordGroups.modules,
            KeywordGroups.values,
            KeywordGroups.types,
            KeywordGroups.language
        ]

        let flattened = groups.flatMap { $0 }

        return flattened.count == 60 &&
            Set(flattened).count == 60 &&
            Set(flattened) == Set(Lexer.keywordNames)
    }
}

// Lexer statistics.
public struct LexerStatistics {
    public let tokenCount: Int
    public let keywordCount: Int
    public let identifierCount: Int
    public let integerCount: Int
    public let floatingCount: Int
    public let stringCount: Int
    public let characterCount: Int
    public let unknownCount: Int

    public init(tokens: [Token]) {
        tokenCount = tokens.count
        keywordCount = tokens.keywords.count
        identifierCount = tokens.identifiers.count

        integerCount = tokens.filter {
            $0.kind == .integerLiteral
        }.count

        floatingCount = tokens.filter {
            $0.kind == .floatingLiteral
        }.count

        stringCount = tokens.filter {
            $0.kind == .stringLiteral
        }.count

        characterCount = tokens.filter {
            $0.kind == .characterLiteral
        }.count

        unknownCount = tokens.filter {
            $0.kind == .unknown
        }.count
    }
}

// Lexer source metadata.
public struct LexerSourceMetadata {
    public let byteCount: Int
    public let characterCount: Int
    public let lineCount: Int

    public init(source: String) {
        byteCount = source.utf8.count
        characterCount = source.count
        lineCount = max(1, source.split(separator: "\n", omittingEmptySubsequences: false).count)
    }
}

// Lexer service.
public final class LexerService {
    private let configuration: LexerConfiguration

    public init(
        configuration: LexerConfiguration = LexerConfiguration()
    ) {
        self.configuration = configuration
    }

    public func run(_ source: String) -> LexerResult {
        Lex.source(
            source,
            configuration: configuration
        )
    }

    public func statistics(
        for source: String
    ) -> LexerStatistics {
        let result = run(source)
        return LexerStatistics(tokens: result.tokens)
    }

    public func metadata(
        for source: String
    ) -> LexerSourceMetadata {
        LexerSourceMetadata(source: source)
    }
}

// Lexer keyword printer.
public enum KeywordPrinter {
    public static func printAll() {
        for (index, keyword) in Lexer.keywordNames.enumerated() {
            print("\(index + 1). \(keyword)")
        }
    }
}

// Lexer source validation.
public enum LexerSourceValidator {
    public static func validate(_ source: String) -> Bool {
        let result = Lex.source(source)
        return result.succeeded
    }
}

// Lexer token search.
public enum TokenSearch {
    public static func findKeyword(
        _ keyword: String,
        in tokens: [Token]
    ) -> [Token] {
        tokens.filter {
            $0.kind == .keyword(keyword)
        }
    }

    public static func findIdentifier(
        _ identifier: String,
        in tokens: [Token]
    ) -> [Token] {
        tokens.filter {
            $0.kind == .identifier &&
            $0.lexeme == identifier
        }
    }
}

// Lexer keyword classification.
public enum KeywordClassification {
    public static func isDeclaration(_ keyword: String) -> Bool {
        KeywordGroups.declarations.contains(keyword)
    }

    public static func isAccess(_ keyword: String) -> Bool {
        KeywordGroups.access.contains(keyword)
    }

    public static func isControlFlow(_ keyword: String) -> Bool {
        KeywordGroups.controlFlow.contains(keyword)
    }

    public static func isErrorHandling(_ keyword: String) -> Bool {
        KeywordGroups.errors.contains(keyword)
    }

    public static func isOwnership(_ keyword: String) -> Bool {
        KeywordGroups.ownership.contains(keyword)
    }

    public static func isConcurrency(_ keyword: String) -> Bool {
        KeywordGroups.concurrency.contains(keyword)
    }

    public static func isModule(_ keyword: String) -> Bool {
        KeywordGroups.modules.contains(keyword)
    }

    public static func isValue(_ keyword: String) -> Bool {
        KeywordGroups.values.contains(keyword)
    }

    public static func isType(_ keyword: String) -> Bool {
        KeywordGroups.types.contains(keyword)
    }

    public static func isLanguageKeyword(_ keyword: String) -> Bool {
        KeywordGroups.language.contains(keyword)
    }
}

// Lexer source printer.
public enum LexerSourcePrinter {
    public static func printSourceInfo(_ source: String) {
        let metadata = LexerSourceMetadata(source: source)

        print("bytes: \(metadata.byteCount)")
        print("characters: \(metadata.characterCount)")
        print("lines: \(metadata.lineCount)")
    }
}

// Lexer regression suite.
public enum LexerRegressionSuite {
    public static func run() -> [String: Bool] {
        [
            "keyword-count": LexerSmokeTests.run() && KeywordValidator.validate(),
            "keyword-groups": KeywordGroupValidator.validate(),
            "basic-declarations": testBasicDeclarations(),
            "unicode-identifiers": testUnicodeIdentifiers(),
            "nested-comments": testNestedComments(),
            "diagnostics": testDiagnostics()
        ]
    }

    private static func testBasicDeclarations() -> Bool {
        let source = """
        func test() {
            let value = 10
            var result = value
            return result
        }
        """

        let result = Lex.source(source)

        return result.succeeded &&
            result.tokens.keywords.count >= 4
    }

    private static func testUnicodeIdentifiers() -> Bool {
        let result = Lex.source("let café = 10")

        return result.succeeded &&
            result.tokens.identifiers.contains {
                $0.lexeme == "café"
            }
    }

    private static func testNestedComments() -> Bool {
        let source = """
        /*
            outer
            /*
                inner
            */
        */
        let value = 10
        """

        let result = Lex.source(source)

        return result.succeeded
    }

    private static func testDiagnostics() -> Bool {
        let result = Lex.source("\"unterminated")

        return !result.diagnostics.isEmpty
    }
}

// Lexer public entry point.
public enum LanguageLexer {
    public static func tokenize(_ source: String) -> [Token] {
        Lexer(source: source).tokenize()
    }

    public static func tokenize(
        _ source: String,
        configuration: LexerConfiguration
    ) -> LexerResult {
        Lex.source(
            source,
            configuration: configuration
        )
    }

    public static func keywords() -> [String] {
        Lexer.keywordNames
    }

    public static func validateKeywordTable() -> Bool {
        KeywordValidator.validate() &&
        KeywordGroupValidator.validate()
    }
}

// End of lexer implementation.

// Keyword accessors.
public extension Keywords {
    static var funcKeyword: TokenKind { .keyword("func") }
    static var varKeyword: TokenKind { .keyword("var") }
    static var letKeyword: TokenKind { .keyword("let") }
    static var typeKeyword: TokenKind { .keyword("type") }
    static var classKeyword: TokenKind { .keyword("class") }
    static var structKeyword: TokenKind { .keyword("struct") }
    static var enumKeyword: TokenKind { .keyword("enum") }
    static var templateKeyword: TokenKind { .keyword("template") }
    static var protoKeyword: TokenKind { .keyword("proto") }
    static var extendKeyword: TokenKind { .keyword("extend") }
    static var publicKeyword: TokenKind { .keyword("public") }
    static var privateKeyword: TokenKind { .keyword("private") }
    static var protectedKeyword: TokenKind { .keyword("protected") }
    static var staticKeyword: TokenKind { .keyword("static") }
    static var finalKeyword: TokenKind { .keyword("final") }
    static var constKeyword: TokenKind { .keyword("const") }
    static var ifKeyword: TokenKind { .keyword("if") }
    static var elseKeyword: TokenKind { .keyword("else") }
    static var guardKeyword: TokenKind { .keyword("guard") }
    static var matchKeyword: TokenKind { .keyword("match") }
    static var caseKeyword: TokenKind { .keyword("case") }
    static var defaultKeyword: TokenKind { .keyword("default") }
    static var whileKeyword: TokenKind { .keyword("while") }
    static var forKeyword: TokenKind { .keyword("for") }
    static var breakKeyword: TokenKind { .keyword("break") }
    static var continueKeyword: TokenKind { .keyword("continue") }
    static var returnKeyword: TokenKind { .keyword("return") }
    static var throwKeyword: TokenKind { .keyword("throw") }
    static var catchKeyword: TokenKind { .keyword("catch") }
    static var deferKeyword: TokenKind { .keyword("defer") }
    static var tryKeyword: TokenKind { .keyword("try") }
    static var newKeyword: TokenKind { .keyword("new") }
    static var deleteKeyword: TokenKind { .keyword("delete") }
    static var thisKeyword: TokenKind { .keyword("this") }
    static var moveKeyword: TokenKind { .keyword("move") }
    static var copyKeyword: TokenKind { .keyword("copy") }
    static var borrowKeyword: TokenKind { .keyword("borrow") }
    static var asyncKeyword: TokenKind { .keyword("async") }
    static var awaitKeyword: TokenKind { .keyword("await") }
    static var taskKeyword: TokenKind { .keyword("task") }
    static var actorKeyword: TokenKind { .keyword("actor") }
    static var importKeyword: TokenKind { .keyword("import") }
    static var exportKeyword: TokenKind { .keyword("export") }
    static var moduleKeyword: TokenKind { .keyword("module") }
    static var trueKeyword: TokenKind { .keyword("true") }
    static var falseKeyword: TokenKind { .keyword("false") }
    static var nilKeyword: TokenKind { .keyword("nil") }
    static var intKeyword: TokenKind { .keyword("int") }
    static var uintKeyword: TokenKind { .keyword("uint") }
    static var floatKeyword: TokenKind { .keyword("float") }
    static var doubleKeyword: TokenKind { .keyword("double") }
    static var boolKeyword: TokenKind { .keyword("bool") }
    static var stringKeyword: TokenKind { .keyword("string") }
    static var charKeyword: TokenKind { .keyword("char") }
    static var operatorKeyword: TokenKind { .keyword("operator") }
    static var autoKeyword: TokenKind { .keyword("auto") }
    static var initKeyword: TokenKind { .keyword("init") }
    static var deinitKeyword: TokenKind { .keyword("deinit") }
    static var inKeyword: TokenKind { .keyword("in") }
    static var selfKeyword: TokenKind { .keyword("self") }
}

// Token factory helpers.
public enum TokenFactory {
    public static func plus() -> TokenKind { .plus }
    public static func minus() -> TokenKind { .minus }
    public static func star() -> TokenKind { .star }
    public static func slash() -> TokenKind { .slash }
    public static func percent() -> TokenKind { .percent }
    public static func equal() -> TokenKind { .equal }
    public static func equalEqual() -> TokenKind { .equalEqual }
    public static func notEqual() -> TokenKind { .notEqual }
    public static func less() -> TokenKind { .less }
    public static func lessEqual() -> TokenKind { .lessEqual }
    public static func greater() -> TokenKind { .greater }
    public static func greaterEqual() -> TokenKind { .greaterEqual }
    public static func and() -> TokenKind { .and }
    public static func or() -> TokenKind { .or }
    public static func not() -> TokenKind { .not }
    public static func plusEqual() -> TokenKind { .plusEqual }
    public static func minusEqual() -> TokenKind { .minusEqual }
    public static func starEqual() -> TokenKind { .starEqual }
    public static func slashEqual() -> TokenKind { .slashEqual }
    public static func percentEqual() -> TokenKind { .percentEqual }
    public static func arrow() -> TokenKind { .arrow }
    public static func range() -> TokenKind { .range }
    public static func rangeEqual() -> TokenKind { .rangeEqual }
    public static func leftParen() -> TokenKind { .leftParen }
    public static func rightParen() -> TokenKind { .rightParen }
    public static func leftBrace() -> TokenKind { .leftBrace }
    public static func rightBrace() -> TokenKind { .rightBrace }
    public static func leftBracket() -> TokenKind { .leftBracket }
    public static func rightBracket() -> TokenKind { .rightBracket }
    public static func comma() -> TokenKind { .comma }
    public static func colon() -> TokenKind { .colon }
    public static func semicolon() -> TokenKind { .semicolon }
    public static func dot() -> TokenKind { .dot }
    public static func question() -> TokenKind { .question }
    public static func exclamation() -> TokenKind { .exclamation }
    public static func at() -> TokenKind { .at }
    public static func hash() -> TokenKind { .hash }
    public static func backslash() -> TokenKind { .backslash }
    public static func endOfFile() -> TokenKind { .endOfFile }
    public static func unknown() -> TokenKind { .unknown }
}

// Lexer token statistics helpers.
public extension LexerStatistics {
    var hasUnknownTokens: Bool {
        unknownCount > 0
    }

    var literalCount: Int {
        integerCount +
        floatingCount +
        stringCount +
        characterCount
    }

    var declarationCount: Int {
        keywordCount
    }
}

// Lexer result helpers.
public extension LexerResult {
    var hasErrors: Bool {
        !diagnostics.isEmpty
    }

    var statistics: LexerStatistics {
        LexerStatistics(tokens: tokens)
    }
}

// Source range helpers.
public extension SourceRange {
    var startLine: Int {
        start.line
    }

    var startColumn: Int {
        start.column
    }

    var endLine: Int {
        end.line
    }

    var endColumn: Int {
        end.column
    }
}

// Source position helpers.
public extension SourcePosition {
    var isBeginningOfFile: Bool {
        offset == 0 && line == 1 && column == 1
    }
}

// Token helpers.
public extension Token {
    var isKeyword: Bool {
        if case .keyword = kind {
            return true
        }

        return false
    }

    var isIdentifier: Bool {
        kind == .identifier
    }

    var isLiteral: Bool {
        switch kind {
        case .integerLiteral,
             .floatingLiteral,
             .stringLiteral,
             .characterLiteral:
            return true
        default:
            return false
        }
    }

    var isOperator: Bool {
        switch kind {
        case .plus,
             .minus,
             .star,
             .slash,
             .percent,
             .equal,
             .equalEqual,
             .notEqual,
             .less,
             .lessEqual,
             .greater,
             .greaterEqual,
             .and,
             .or,
             .not,
             .plusEqual,
             .minusEqual,
             .starEqual,
             .slashEqual,
             .percentEqual,
             .arrow,
             .range,
             .rangeEqual:
            return true
        default:
            return false
        }
    }

    var isDelimiter: Bool {
        switch kind {
        case .leftParen,
             .rightParen,
             .leftBrace,
             .rightBrace,
             .leftBracket,
             .rightBracket,
             .comma,
             .colon,
             .semicolon,
             .dot:
            return true
        default:
            return false
        }
    }
}

// Lexer source factory.
public enum LexerFactory {
    public static func make(
        source: String
    ) -> Lexer {
        Lexer(source: source)
    }

    public static func make(
        source: String,
        configuration: LexerConfiguration
    ) -> Lexer {
        Lexer(
            source: source,
            configuration: configuration
        )
    }
}

// Lexer test runner.
public enum LexerTestRunner {
    public static func runAll() -> Bool {
        let results = LexerRegressionSuite.run()

        for (name, result) in results.sorted(by: { $0.key < $1.key }) {
            print("\(name): \(result ? "passed" : "failed")")
        }

        return results.values.allSatisfy { $0 }
    }
}

// Keyword table output.
public enum KeywordTablePrinter {
    public static func table() -> String {
        Lexer.keywordNames
            .enumerated()
            .map { index, keyword in
                "\(index + 1): \(keyword)"
            }
            .joined(separator: "\n")
    }
}

// Lexer module information.
public enum LexerModuleInfo {
    public static let name = "LanguageLexer"
    public static let version = LexerVersion.string
    public static let keywordCount = Lexer.keywordNames.count
    public static let implementationLanguage = "Swift"
    public static let targetLanguage = "Swift"
}

// Lexer parser boundary.
public struct LexerOutput {
    public let tokens: [Token]
    public let diagnostics: [LexerDiagnostic]

    public init(
        tokens: [Token],
        diagnostics: [LexerDiagnostic]
    ) {
        self.tokens = tokens
        self.diagnostics = diagnostics
    }
}

// Lexer pipeline boundary.
public protocol Lexing {
    func tokenize() -> [Token]
}

// Lexer service conformance.
extension Lexer: Lexing {}

// Lexer diagnostic collection.
public final class DiagnosticCollector {
    private var values: [LexerDiagnostic] = []

    public init() {}

    public func append(_ diagnostic: LexerDiagnostic) {
        values.append(diagnostic)
    }

    public func append(contentsOf diagnostics: [LexerDiagnostic]) {
        values.append(contentsOf: diagnostics)
    }

    public func all() -> [LexerDiagnostic] {
        values
    }

    public var isEmpty: Bool {
        values.isEmpty
    }

    public var count: Int {
        values.count
    }

    public func clear() {
        values.removeAll(keepingCapacity: true)
    }
}

// Lexer token buffer.
public final class TokenBuffer {
    private var values: [Token] = []

    public init() {}

    public func append(_ token: Token) {
        values.append(token)
    }

    public func append(contentsOf tokens: [Token]) {
        values.append(contentsOf: tokens)
    }

    public func all() -> [Token] {
        values
    }

    public var count: Int {
        values.count
    }

    public var isEmpty: Bool {
        values.isEmpty
    }

    public func clear() {
        values.removeAll(keepingCapacity: true)
    }
}

// Lexer keyword set.
public struct KeywordSet {
    private let values: Set<String>

    public init() {
        values = Set(Lexer.keywordNames)
    }

    public func contains(_ value: String) -> Bool {
        values.contains(value)
    }

    public var count: Int {
        values.count
    }

    public func all() -> [String] {
        values.sorted()
    }
}

// Lexer operator set.
public enum OperatorSet {
    public static let names: [String] = [
        "+",
        "-",
        "*",
        "/",
        "%",
        "=",
        "==",
        "!=",
        "<",
        "<=",
        ">",
        ">=",
        "&&",
        "||",
        "!",
        "+=",
        "-=",
        "*=",
        "/=",
        "%=",
        "->",
        "..",
        "..="
    ]

    public static func contains(_ value: String) -> Bool {
        names.contains(value)
    }
}

// Lexer delimiter set.
public enum DelimiterSet {
    public static let names: [String] = [
        "(",
        ")",
        "{",
        "}",
        "[",
        "]",
        ",",
        ":",
        ";",
        ".",
        "?",
        "@",
        "#",
        "\\"
    ]

    public static func contains(_ value: String) -> Bool {
        names.contains(value)
    }
}

// Lexer validation report.
public struct LexerValidationReport {
    public let keywordCount: Int
    public let uniqueKeywordCount: Int
    public let keywordTableValid: Bool
    public let groupTableValid: Bool

    public init() {
        keywordCount = Lexer.keywordNames.count
        uniqueKeywordCount = Set(Lexer.keywordNames).count
        keywordTableValid = KeywordValidator.validate()
        groupTableValid = KeywordGroupValidator.validate()
    }

    public var valid: Bool {
        keywordCount == 60 &&
        uniqueKeywordCount == 60 &&
        keywordTableValid &&
        groupTableValid
    }
}

// Lexer validation entry point.
public enum LexerValidator {
    public static func report() -> LexerValidationReport {
        LexerValidationReport()
    }

    public static func validate() -> Bool {
        report().valid
    }
}

// Token classification utilities.
public enum TokenClassifier {
    public static func isControlFlow(_ value: String) -> Bool {
        ["if", "else", "guard", "match", "case", "default", "while", "for", "break", "continue"].contains(value)
    }
    public static func isDeclaration(_ value: String) -> Bool {
        ["func", "var", "let", "type", "class", "struct", "enum", "template", "proto", "extend"].contains(value)
    }
    public static func isAccessModifier(_ value: String) -> Bool {
        ["public", "private", "protected"].contains(value)
    }
    public static func isOwnership(_ value: String) -> Bool {
        ["new", "delete", "this", "move", "copy", "borrow"].contains(value)
    }
    public static func isConcurrency(_ value: String) -> Bool {
        ["async", "await", "task", "actor"].contains(value)
    }
    public static func isModule(_ value: String) -> Bool {
        ["import", "export", "module"].contains(value)
    }
    public static func isPrimitiveType(_ value: String) -> Bool {
        ["int", "uint", "float", "double", "bool", "string", "char"].contains(value)
    }
    public static func isValueLiteral(_ value: String) -> Bool {
        ["true", "false", "nil"].contains(value)
    }
}

// Token name lookup.
public enum TokenNameLookup {
    public static let names: [TokenKind: String] = [:]
}