import Foundation

// Parser syntax nodes.
public enum ParsedNode {
    case program(Program)
    case declaration(Declaration)
    case statement(Statement)
    case expression(Expression)
    case type(TypeReference)
}

// Parsed source program.
public struct Program {
    public var declarations: [Declaration]
    public var statements: [Statement]

    public init(
        declarations: [Declaration] = [],
        statements: [Statement] = []
    ) {
        self.declarations = declarations
        self.statements = statements
    }
}

// Declaration syntax.
public enum Declaration {
    case function(FunctionDeclaration)
    case variable(VariableDeclaration)
    case type(TypeDeclaration)
    case classType(ClassDeclaration)
    case structType(StructDeclaration)
    case enumType(EnumDeclaration)
    case template(TemplateDeclaration)
    case protocolType(ProtocolDeclaration)
    case extensionType(ExtensionDeclaration)
    case importDeclaration(ImportDeclaration)
    case exportDeclaration(ExportDeclaration)
    case moduleDeclaration(ModuleDeclaration)
}

// Function declaration.
public struct FunctionDeclaration {
    public var modifiers: [String]
    public var name: String
    public var parameters: [ParameterDeclaration]
    public var returnType: TypeReference?
    public var body: [Statement]

    public init(
        modifiers: [String] = [],
        name: String,
        parameters: [ParameterDeclaration] = [],
        returnType: TypeReference? = nil,
        body: [Statement] = []
    ) {
        self.modifiers = modifiers
        self.name = name
        self.parameters = parameters
        self.returnType = returnType
        self.body = body
    }
}

// Function parameter declaration.
public struct ParameterDeclaration {
    public var externalName: String?
    public var name: String
    public var type: TypeReference?
    public var defaultValue: Expression?

    public init(
        externalName: String? = nil,
        name: String,
        type: TypeReference? = nil,
        defaultValue: Expression? = nil
    ) {
        self.externalName = externalName
        self.name = name
        self.type = type
        self.defaultValue = defaultValue
    }
}

// Variable declaration.
public struct VariableDeclaration {
    public var isMutable: Bool
    public var isConstant: Bool
    public var name: String
    public var type: TypeReference?
    public var initializer: Expression?

    public init(
        isMutable: Bool,
        isConstant: Bool = false,
        name: String,
        type: TypeReference? = nil,
        initializer: Expression? = nil
    ) {
        self.isMutable = isMutable
        self.isConstant = isConstant
        self.name = name
        self.type = type
        self.initializer = initializer
    }
}

// General type declaration.
public struct TypeDeclaration {
    public var name: String
    public var underlyingType: TypeReference?

    public init(
        name: String,
        underlyingType: TypeReference? = nil
    ) {
        self.name = name
        self.underlyingType = underlyingType
    }
}

// Class declaration.
public struct ClassDeclaration {
    public var modifiers: [String]
    public var name: String
    public var inheritedTypes: [TypeReference]
    public var members: [Declaration]
    public var statements: [Statement]

    public init(
        modifiers: [String] = [],
        name: String,
        inheritedTypes: [TypeReference] = [],
        members: [Declaration] = [],
        statements: [Statement] = []
    ) {
        self.modifiers = modifiers
        self.name = name
        self.inheritedTypes = inheritedTypes
        self.members = members
        self.statements = statements
    }
}

// Struct declaration.
public struct StructDeclaration {
    public var modifiers: [String]
    public var name: String
    public var inheritedTypes: [TypeReference]
    public var members: [Declaration]
    public var statements: [Statement]

    public init(
        modifiers: [String] = [],
        name: String,
        inheritedTypes: [TypeReference] = [],
        members: [Declaration] = [],
        statements: [Statement] = []
    ) {
        self.modifiers = modifiers
        self.name = name
        self.inheritedTypes = inheritedTypes
        self.members = members
        self.statements = statements
    }
}

// Enum declaration.
public struct EnumDeclaration {
    public var modifiers: [String]
    public var name: String
    public var cases: [EnumCase]
    public var members: [Declaration]

    public init(
        modifiers: [String] = [],
        name: String,
        cases: [EnumCase] = [],
        members: [Declaration] = []
    ) {
        self.modifiers = modifiers
        self.name = name
        self.cases = cases
        self.members = members
    }
}

// Enum case.
public struct EnumCase {
    public var name: String
    public var associatedValues: [TypeReference]

    public init(
        name: String,
        associatedValues: [TypeReference] = []
    ) {
        self.name = name
        self.associatedValues = associatedValues
    }
}

// Template declaration.
public struct TemplateDeclaration {
    public var modifiers: [String]
    public var name: String
    public var parameters: [String]
    public var members: [Declaration]
    public var statements: [Statement]

    public init(
        modifiers: [String] = [],
        name: String,
        parameters: [String] = [],
        members: [Declaration] = [],
        statements: [Statement] = []
    ) {
        self.modifiers = modifiers
        self.name = name
        self.parameters = parameters
        self.members = members
        self.statements = statements
    }
}

// Protocol declaration.
public struct ProtocolDeclaration {
    public var modifiers: [String]
    public var name: String
    public var inheritedTypes: [TypeReference]
    public var requirements: [Declaration]

    public init(
        modifiers: [String] = [],
        name: String,
        inheritedTypes: [TypeReference] = [],
        requirements: [Declaration] = []
    ) {
        self.modifiers = modifiers
        self.name = name
        self.inheritedTypes = inheritedTypes
        self.requirements = requirements
    }
}

// Extension declaration.
public struct ExtensionDeclaration {
    public var modifiers: [String]
    public var target: TypeReference
    public var inheritedTypes: [TypeReference]
    public var members: [Declaration]

    public init(
        modifiers: [String] = [],
        target: TypeReference,
        inheritedTypes: [TypeReference] = [],
        members: [Declaration] = []
    ) {
        self.modifiers = modifiers
        self.target = target
        self.inheritedTypes = inheritedTypes
        self.members = members
    }
}

// Import declaration.
public struct ImportDeclaration {
    public var path: [String]

    public init(path: [String]) {
        self.path = path
    }
}

// Export declaration.
public struct ExportDeclaration {
    public var name: String

    public init(name: String) {
        self.name = name
    }
}

// Module declaration.
public struct ModuleDeclaration {
    public var name: String

    public init(name: String) {
        self.name = name
    }
}

// Statement syntax.
public enum Statement {
    case expression(Expression)
    case variable(VariableDeclaration)
    case returnStatement(Expression?)
    case ifStatement(IfStatement)
    case guardStatement(GuardStatement)
    case matchStatement(MatchStatement)
    case whileStatement(WhileStatement)
    case forStatement(ForStatement)
    case breakStatement
    case continueStatement
    case throwStatement(Expression)
    case tryStatement(TryStatement)
    case deferStatement([Statement])
    case block([Statement])
}

