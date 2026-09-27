// MARK: - Source Location

public struct SourceLocation: Equatable, CustomStringConvertible {
    public let line: Int
    public let column: Int
    public let offset: Int

    public var description: String {
        return "line \(line), column \(column)"
    }
}

public struct SourceSpan: Equatable, CustomStringConvertible {
    public let start: SourceLocation
    public let end: SourceLocation

    public var description: String {
        return "\(start) - \(end)"
    }
}

// MARK: - Token Types

public enum TokenType: Equatable {
    // End of file
    case eof
    
    // Identifiers and Literals
    case identifier(String)
    case stringLiteral(String)
    case integerLiteral(Int64)
    case floatLiteral(Double)

    // Declarations & Scope
    case keywordClass
    case keywordStruct
    case keywordInterface
    case keywordEnum
    case keywordActor
    case keywordDef
    case keywordVal
    case keywordVar
    case keywordInit
    case keywordDeinit
    case keywordModule
    case keywordInclude

    // Control Flow & Logic
    case keywordIf
    case keywordElse
    case keywordWhen
    case keywordCase
    case keywordDefault
    case keywordFor
    case keywordIn
    case keywordWhile
    case keywordLoop
    case keywordBreak
    case keywordContinue
    case keywordReturn
    case keywordGuard
    case keywordDefer
    case keywordPass

    // Concurrency & Errors
    case keywordAsync
    case keywordAwait
    case keywordTry
    case keywordCatch
    case keywordRaise
    case keywordThrows
    case keywordSpawn

    // Expressions & Literals
    case keywordSelfVal
    case keywordSelfType
    case keywordSuper
    case keywordNone
    case keywordTrue
    case keywordFalse
    case keywordAs

    // Visibility & ARC Modifiers
    case keywordPublic
    case keywordPrivate
    case keywordMut
    case keywordOverride
    case keywordStatic
    case keywordWeak
    case keywordUnowned
    case keywordInOut

    // Punctuation & Symbols
    case leftParen       // (
    case rightParen      // )
    case leftBrace       // {
    case rightBrace      // }
    case leftBracket     // [
    case rightBracket    // ]
    case comma           // ,
    case dot             // .
    case colon           // :
    case semicolon       // ;
    case questionMark    // ?
    case bang            // !
    case arrow           // ->

    // Operators
    case equal           // =
    case equalEqual      // ==
    case bangEqual       // !=
    case less            // <
    case lessEqual       // <=
    case greater         // >
    case greaterEqual    // >=
    case plus            // +
    case plusEqual       // +=
    case minus           // -
    case minusEqual      // -=
    case star            // *
    case starEqual       // *=
    case slash           // /
    case slashEqual      // /=
    case percent         // %
    case percentEqual    // %=
    case ampersand       // &
    case ampersandAmpersand // &&
    case pipe            // |
    case pipePipe        // ||
}

// MARK: - Token

public struct Token: Equatable {
    public let type: TokenType
    public let lexeme: String
    public let span: SourceSpan
}

// MARK: - Lexer Errors

public enum LexerError: Error, CustomStringConvertible {
    case unexpectedCharacter(Character, SourceLocation)
    case unterminatedString(SourceLocation)
    case invalidNumberFormat(String, SourceLocation)

    public var description: String {
        switch self {
        case .unexpectedCharacter(let char, let loc):
            return "Unexpected character '\(char)' at \(loc)"
        case .unterminatedString(let loc):
            return "Unterminated string literal starting at \(loc)"
        case .invalidNumberFormat(let str, let loc):
            return "Invalid number format '\(str)' at \(loc)"
        }
    }
}

// MARK: - Lexer Core

public final class Lexer {
    private let source: [Character]
    private var start: Int = 0
    private var current: Int = 0
    private var line: Int = 1
    private var column: Int = 1
    