// If statement.
public struct IfStatement {
    public var condition: Expression
    public var body: [Statement]
    public var elseBody: [Statement]?

    public init(
        condition: Expression,
        body: [Statement],
        elseBody: [Statement]? = nil
    ) {
        self.condition = condition
        self.body = body
        self.elseBody = elseBody
    }
}

// Guard statement.
public struct GuardStatement {
    public var condition: Expression
    public var body: [Statement]

    public init(
        condition: Expression,
        body: [Statement]
    ) {
        self.condition = condition
        self.body = body
    }
}

// Match statement.
public struct MatchStatement {
    public var expression: Expression
    public var cases: [MatchCase]

    public init(
        expression: Expression,
        cases: [MatchCase] = []
    ) {
        self.expression = expression
        self.cases = cases
    }
}

// Match case.
public struct MatchCase {
    public var pattern: Expression?
    public var statements: [Statement]
    public var isDefault: Bool

    public init(
        pattern: Expression? = nil,
        statements: [Statement] = [],
        isDefault: Bool = false
    ) {
        self.pattern = pattern
        self.statements = statements
        self.isDefault = isDefault
    }
}

// While statement.
public struct WhileStatement {
    public var condition: Expression
    public var body: [Statement]

    public init(
        condition: Expression,
        body: [Statement]
    ) {
        self.condition = condition
        self.body = body
    }
}

// For statement.
public struct ForStatement {
    public var name: String
    public var sequence: Expression
    public var body: [Statement]

    public init(
        name: String,
        sequence: Expression,
        body: [Statement]
    ) {
        self.name = name
        self.sequence = sequence
        self.body = body
    }
}

// Try statement.
public struct TryStatement {
    public var body: [Statement]
    public var catches: [CatchClause]

    public init(
        body: [Statement] = [],
        catches: [CatchClause] = []
    ) {
        self.body = body
        self.catches = catches
    }
}

// Catch clause.
public struct CatchClause {
    public var name: String?
    public var type: TypeReference?
    public var body: [Statement]

    public init(
        name: String? = nil,
        type: TypeReference? = nil,
        body: [Statement] = []
    ) {
        self.name = name
        self.type = type
        self.body = body
    }
}

// Expression syntax.
public indirect enum Expression {
    case identifier(String)
    case integer(String)
    case floating(String)
    case string(String)
    case character(String)
    case boolean(Bool)
    case nilValue
    case selfExpression
    case unary(TokenKind, Expression)
    case binary(Expression, TokenKind, Expression)
    case assignment(Expression, TokenKind, Expression)
    case call(Expression, [Expression])
    case member(Expression, String)
    case subscriptExpression(Expression, [Expression])
    case functionLiteral([ParameterDeclaration], [Statement])
    case parenthesized(Expression)
}

// Type reference.
public indirect enum TypeReference {
    case named(String)
    case optional(TypeReference)
    case generic(String, [TypeReference])
    case function([TypeReference], TypeReference?)
}

// Parser error severity.
public enum ParserDiagnosticSeverity {
    case error
    case warning
}

// Parser diagnostic.
public struct ParserDiagnostic {
    public let severity: ParserDiagnosticSeverity
    public let message: String
    public let range: SourceRange

    public init(
        severity: ParserDiagnosticSeverity,
        message: String,
        range: SourceRange
    ) {
        self.severity = severity
        self.message = message
        self.range = range
    }
}

// Parser configuration.
public struct ParserConfiguration {
    public var allowTopLevelStatements: Bool
    public var allowTrailingCommas: Bool
    public var allowEmptyBodies: Bool

    public init(
        allowTopLevelStatements: Bool = true,
        allowTrailingCommas: Bool = true,
        allowEmptyBodies: Bool = true
    ) {
        self.allowTopLevelStatements = allowTopLevelStatements
        self.allowTrailingCommas = allowTrailingCommas
        self.allowEmptyBodies = allowEmptyBodies
    }
}

// Main parser.
public final class Parser {
    private let tokens: [Token]
    private let configuration: ParserConfiguration

    private var index: Int = 0
    private var diagnostics: [ParserDiagnostic] = []

    public init(
        tokens: [Token],
        configuration: ParserConfiguration = ParserConfiguration()
    ) {
        self.tokens = tokens
        self.configuration = configuration
    }

    public convenience init(
        source: String,
        configuration: ParserConfiguration = ParserConfiguration()
    ) {
        let lexer = Lexer(source: source)
        let tokens = lexer.tokenize()

        self.init(
            tokens: tokens,
            configuration: configuration
        )
    }

    public func parse() -> Program {
        var declarations: [Declaration] = []
        var statements: [Statement] = []

        while !isAtEnd {
            if let declaration = parseDeclaration() {
                declarations.append(declaration)
                continue
            }

            if configuration.allowTopLevelStatements,
               let statement = parseStatement() {
                statements.append(statement)
                continue
            }

            synchronize()
        }

        return Program(
            declarations: declarations,
            statements: statements
        )
    }

    public func parseWithDiagnostics() -> (Program, [ParserDiagnostic]) {
        let program = parse()
        return (program, diagnostics)
    }

    public func getDiagnostics() -> [ParserDiagnostic] {
        diagnostics
    }

    // Checks whether parsing has reached the end.
    private var isAtEnd: Bool {
        peek().kind == .endOfFile
    }

    // Returns the current token.
    private func peek() -> Token {
        tokens[min(index, tokens.count - 1)]
    }

    // Returns the previous token.
    private func previous() -> Token {
        tokens[max(0, index - 1)]
    }

    // Advances to the next token.
    @discardableResult
    private func advance() -> Token {
        if !isAtEnd {
            index += 1
        }

        return previous()
    }

    // Checks the current token kind.
    private func check(_ kind: TokenKind) -> Bool {
        peek().kind == kind
    }

    // Checks a keyword.
    private func checkKeyword(_ keyword: String) -> Bool {
        peek().kind == .keyword(keyword)
    }

    // Consumes a token kind.
    @discardableResult
    private func match(_ kind: TokenKind) -> Bool {
        guard check(kind) else {
            return false
        }

        advance()
        return true
    }

    // Consumes a keyword.
    @discardableResult
    private func matchKeyword(_ keyword: String) -> Bool {
        guard checkKeyword(keyword) else {
            return false
        }

        advance()
        return true
    }

    // Consumes a required token.
    private func consume(
        _ kind: TokenKind,
        message: String
    ) -> Token? {
        if check(kind) {
            return advance()
        }

        addError(message)
        return nil
    }

    // Consumes a required keyword.
    private func consumeKeyword(
        _ keyword: String,
        message: String
    ) -> Token? {
        if checkKeyword(keyword) {
            return advance()
        }

        addError(message)
        return nil
    }

    // Parses a declaration.
    private func parseDeclaration() -> Declaration? {
        let modifiers = parseModifiers()

        if checkKeyword("func") {
            return parseFunction(modifiers: modifiers)
        }

        if checkKeyword("var") || checkKeyword("let") {
            return parseVariableDeclaration(
                modifiers: modifiers,
                asTopLevel: true
            )
        }

        if checkKeyword("type") {
            return parseTypeDeclaration()
        }

        if checkKeyword("class") {
            return parseClassDeclaration(modifiers: modifiers)
        }

        if checkKeyword("struct") {
            return parseStructDeclaration(modifiers: modifiers)
        }

        if checkKeyword("enum") {
            return parseEnumDeclaration(modifiers: modifiers)
        }

        if checkKeyword("template") {
            return parseTemplateDeclaration(modifiers: modifiers)
        }

        if checkKeyword("proto") {
            return parseProtocolDeclaration(modifiers: modifiers)
        }

        if checkKeyword("extend") {
            return parseExtensionDeclaration(modifiers: modifiers)
        }

        if checkKeyword("import") {
            return parseImportDeclaration()
        }

        if checkKeyword("export") {
            return parseExportDeclaration()
        }

        if checkKeyword("module") {
            return parseModuleDeclaration()
        }

        if !modifiers.isEmpty {
            addError("expected declaration after modifiers")
        }

        return nil
    }

    // Parses access and declaration modifiers.
    private func parseModifiers() -> [String] {
        var modifiers: [String] = []

        let names = [
            "public",
            "private",
            "protected",
            "static",
            "final",
            "const"
        ]

        while let modifier = names.first(where: { checkKeyword($0) }) {
            modifiers.append(modifier)
            advance()
        }

        return modifiers
    }

    // Parses a function declaration.
    private func parseFunction(
        modifiers: [String]
    ) -> Declaration? {
        guard consumeKeyword(
            "func",
            message: "expected 'func'"
        ) != nil else {
            return nil
        }

        guard let name = consumeIdentifier(
            message: "expected function name"
        ) else {
            synchronizeDeclaration()
            return nil
        }

        guard consume(
            .leftParen,
            message: "expected '(' after function name"
        ) != nil else {
            return nil
        }

        let parameters = parseParameters()

        guard consume(
            .rightParen,
            message: "expected ')' after parameters"
        ) != nil else {
            return nil
        }

        var returnType: TypeReference?

        if match(.arrow) {
            returnType = parseTypeReference()
        }

        let body = parseRequiredBlock()

        return .function(
            FunctionDeclaration(
                modifiers: modifiers,
                name: name,
                parameters: parameters,
                returnType: returnType,
                body: body
            )
        )
    }

    // Parses function parameters.
    private func parseParameters() -> [ParameterDeclaration] {
        var parameters: [ParameterDeclaration] = []

        if check(.rightParen) {
            return parameters
        }

        repeat {
            guard let firstName = consumeIdentifier(
                message: "expected parameter name"
            ) else {
                synchronizeParameterList()
                break
            }

            var externalName: String?
            var name = firstName

            if check(.identifier) {
                externalName = firstName
                name = advance().lexeme
            }

            var type: TypeReference?

            if match(.colon) {
                type = parseTypeReference()
            }

            var defaultValue: Expression?

            if match(.equal) {
                defaultValue = parseExpression()
            }

            parameters.append(
                ParameterDeclaration(
                    externalName: externalName,
                    name: name,
                    type: type,
                    defaultValue: defaultValue
                )
            )
        } while match(.comma) && !check(.rightParen)

        return parameters
    }

    // Parses a variable declaration.
    private func parseVariableDeclaration(
        modifiers: [String] = [],
        asTopLevel: Bool = false
    ) -> Declaration? {
        let keyword: String

        if matchKeyword("var") {
            keyword = "var"
        } else if matchKeyword("let") {
            keyword = "let"
        } else {
            addError("expected 'var' or 'let'")
            return nil
        }

        guard let name = consumeIdentifier(
            message: "expected variable name"
        ) else {
            return nil
        }

        var type: TypeReference?

        if match(.colon) {
            type = parseTypeReference()
        }

        var initializer: Expression?

        if match(.equal) {
            initializer = parseExpression()
        }

        if initializer == nil && type == nil {
            addError("variable declaration requires a type or initializer")
        }

        match(.semicolon)

        return .variable(
            VariableDeclaration(
                isMutable: keyword == "var",
                isConstant: keyword == "let",
                name: name,
                type: type,
                initializer: initializer
            )
        )
    }

    // Parses a type alias declaration.
    private func parseTypeDeclaration() -> Declaration? {
        guard consumeKeyword(
            "type",
            message: "expected 'type'"
        ) != nil else {
            return nil
        }

        guard let name = consumeIdentifier(
            message: "expected type name"
        ) else {
            return nil
        }

        var underlyingType: TypeReference?

        if match(.equal) {
            underlyingType = parseTypeReference()
        }

        match(.semicolon)

        return .type(
            TypeDeclaration(
                name: name,
                underlyingType: underlyingType
            )
        )
    }