    // Keyword lookup table
    private let keywords: [String: TokenType] = [
        "class": .keywordClass,
        "struct": .keywordStruct,
        "interface": .keywordInterface,
        "enum": .keywordEnum,
        "actor": .keywordActor,
        "def": .keywordDef,
        "val": .keywordVal,
        "var": .keywordVar,
        "init": .keywordInit,
        "deinit": .keywordDeinit,
        "module": .keywordModule,
        "include": .keywordInclude,
        "if": .keywordIf,
        "else": .keywordElse,
        "when": .keywordWhen,
        "case": .keywordCase,
        "default": .keywordDefault,
        "for": .keywordFor,
        "in": .keywordIn,
        "while": .keywordWhile,
        "loop": .keywordLoop,
        "break": .keywordBreak,
        "continue": .keywordContinue,
        "return": .keywordReturn,
        "guard": .keywordGuard,
        "defer": .keywordDefer,
        "pass": .keywordPass,
        "async": .keywordAsync,
        "await": .keywordAwait,
        "try": .keywordTry,
        "catch": .keywordCatch,
        "raise": .keywordRaise,
        "throws": .keywordThrows,
        "spawn": .keywordSpawn,
        "self": .keywordSelfVal,
        "Self": .keywordSelfType,
        "super": .keywordSuper,
        "None": .keywordNone,
        "true": .keywordTrue,
        "false": .keywordFalse,
        "as": .keywordAs,
        "public": .keywordPublic,
        "private": .keywordPrivate,
        "mut": .keywordMut,
        "override": .keywordOverride,
        "static": .keywordStatic,
        "weak": .keywordWeak,
        "unowned": .keywordUnowned,
        "inout": .keywordInOut
    ]

    public init(source: String) {
        self.source = Array(source)
    }
    
    // MARK: - Public API
    
    public func tokenize() throws -> [Token] {
        var tokens: [Token] = []
        while !isAtEnd {
            start = current
            if let token = try scanToken() {
                tokens.append(token)
            }
        }
        
        let eofLoc = SourceLocation(line: line, column: column, offset: current)
        tokens.append(Token(
            type: .eof,
            lexeme: "",
            span: SourceSpan(start: eofLoc, end: eofLoc)
        ))
        return tokens
    }
    
    // MARK: - Scanning Logic
    
    private func scanToken() throws -> Token? {
        skipWhitespaceAndComments()
        start = current
        
        guard !isAtEnd else { return nil }
        
        let c = advance()
        
        // Identifiers and Keywords
        if isAlpha(c) {
            return lexIdentifierOrKeyword()
        }
        
        // Numbers
        if isDigit(c) {
            return try lexNumber()
        }
        
        // Strings
        if c == "\"" {
            return try lexString()
        }
        
        // Punctuation and Operators
        switch c {
        case "(": return makeToken(.leftParen)
        case ")": return makeToken(.rightParen)
        case "{": return makeToken(.leftBrace)
        case "}": return makeToken(.rightBrace)
        case "[": return makeToken(.leftBracket)
        case "]": return makeToken(.rightBracket)
        case ",": return makeToken(.comma)
        case ".": return makeToken(.dot)
        case ":": return makeToken(.colon)
        case ";": return makeToken(.semicolon)
        case "?": return makeToken(.questionMark)
        
        case "-":
            if match(">") { return makeToken(.arrow) }
            if match("=") { return makeToken(.minusEqual) }
            return makeToken(.minus)
            
        case "+":
            return makeToken(match("=") ? .plusEqual : .plus)
            
        case "*":
            return makeToken(match("=") ? .starEqual : .star)
            
        case "/":
            return makeToken(match("=") ? .slashEqual : .slash)
            
        case "%":
            return makeToken(match("=") ? .percentEqual : .percent)
            
        case "=":
            return makeToken(match("=") ? .equalEqual : .equal)
            
        case "!":
            return makeToken(match("=") ? .bangEqual : .bang)
            
        case "<":
            return makeToken(match("=") ? .lessEqual : .less)
            
        case ">":
            return makeToken(match("=") ? .greaterEqual : .greater)
            
        case "&":
            return makeToken(match("&") ? .ampersandAmpersand : .ampersand)
            
        case "|":
            return makeToken(match("|") ? .pipePipe : .pipe)
            
        default:
            throw LexerError.unexpectedCharacter(c, currentLocation())
        }
    }
    
    // MARK: - Lexing Sub-Routines
    
    private func lexIdentifierOrKeyword() -> Token {
        while isAlphaNumeric(peek()) {
            _ = advance()
        }
        
        let text = String(source[start..<current])
        if let keywordType = keywords[text] {
            return makeToken(keywordType)
        }
        return makeToken(.identifier(text))
    }
    