    // Parses a class declaration.
    private func parseClassDeclaration(
        modifiers: [String]
    ) -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected class name"
        ) else {
            return nil
        }

        let inheritedTypes = parseInheritedTypes()
        let (members, statements) = parseTypeBody()

        return .classType(
            ClassDeclaration(
                modifiers: modifiers,
                name: name,
                inheritedTypes: inheritedTypes,
                members: members,
                statements: statements
            )
        )
    }

    // Parses a struct declaration.
    private func parseStructDeclaration(
        modifiers: [String]
    ) -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected struct name"
        ) else {
            return nil
        }

        let inheritedTypes = parseInheritedTypes()
        let (members, statements) = parseTypeBody()

        return .structType(
            StructDeclaration(
                modifiers: modifiers,
                name: name,
                inheritedTypes: inheritedTypes,
                members: members,
                statements: statements
            )
        )
    }

    // Parses an enum declaration.
    private func parseEnumDeclaration(
        modifiers: [String]
    ) -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected enum name"
        ) else {
            return nil
        }

        guard consume(
            .leftBrace,
            message: "expected '{' after enum name"
        ) != nil else {
            return nil
        }

        var cases: [EnumCase] = []
        var members: [Declaration] = []

        while !check(.rightBrace) && !isAtEnd {
            if matchKeyword("case") {
                guard let caseName = consumeIdentifier(
                    message: "expected enum case name"
                ) else {
                    synchronizeDeclaration()
                    continue
                }

                var associatedValues: [TypeReference] = []

                if match(.leftParen) {
                    if !check(.rightParen) {
                        repeat {
                            if let type = parseTypeReference() {
                                associatedValues.append(type)
                            }
                        } while match(.comma) && !check(.rightParen)
                    }

                    consume(
                        .rightParen,
                        message: "expected ')' after enum case values"
                    )
                }

                cases.append(
                    EnumCase(
                        name: caseName,
                        associatedValues: associatedValues
                    )
                )

                match(.comma)
                match(.semicolon)
                continue
            }

            if let declaration = parseDeclaration() {
                members.append(declaration)
                continue
            }

            synchronizeDeclaration()
        }

        consume(
            .rightBrace,
            message: "expected '}' after enum declaration"
        )

        return .enumType(
            EnumDeclaration(
                modifiers: modifiers,
                name: name,
                cases: cases,
                members: members
            )
        )
    }

    // Parses a template declaration.
    private func parseTemplateDeclaration(
        modifiers: [String]
    ) -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected template name"
        ) else {
            return nil
        }

        var parameters: [String] = []

        if match(.less) {
            if !check(.greater) {
                repeat {
                    if let parameter = consumeIdentifier(
                        message: "expected template parameter"
                    ) {
                        parameters.append(parameter)
                    }
                } while match(.comma) && !check(.greater)
            }

            consume(
                .greater,
                message: "expected '>' after template parameters"
            )
        }

        let (members, statements) = parseTypeBody()

        return .template(
            TemplateDeclaration(
                modifiers: modifiers,
                name: name,
                parameters: parameters,
                members: members,
                statements: statements
            )
        )
    }

    // Parses a protocol declaration.
    private func parseProtocolDeclaration(
        modifiers: [String]
    ) -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected protocol name"
        ) else {
            return nil
        }

        let inheritedTypes = parseInheritedTypes()

        guard consume(
            .leftBrace,
            message: "expected '{' after protocol name"
        ) != nil else {
            return nil
        }

        var requirements: [Declaration] = []

        while !check(.rightBrace) && !isAtEnd {
            if let declaration = parseDeclaration() {
                requirements.append(declaration)
                continue
            }

            synchronizeDeclaration()
        }

        consume(
            .rightBrace,
            message: "expected '}' after protocol declaration"
        )

        return .protocolType(
            ProtocolDeclaration(
                modifiers: modifiers,
                name: name,
                inheritedTypes: inheritedTypes,
                requirements: requirements
            )
        )
    }

    // Parses an extension declaration.
    private func parseExtensionDeclaration(
        modifiers: [String]
    ) -> Declaration? {
        advance()

        guard let target = parseTypeReference() else {
            return nil
        }

        let inheritedTypes = parseInheritedTypes()
        let (members, _) = parseTypeBody()

        return .extensionType(
            ExtensionDeclaration(
                modifiers: modifiers,
                target: target,
                inheritedTypes: inheritedTypes,
                members: members
            )
        )
    }

    // Parses inherited types.
    private func parseInheritedTypes() -> [TypeReference] {
        guard match(.colon) else {
            return []
        }

        var types: [TypeReference] = []

        repeat {
            if let type = parseTypeReference() {
                types.append(type)
            }
        } while match(.comma)

        return types
    }

    // Parses the body of a type declaration.
    private func parseTypeBody() -> ([Declaration], [Statement]) {
        guard consume(
            .leftBrace,
            message: "expected '{' before type body"
        ) != nil else {
            return ([], [])
        }

        var members: [Declaration] = []
        var statements: [Statement] = []

        while !check(.rightBrace) && !isAtEnd {
            if let declaration = parseDeclaration() {
                members.append(declaration)
                continue
            }

            if let statement = parseStatement() {
                statements.append(statement)
                continue
            }

            synchronizeDeclaration()
        }

        consume(
            .rightBrace,
            message: "expected '}' after type body"
        )

        return (members, statements)
    }

    // Parses an import declaration.
    private func parseImportDeclaration() -> Declaration? {
        advance()

        var path: [String] = []

        guard let first = consumeIdentifier(
            message: "expected module name after 'import'"
        ) else {
            return nil
        }

        path.append(first)

        while match(.dot) {
            guard let component = consumeIdentifier(
                message: "expected module path component"
            ) else {
                break
            }

            path.append(component)
        }

        match(.semicolon)

        return .importDeclaration(
            ImportDeclaration(path: path)
        )
    }

    // Parses an export declaration.
    private func parseExportDeclaration() -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected name after 'export'"
        ) else {
            return nil
        }

        match(.semicolon)

        return .exportDeclaration(
            ExportDeclaration(name: name)
        )
    }

    // Parses a module declaration.
    private func parseModuleDeclaration() -> Declaration? {
        advance()

        guard let name = consumeIdentifier(
            message: "expected module name"
        ) else {
            return nil
        }

        match(.semicolon)

        return .moduleDeclaration(
            ModuleDeclaration(name: name)
        )
    }

    // Parses a statement.
    private func parseStatement() -> Statement? {
        if checkKeyword("var") || checkKeyword("let") {
            guard let declaration = parseVariableDeclaration(
                asTopLevel: false
            ) else {
                return nil
            }

            if case .variable(let variable) = declaration {
                return .variable(variable)
            }

            return nil
        }

        if matchKeyword("return") {
            var value: Expression?

            if !check(.rightBrace) && !check(.semicolon) {
                value = parseExpression()
            }

            match(.semicolon)

            return .returnStatement(value)
        }

        if matchKeyword("if") {
            return parseIfStatement()
        }

        if matchKeyword("guard") {
            return parseGuardStatement()
        }

        if matchKeyword("match") {
            return parseMatchStatement()
        }

        if matchKeyword("while") {
            return parseWhileStatement()
        }

        if matchKeyword("for") {
            return parseForStatement()
        }

        if matchKeyword("break") {
            match(.semicolon)
            return .breakStatement
        }

        if matchKeyword("continue") {
            match(.semicolon)
            return .continueStatement
        }

        if matchKeyword("throw") {
            guard let expression = parseExpression() else {
                return nil
            }

            match(.semicolon)
            return .throwStatement(expression)
        }

        if matchKeyword("try") {
            return parseTryStatement()
        }

        if matchKeyword("defer") {
            let body = parseRequiredBlock()
            return .deferStatement(body)
        }

        if check(.leftBrace) {
            return .block(parseRequiredBlock())
        }

        if let expression = parseExpression() {
            match(.semicolon)
            return .expression(expression)
        }

        return nil
    }

    // Parses an if statement.
    private func parseIfStatement() -> Statement {
        guard let condition = parseExpression() else {
            return .ifStatement(
                IfStatement(
                    condition: .boolean(false),
                    body: [],
                    elseBody: nil
                )
            )
        }

        let body = parseRequiredBlock()
        var elseBody: [Statement]?

        if matchKeyword("else") {
            if matchKeyword("if") {
                let nested = parseIfStatement()
                elseBody = [nested]
            } else {
                elseBody = parseRequiredBlock()
            }
        }

        return .ifStatement(
            IfStatement(
                condition: condition,
                body: body,
                elseBody: elseBody
            )
        )
    }

    // Parses a guard statement.
    private func parseGuardStatement() -> Statement {
        guard let condition = parseExpression() else {
            return .guardStatement(
                GuardStatement(
                    condition: .boolean(false),
                    body: []
                )
            )
        }

        let body = parseRequiredBlock()

        return .guardStatement(
            GuardStatement(
                condition: condition,
                body: body
            )
        )
    }

    // Parses a match statement.
    private func parseMatchStatement() -> Statement {
        guard let expression = parseExpression() else {
            return .matchStatement(
                MatchStatement(
                    expression: .nilValue,
                    cases: []
                )
            )
        }

        guard consume(
            .leftBrace,
            message: "expected '{' after match expression"
        ) != nil else {
            return .matchStatement(
                MatchStatement(
                    expression: expression,
                    cases: []
                )
            )
        }

        var cases: [MatchCase] = []

        while !check(.rightBrace) && !isAtEnd {
            if matchKeyword("case") {
                let pattern = parseExpression()

                guard consume(
                    .colon,
                    message: "expected ':' after match case"
                ) != nil else {
                    synchronizeMatchCase()
                    continue
                }

                let statements = parseCaseStatements()

                cases.append(
                    MatchCase(
                        pattern: pattern,
                        statements: statements,
                        isDefault: false
                    )
                )

                continue
            }

            if matchKeyword("default") {
                consume(
                    .colon,
                    message: "expected ':' after default"
                )

                let statements = parseCaseStatements()

                cases.append(
                    MatchCase(
                        pattern: nil,
                        statements: statements,
                        isDefault: true
                    )
                )

                continue
            }

            synchronizeMatchCase()
        }

        consume(
            .rightBrace,
            message: "expected '}' after match"
        )

        return .matchStatement(
            MatchStatement(
                expression: expression,
                cases: cases
            )
        )
    }

    // Parses match case statements.
    private func parseCaseStatements() -> [Statement] {
        var statements: [Statement] = []

        while !checkKeyword("case") &&
              !checkKeyword("default") &&
              !check(.rightBrace) &&
              !isAtEnd {
            if let statement = parseStatement() {
                statements.append(statement)
            } else {
                synchronizeStatement()
            }
        }

        return statements
    }

    // Parses a while statement.
    private func parseWhileStatement() -> Statement {
        let condition = parseExpression() ?? .boolean(false)
        let body = parseRequiredBlock()

        return .whileStatement(
            WhileStatement(
                condition: condition,
                body: body
            )
        )
    }

    // Parses a for statement.
    private func parseForStatement() -> Statement {
        guard let name = consumeIdentifier(
            message: "expected loop variable"
        ) else {
            return .forStatement(
                ForStatement(
                    name: "_",
                    sequence: .nilValue,
                    body: []
                )
            )
        }

        consumeKeyword(
            "in",
            message: "expected 'in' in for statement"
        )

        let sequence = parseExpression() ?? .nilValue
        let body = parseRequiredBlock()

        return .forStatement(
            ForStatement(
                name: name,
                sequence: sequence,
                body: body
            )
        )
    }

    // Parses a try statement.
    private func parseTryStatement() -> Statement {
        let body = parseRequiredBlock()
        var catches: [CatchClause] = []

        while matchKeyword("catch") {
            var name: String?
            var type: TypeReference?

            if match(.leftParen) {
                name = consumeIdentifier(
                    message: "expected catch variable"
                )

                if match(.colon) {
                    type = parseTypeReference()
                }

                consume(
                    .rightParen,
                    message: "expected ')' after catch variable"
                )
            }

            let catchBody = parseRequiredBlock()

            catches.append(
                CatchClause(
                    name: name,
                    type: type,
                    body: catchBody
                )
            )
        }

        return .tryStatement(
            TryStatement(
                body: body,
                catches: catches
            )
        )
    }

    // Parses a required block.
    private func parseRequiredBlock() -> [Statement] {
        guard consume(
            .leftBrace,
            message: "expected '{'"
        ) != nil else {
            return []
        }

        var statements: [Statement] = []

        while !check(.rightBrace) && !isAtEnd {
            if let statement = parseStatement() {
                statements.append(statement)
            } else {
                synchronizeStatement()
            }
        }

        consume(
            .rightBrace,
            message: "expected '}'"
        )

        return statements
    }

    // Parses a type reference.
    private func parseTypeReference() -> TypeReference? {
        guard let name = consumeTypeName() else {
            addError("expected type name")
            return nil
        }

        var type: TypeReference = .named(name)

        if match(.less) {
            var arguments: [TypeReference] = []

            if !check(.greater) {
                repeat {
                    if let argument = parseTypeReference() {
                        arguments.append(argument)
                    }
                } while match(.comma) && !check(.greater)
            }

            consume(
                .greater,
                message: "expected '>' after generic type"
            )

            type = .generic(
                name,
                arguments
            )
        }

        if match(.question) {
            type = .optional(type)
        }

        return type
    }

    // Consumes a type name.
    private func consumeTypeName() -> String? {
        if case .identifier = peek().kind {
            return advance().lexeme
        }

        if case .keyword(let value) = peek().kind {
            let typeKeywords = [
                "int",
                "uint",
                "float",
                "double",
                "bool",
                "string",
                "char",
                "type",
                "auto"
            ]

            if typeKeywords.contains(value) {
                return advance().lexeme
            }
        }

        return nil
    }

    // Parses an expression.
    private func parseExpression() -> Expression? {
        parseAssignment()
    }

    // Parses assignment expressions.
    private func parseAssignment() -> Expression? {
        guard var expression = parseLogicalOr() else {
            return nil
        }

        let assignmentOperators: [TokenKind] = [
            .equal,
            .plusEqual,
            .minusEqual,
            .starEqual,
            .slashEqual,
            .percentEqual
        ]

        if assignmentOperators.contains(peek().kind) {
            let operation = advance().kind

            guard let value = parseAssignment() else {
                addError("expected expression after assignment operator")
                return expression
            }

            expression = .assignment(
                expression,
                operation,
                value
            )
        }

        return expression
    }

    // Parses logical OR expressions.
    private func parseLogicalOr() -> Expression? {
        guard var expression = parseLogicalAnd() else {
            return nil
        }

        while match(.or) {
            guard let right = parseLogicalAnd() else {
                addError("expected expression after '||'")
                break
            }

            expression = .binary(
                expression,
                .or,
                right
            )
        }

        return expression
    }

    // Parses logical AND expressions.
    private func parseLogicalAnd() -> Expression? {
        guard var expression = parseEquality() else {
            return nil
        }

        while match(.and) {
            guard let right = parseEquality() else {
                addError("expected expression after '&&'")
                break
            }

            expression = .binary(
                expression,
                .and,
                right
            )
        }

        return expression
    }

    // Parses equality expressions.
    private func parseEquality() -> Expression? {
        guard var expression = parseComparison() else {
            return nil
        }

        while check(.equalEqual) || check(.notEqual) {
            let operation = advance().kind

            guard let right = parseComparison() else {
                addError("expected expression after equality operator")
                break
            }

            expression = .binary(
                expression,
                operation,
                right
            )
        }

        return expression
    }

    // Parses comparison expressions.
    private func parseComparison() -> Expression? {
        guard var expression = parseTerm() else {
            return nil
        }

        while check(.less) ||
              check(.lessEqual) ||
              check(.greater) ||
              check(.greaterEqual) {
            let operation = advance().kind

            guard let right = parseTerm() else {
                addError("expected expression after comparison operator")
                break
            }

            expression = .binary(
                expression,
                operation,
                right
            )
        }

        return expression
    }

    // Parses additive expressions.
    private func parseTerm() -> Expression? {
        guard var expression = parseFactor() else {
            return nil
        }

        while check(.plus) || check(.minus) {
            let operation = advance().kind

            guard let right = parseFactor() else {
                addError("expected expression after additive operator")
                break
            }

            expression = .binary(
                expression,
                operation,
                right
            )
        }

        return expression
    }

    // Parses multiplicative expressions.
    private func parseFactor() -> Expression? {
        guard var expression = parseUnary() else {
            return nil
        }

        while check(.star) ||
              check(.slash) ||
              check(.percent) {
            let operation = advance().kind

            guard let right = parseUnary() else {
                addError("expected expression after multiplicative operator")
                break
            }

            expression = .binary(
                expression,
                operation,
                right
            )
        }

        return expression
    }

    // Parses unary expressions.
    private func parseUnary() -> Expression? {
        if check(.not) ||
           check(.minus) ||
           check(.plus) ||
           check(.exclamation) {
            let operation = advance().kind

            guard let operand = parseUnary() else {
                addError("expected expression after unary operator")
                return nil
            }

            return .unary(
                operation,
                operand
            )
        }

        return parsePostfix()
    }

    // Parses postfix expressions.
    private func parsePostfix() -> Expression? {
        guard var expression = parsePrimary() else {
            return nil
        }

        while true {
            if match(.leftParen) {
                var arguments: [Expression] = []

                if !check(.rightParen) {
                    repeat {
                        if let argument = parseExpression() {
                            arguments.append(argument)
                        }
                    } while match(.comma) && !check(.rightParen)
                }

                consume(
                    .rightParen,
                    message: "expected ')' after arguments"
                )

                expression = .call(
                    expression,
                    arguments
                )

                continue
            }

            if match(.dot) {
                guard let member = consumeIdentifier(
                    message: "expected member name after '.'"
                ) else {
                    break
                }

                expression = .member(
                    expression,
                    member
                )

                continue
            }

            if match(.leftBracket) {
                var arguments: [Expression] = []

                if !check(.rightBracket) {
                    repeat {
                        if let argument = parseExpression() {
                            arguments.append(argument)
                        }
                    } while match(.comma) && !check(.rightBracket)
                }

                consume(
                    .rightBracket,
                    message: "expected ']' after subscript"
                )

                expression = .subscriptExpression(
                    expression,
                    arguments
                )

                continue
            }

            break
        }

        return expression
    }

    // Parses primary expressions.
    private func parsePrimary() -> Expression? {
        let token = peek()

        switch token.kind {
        case .integerLiteral:
            advance()
            return .integer(token.lexeme)

        case .floatingLiteral:
            advance()
            return .floating(token.lexeme)

        case .stringLiteral:
            advance()
            return .string(token.lexeme)

        case .characterLiteral:
            advance()
            return .character(token.lexeme)

        case .identifier:
            advance()
            return .identifier(token.lexeme)

        case .keyword("true"):
            advance()
            return .boolean(true)

        case .keyword("false"):
            advance()
            return .boolean(false)

        case .keyword("nil"):
            advance()
            return .nilValue

        case .keyword("self"):
            advance()
            return .selfExpression

        case .leftParen:
            advance()

            guard let expression = parseExpression() else {
                consume(
                    .rightParen,
                    message: "expected ')'"
                )
                return nil
            }

            consume(
                .rightParen,
                message: "expected ')' after expression"
            )

            return .parenthesized(expression)

        default:
            return nil
        }
    }

    // Consumes an identifier.
    private func consumeIdentifier(
        message: String
    ) -> String? {
        if case .identifier = peek().kind {
            return advance().lexeme
        }

        addError(message)
        return nil
    }

    // Synchronizes after a declaration error.
    private func synchronizeDeclaration() {
        while !isAtEnd {
            if check(.semicolon) {
                advance()
                return
            }

            if check(.rightBrace) {
                return
            }

            if checkKeyword("func") ||
               checkKeyword("class") ||
               checkKeyword("struct") ||
               checkKeyword("enum") ||
               checkKeyword("template") ||
               checkKeyword("proto") ||
               checkKeyword("extend") ||
               checkKeyword("import") {
                return
            }

            advance()
        }
    }

    // Synchronizes after a statement error.
    private func synchronizeStatement() {
        while !isAtEnd {
            if check(.semicolon) {
                advance()
                return
            }

            if check(.rightBrace) {
                return
            }

            if checkKeyword("if") ||
               checkKeyword("for") ||
               checkKeyword("while") ||
               checkKeyword("return") ||
               checkKeyword("var") ||
               checkKeyword("let") ||
               checkKeyword("match") {
                return
            }

            advance()
        }
    }

    // Synchronizes a parameter list.
    private func synchronizeParameterList() {
        while !isAtEnd {
            if check(.comma) || check(.rightParen) {
                return
            }

            advance()
        }
    }

    // Synchronizes a match case.
    private func synchronizeMatchCase() {
        while !isAtEnd {
            if checkKeyword("case") ||
               checkKeyword("default") ||
               check(.rightBrace) {
                return
            }

            advance()
        }
    }

    // Adds a parser error.
    private func addError(_ message: String) {
        let token = peek()

        diagnostics.append(
            ParserDiagnostic(
                severity: .error,
                message: message,
                range: token.range
            )
        )
    }
}

// Parser result.
public struct ParserResult {
    public let program: Program
    public let diagnostics: [ParserDiagnostic]

    public init(
        program: Program,
        diagnostics: [ParserDiagnostic]
    ) {
        self.program = program
        self.diagnostics = diagnostics
    }

    public var succeeded: Bool {
        diagnostics.allSatisfy {
            $0.severity != .error
        }
    }
}

// Parser convenience API.
public enum LanguageParser {
    public static func parse(
        _ source: String,
        configuration: ParserConfiguration = ParserConfiguration()
    ) -> ParserResult {
        let parser = Parser(
            source: source,
            configuration: configuration
        )

        let (program, diagnostics) = parser.parseWithDiagnostics()

        return ParserResult(
            program: program,
            diagnostics: diagnostics
        )
    }

    public static func parse(
        tokens: [Token],
        configuration: ParserConfiguration = ParserConfiguration()
    ) -> ParserResult {
        let parser = Parser(
            tokens: tokens,
            configuration: configuration
        )

        let (program, diagnostics) = parser.parseWithDiagnostics()

        return ParserResult(
            program: program,
            diagnostics: diagnostics
        )
    }
}

// Parser diagnostic formatter.
public enum ParserDiagnosticFormatter {
    public static func format(
        _ diagnostic: ParserDiagnostic
    ) -> String {
        let severity: String

        switch diagnostic.severity {
        case .error:
            severity = "error"

        case .warning:
            severity = "warning"
        }

        let position = diagnostic.range.start

        return "\(severity): \(diagnostic.message) at " +
            "\(position.line):\(position.column)"
    }