    private func lexNumber() throws -> Token {
        while isDigit(peek()) {
            _ = advance()
        }
        
        var isFloat = false
        if peek() == "." && isDigit(peekNext()) {
            isFloat = true
            _ = advance() // consume '.'
            while isDigit(peek()) {
                _ = advance()
            }
        }
        
        let text = String(source[start..<current])
        
        if isFloat {
            guard let value = Double(text) else {
                throw LexerError.invalidNumberFormat(text, startLocation())
            }
            return makeToken(.floatLiteral(value))
        } else {
            guard let value = Int64(text) else {
                throw LexerError.invalidNumberFormat(text, startLocation())
            }
            return makeToken(.integerLiteral(value))
        }
    }
    
    private func lexString() throws -> Token {
        let stringStartLoc = startLocation()
        
        while peek() != "\"" && !isAtEnd {
            if peek() == "\n" {
                line += 1
                column = 0
            }
            // Handle escaped characters
            if peek() == "\\" {
                _ = advance() // consume backslash
                _ = advance() // consume escaped character
            } else {
                _ = advance()
            }
        }
        
        if isAtEnd {
            throw LexerError.unterminatedString(stringStartLoc)
        }
        
        _ = advance() // consume closing quote
        
        // Strip out the surrounding quotes
        let value = String(source[(start + 1)..<(current - 1)])
        return makeToken(.stringLiteral(value))
    }
    
    private func skipWhitespaceAndComments() {
        while !isAtEnd {
            let c = peek()
            switch c {
            case " ", "\r", "\t":
                _ = advance()
            case "\n":
                line += 1
                column = 0
                _ = advance()
            case "/":
                if peekNext() == "/" {
                    // Single-line comment
                    while peek() != "\n" && !isAtEnd {
                        _ = advance()
                    }
                } else if peekNext() == "*" {
                    // Multi-line comment
                    _ = advance()
                    _ = advance()
                    while !isAtEnd {
                        if peek() == "*" && peekNext() == "/" {
                            _ = advance()
                            _ = advance()
                            break
                        }
                        if peek() == "\n" {
                            line += 1
                            column = 0
                        }
                        _ = advance()
                    }
                } else {
                    return // It's just a division operator
                }
            default:
                return
            }
        }
    }
    
    // MARK: - Character Helpers
    
    private var isAtEnd: Bool {
        return current >= source.count
    }
    
    private func advance() -> Character {
        let char = source[current]
        current += 1
        column += 1
        return char
    }
    
    private func peek() -> Character {
        if isAtEnd { return "\0" }
        return source[current]
    }
    
    private func peekNext() -> Character {
        if current + 1 >= source.count { return "\0" }
        return source[current + 1]
    }
    
    private func match(_ expected: Character) -> Bool {
        if isAtEnd { return false }
        if source[current] != expected { return false }
        current += 1
        column += 1
        return true
    }
    
    private func isDigit(_ c: Character) -> Bool {
        return c >= "0" && c <= "9"
    }
    
    private func isAlpha(_ c: Character) -> Bool {
        return (c >= "a" && c <= "z") ||
               (c >= "A" && c <= "Z") ||
               c == "_"
    }
    
    private func isAlphaNumeric(_ c: Character) -> Bool {
        return isAlpha(c) || isDigit(c)
    }
    
    // MARK: - Token Factory
    
    private func makeToken(_ type: TokenType) -> Token {
        let text = String(source[start..<current])
        let endLoc = currentLocation()
        
        let startLoc = SourceLocation(
            line: (type == .stringLiteral(text) && text.contains("\n")) ? line - text.filter { $0 == "\n" }.count : line,
            column: column - (current - start),
            offset: start
        )
        
        return Token(
            type: type,
            lexeme: text,
            span: SourceSpan(start: startLoc, end: endLoc)
        )
    }
    
    private func startLocation() -> SourceLocation {
        return SourceLocation(line: line, column: column - (current - start), offset: start)
    }
    
    private func currentLocation() -> SourceLocation {
        return SourceLocation(line: line, column: column, offset: current)
    }
}