    public static func formatAll(
        _ diagnostics: [ParserDiagnostic]
    ) -> String {
        diagnostics
            .map(format)
            .joined(separator: "\n")
    }
}

// Parser test source.
public enum ParserTestSource {
    public static let function = """
    func add(first: int, second: int) -> int {
        return first + second
    }
    """

    public static let variables = """
    let answer: int = 42
    var count: int = 10
    """

    public static let controlFlow = """
    func test(value: int) -> string {
        if value > 10 {
            return "large"
        } else {
            return "small"
        }
    }
    """

    public static let loop = """
    func sum(values: int) -> int {
        var result: int = 0

        for item in values {
            result = result + item
        }

        return result
    }
    """

    public static let match = """
    func describe(value: int) -> string {
        match value {
            case 0:
                return "zero"
            case 1:
                return "one"
            default:
                return "other"
        }
    }
    """

    public static let types = """
    class Person {
        var name: string

        func greet() {
            return
        }
    }

    struct Point {
        var x: int
        var y: int
    }

    enum Result {
        case success
        case failure
    }
    """
}

// Parser smoke tests.
public enum ParserSmokeTests {
    public static func run() -> Bool {
        testFunction() &&
        testVariables() &&
        testControlFlow() &&
        testLoop() &&
        testMatch() &&
        testTypes()
    }

    private static func testFunction() -> Bool {
        let result = LanguageParser.parse(
            ParserTestSource.function
        )

        return result.succeeded &&
            result.program.declarations.count == 1
    }

    private static func testVariables() -> Bool {
        let result = LanguageParser.parse(
            ParserTestSource.variables
        )

        return result.succeeded &&
            result.program.declarations.count == 2
    }

    private static func testControlFlow() -> Bool {
        let result = LanguageParser.parse(
            ParserTestSource.controlFlow
        )

        return result.succeeded
    }

    private static func testLoop() -> Bool {
        let result = LanguageParser.parse(
            ParserTestSource.loop
        )

        return result.succeeded
    }

    private static func testMatch() -> Bool {
        let result = LanguageParser.parse(
            ParserTestSource.match
        )

        return result.succeeded
    }

    private static func testTypes() -> Bool {
        let result = LanguageParser.parse(
            ParserTestSource.types
        )

        return result.succeeded &&
            result.program.declarations.count == 3
    }
}

// Parser module information.
public enum ParserModuleInfo {
    public static let name = "LanguageParser"
    public static let version = "0.1.0"
    public static let implementationLanguage = "Swift"
    public static let targetLanguage = "Swift"
}

// Parser validation.
public enum ParserValidator {
    public static func validate(
        _ source: String
    ) -> Bool {
        LanguageParser.parse(source).succeeded
    }
}

// Parser source service.
public final class ParserService {
    private let configuration: ParserConfiguration

    public init(
        configuration: ParserConfiguration = ParserConfiguration()
    ) {
        self.configuration = configuration
    }

    public func parse(_ source: String) -> ParserResult {
        LanguageParser.parse(
            source,
            configuration: configuration
        )
    }
}

// Parsed program statistics.
public struct ProgramStatistics {
    public var declarations: Int
    public var statements: Int
    public var functions: Int
    public var variables: Int
    public var types: Int

    public init(program: Program) {
        declarations = program.declarations.count
        statements = program.statements.count

        functions = program.declarations.reduce(0) {
            partialResult, declaration in
            if case .function = declaration {
                return partialResult + 1
            }

            return partialResult
        }

        variables = program.declarations.reduce(0) {
            partialResult, declaration in
            if case .variable = declaration {
                return partialResult + 1
            }

            return partialResult
        }

        types = program.declarations.reduce(0) {
            partialResult, declaration in
            switch declaration {
            case .type,
                 .classType,
                 .structType,
                 .enumType,
                 .template,
                 .protocolType,
                 .extensionType:
                return partialResult + 1

            default:
                return partialResult
            }
        }
    }
}

// Parser result helpers.
public extension ParserResult {
    var statistics: ProgramStatistics {
        ProgramStatistics(program: program)
    }

    var hasErrors: Bool {
        diagnostics.contains {
            $0.severity == .error
        }
    }
}

// Parser declaration helpers.
public extension Declaration {
    var name: String? {
        switch self {
        case .function(let value):
            return value.name

        case .variable(let value):
            return value.name

        case .type(let value):
            return value.name

        case .classType(let value):
            return value.name

        case .structType(let value):
            return value.name

        case .enumType(let value):
            return value.name

        case .template(let value):
            return value.name

        case .protocolType(let value):
            return value.name

        case .extensionType:
            return nil

        case .importDeclaration:
            return nil

        case .exportDeclaration(let value):
            return value.name

        case .moduleDeclaration(let value):
            return value.name
        }
    }
}

// Parser statement helpers.
public extension Statement {
    var isControlFlow: Bool {
        switch self {
        case .ifStatement,
             .guardStatement,
             .matchStatement,
             .whileStatement,
             .forStatement:
            return true

        default:
            return false
        }
    }

    var isReturn: Bool {
        if case .returnStatement = self {
            return true
        }

        return false
    }
}

// Parser expression helpers.
public extension Expression {
    var isLiteral: Bool {
        switch self {
        case .integer,
             .floating,
             .string,
             .character,
             .boolean,
             .nilValue:
            return true

        default:
            return false
        }
    }

    var isBinary: Bool {
        if case .binary = self {
            return true
        }

        return false
    }

    var isCall: Bool {
        if case .call = self {
            return true
        }

        return false
    }
}

// Parser test runner.
public enum ParserTestRunner {
    public static func runAll() -> Bool {
        let tests = [
            "function": ParserSmokeTests.run()
        ]

        for (name, result) in tests {
            print("\(name): \(result ? "passed" : "failed")")
        }

        return tests.values.allSatisfy { $0 }
    }
}

// Parser entry point.
public enum ParserEntryPoint {
    public static func run(
        source: String
    ) -> ParserResult {
        LanguageParser.parse(source)
    }

    public static func validate(
        source: String
    ) -> Bool {
        LanguageParser.parse(source).succeeded
    }
}

// Parser output summary.
public enum ParserSummary {
    public static func make(
        _ result: ParserResult
    ) -> String {
        let statistics = result.statistics

        return [
            "declarations: \(statistics.declarations)",
            "statements: \(statistics.statements)",
            "functions: \(statistics.functions)",
            "variables: \(statistics.variables)",
            "types: \(statistics.types)",
            "diagnostics: \(result.diagnostics.count)"
        ].joined(separator: "\n")
    }
}

// Parser version information.
public enum ParserVersion {
    public static let major = 0
    public static let minor = 1
    public static let patch = 0

    public static var string: String {
        "\(major).\(minor).\(patch)"
    }
}

// Parser feature information.
public enum ParserFeatures {
    public static let functions = true
    public static let variables = true
    public static let classes = true
    public static let structs = true
    public static let enums = true
    public static let templates = true
    public static let protocols = true
    public static let extensions = true
    public static let imports = true
    public static let exports = true
    public static let modules = true
    public static let conditionals = true
    public static let loops = true
    public static let matching = true
    public static let errorHandling = true
    public static let expressions = true
}

// Parser configuration presets.
public enum ParserConfigurations {
    public static let standard = ParserConfiguration()

    public static let declarationsOnly = ParserConfiguration(
        allowTopLevelStatements: false,
        allowTrailingCommas: true,
        allowEmptyBodies: true
    )

    public static let strict = ParserConfiguration(
        allowTopLevelStatements: false,
        allowTrailingCommas: false,
        allowEmptyBodies: false
    )
}

// Parser token boundary.
public struct ParserTokenStream {
    public let tokens: [Token]

    public init(tokens: [Token]) {
        self.tokens = tokens
    }

    public init(source: String) {
        let lexer = Lexer(source: source)
        self.tokens = lexer.tokenize()
    }
}

// Parser source boundary.
public struct ParserSource {
    public let source: String

    public init(_ source: String) {
        self.source = source
    }

    public func parse(
        configuration: ParserConfiguration = ParserConfiguration()
    ) -> ParserResult {
        LanguageParser.parse(
            source,
            configuration: configuration
        )
    }
}

// Parser diagnostics collection.
public final class ParserDiagnosticCollector {
    private var values: [ParserDiagnostic] = []

    public init() {}

    public func append(_ diagnostic: ParserDiagnostic) {
        values.append(diagnostic)
    }

    public func append(contentsOf diagnostics: [ParserDiagnostic]) {
        values.append(contentsOf: diagnostics)
    }

    public func all() -> [ParserDiagnostic] {
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

// Parser declaration collection.
public final class DeclarationBuffer {
    private var values: [Declaration] = []

    public init() {}

    public func append(_ declaration: Declaration) {
        values.append(declaration)
    }

    public func append(contentsOf declarations: [Declaration]) {
        values.append(contentsOf: declarations)
    }

    public func all() -> [Declaration] {
        values
    }

    public var count: Int {
        values.count
    }

    public func clear() {
        values.removeAll(keepingCapacity: true)
    }
}

// Parser statement collection.
public final class StatementBuffer {
    private var values: [Statement] = []

    public init() {}

    public func append(_ statement: Statement) {
        values.append(statement)
    }

    public func append(contentsOf statements: [Statement]) {
        values.append(contentsOf: statements)
    }

    public func all() -> [Statement] {
        values
    }

    public var count: Int {
        values.count
    }

    public func clear() {
        values.removeAll(keepingCapacity: true)
    }
}

// Parser expression collection.
public final class ExpressionBuffer {
    private var values: [Expression] = []

    public init() {}

    public func append(_ expression: Expression) {
        values.append(expression)
    }

    public func append(contentsOf expressions: [Expression]) {
        values.append(contentsOf: expressions)
    }

    public func all() -> [Expression] {
        values
    }

    public var count: Int {
        values.count
    }

    public func clear() {
        values.removeAll(keepingCapacity: true)
    }
}

// Parser type collection.
public final class TypeBuffer {
    private var values: [TypeReference] = []

    public init() {}

    public func append(_ type: TypeReference) {
        values.append(type)
    }

    public func append(contentsOf types: [TypeReference]) {
        values.append(contentsOf: types)
    }

    public func all() -> [TypeReference] {
        values
    }

    public var count: Int {
        values.count
    }

    public func clear() {
        values.removeAll(keepingCapacity: true)
    }
}

// Parser declaration classification.
public enum DeclarationClassifier {
    public static func isType(_ declaration: Declaration) -> Bool {
        switch declaration {
        case .type,
             .classType,
             .structType,
             .enumType,
             .template,
             .protocolType,
             .extensionType:
            return true

        default:
            return false
        }
    }

    public static func isCallable(
        _ declaration: Declaration
    ) -> Bool {
        if case .function = declaration {
            return true
        }

        return false
    }

    public static func isModule(
        _ declaration: Declaration
    ) -> Bool {
        switch declaration {
        case .importDeclaration,
             .exportDeclaration,
             .moduleDeclaration:
            return true

        default:
            return false
        }
    }
}

// Parser statement classification.
public enum StatementClassifier {
    public static func isLoop(_ statement: Statement) -> Bool {
        switch statement {
        case .whileStatement,
             .forStatement:
            return true

        default:
            return false
        }
    }

    public static func isBranch(_ statement: Statement) -> Bool {
        switch statement {
        case .ifStatement,
             .guardStatement,
             .matchStatement:
            return true

        default:
            return false
        }
    }

    public static func isControlTransfer(
        _ statement: Statement
    ) -> Bool {
        switch statement {
        case .returnStatement,
             .breakStatement,
             .continueStatement,
             .throwStatement:
            return true

        default:
            return false
        }
    }
}

// Parser expression classification.
public enum ExpressionClassifier {
    public static func isAssignment(
        _ expression: Expression
    ) -> Bool {
        if case .assignment = expression {
            return true
        }

        return false
    }

    public static func isMember(
        _ expression: Expression
    ) -> Bool {
        if case .member = expression {
            return true
        }

        return false
    }

    public static func isSubscript(
        _ expression: Expression
    ) -> Bool {
        if case .subscriptExpression = expression {
            return true
        }

        return false
    }
}

// Parser module.
public enum ParserModule {
    public static let name = "LanguageParser"
    public static let implementation = "Swift"
    public static let target = "Swift"
    public static let version = ParserVersion.string
}