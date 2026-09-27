// MARK: - Parser Errors

public enum ParserError: Error, CustomStringConvertible {
    case unexpectedToken(Token, String)
    case invalidAssignmentTarget(Token)
    case missingExpression(Token)
    case tooManyArguments(Token)
    
    public var description: String {
        switch self {
        case .unexpectedToken(let token, let message):
            return "[Error at \(token.span.start)] \(message). Found '\(token.lexeme)'"
        case .invalidAssignmentTarget(let token):
            return "[Error at \(token.span.start)] Invalid assignment target."
        case .missingExpression(let token):
            return "[Error at \(token.span.start)] Expected expression."
        case .tooManyArguments(let token):
            return "[Error at \(token.span.start)] Cannot have more than 255 arguments."
        }
    }
}

// MARK: - Parser Core

public final class Parser {
    private let tokens: [Token]
    private var current: Int = 0
    private var errors: [ParserError] = []
    
    public init(tokens: [Token]) {
        self.tokens = tokens
    }
    
    public func parse() -> [Decl] {
        var declarations: [Decl] = []
        while !isAtEnd() {
            if let decl = declaration() {
                declarations.append(decl)
            }
        }
        return declarations
    }
    
    // MARK: - Declarations
    
    private func declaration() -> Decl? {
        do {
            if match(.keywordModule) { return try moduleDeclaration() }
            if match(.keywordInclude) { return try includeDeclaration() }
            
            let modifiers = parseModifiers()
            
            if match(.keywordClass) { return try classDeclaration(modifiers: modifiers) }
            if match(.keywordStruct) { return try structDeclaration(modifiers: modifiers) }
            if match(.keywordInterface) { return try interfaceDeclaration(modifiers: modifiers) }
            if match(.keywordActor) { return try actorDeclaration(modifiers: modifiers) }
            if match(.keywordEnum) { return try enumDeclaration(modifiers: modifiers) }
            if match(.keywordDef) { return try functionDeclaration(modifiers: modifiers) }
            if match(.keywordInit) { return try initDeclaration(modifiers: modifiers) }
            if match(.keywordDeinit) { return try deinitDeclaration() }
            if match(.keywordVal, .keywordVar) { return try varDeclaration(modifiers: modifiers) }
            
            return try statementAsDecl()
        } catch {
            synchronize()
            return nil
        }
    }
    
    private func parseModifiers() -> [Token] {
        var modifiers: [Token] = []
        while check(.keywordPublic) || check(.keywordPrivate) ||
              check(.keywordMut) || check(.keywordOverride) ||
              check(.keywordStatic) || check(.keywordWeak) ||
              check(.keywordUnowned) {
            modifiers.append(advance())
        }
        return modifiers
    }
    
    private func moduleDeclaration() throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected module name.")
        return ModuleDecl(name: name, span: expandSpan(keyword, name))
    }
    
    private func includeDeclaration() throws -> Decl {
        let keyword = previous()
        // Safely consume any identifier token for the module path
        guard case .identifier = peek().type else {
            throw error(peek(), "Expected module identifier to include.")
        }
        let path = advance()
        return IncludeDecl(modulePath: path, span: expandSpan(keyword, path))
    }
    
    private func classDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected class name.")
        
        var superclass: TypeExpr? = nil
        if match(.colon) {
            superclass = try parseType()
        }
        
        try consume(.leftBrace, "Expected '{' before class body.")
        var members: [Decl] = []
        while !check(.rightBrace) && !isAtEnd() {
            if let member = declaration() {
                members.append(member)
            }
        }
        let endBrace = try consume(.rightBrace, "Expected '}' after class body.")
        
        return ClassDecl(modifiers: modifiers, name: name, superclass: superclass, members: members, span: expandSpan(keyword, endBrace))
    }
    
    private func structDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected struct name.")
        try consume(.leftBrace, "Expected '{' before struct body.")
        var members: [Decl] = []
        while !check(.rightBrace) && !isAtEnd() {
            if let member = declaration() {
                members.append(member)
            }
        }
        let endBrace = try consume(.rightBrace, "Expected '}' after struct body.")
        return StructDecl(modifiers: modifiers, name: name, members: members, span: expandSpan(keyword, endBrace))
    }
    
    private func interfaceDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected interface name.")
        try consume(.leftBrace, "Expected '{' before interface body.")
        var members: [Decl] = []
        while !check(.rightBrace) && !isAtEnd() {
            if let member = declaration() {
                members.append(member)
            }
        }
        let endBrace = try consume(.rightBrace, "Expected '}' after interface body.")
        return InterfaceDecl(modifiers: modifiers, name: name, members: members, span: expandSpan(keyword, endBrace))
    }
    
    private func actorDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected actor name.")
        try consume(.leftBrace, "Expected '{' before actor body.")
        var members: [Decl] = []
        while !check(.rightBrace) && !isAtEnd() {
            if let member = declaration() {
                members.append(member)
            }
        }
        let endBrace = try consume(.rightBrace, "Expected '}' after actor body.")
        return ActorDecl(modifiers: modifiers, name: name, members: members, span: expandSpan(keyword, endBrace))
    }
    
    private func enumDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected enum name.")
        try consume(.leftBrace, "Expected '{' before enum body.")
        
        var cases: [EnumCase] = []
        while !check(.rightBrace) && !isAtEnd() {
            try consume(.keywordCase, "Expected 'case' inside enum.")
            let caseName = try consume(.identifier(""), "Expected enum case name.")
            
            var associated: [TypeExpr]? = nil
            if match(.leftParen) {
                associated = []
                if !check(.rightParen) {
                    repeat {
                        associated?.append(try parseType())
                    } while match(.comma)
                }
                try consume(.rightParen, "Expected ')' after associated types.")
            }
            cases.append(EnumCase(name: caseName, associatedTypes: associated))
        }
        
        let endBrace = try consume(.rightBrace, "Expected '}' after enum body.")
        return EnumDecl(modifiers: modifiers, name: name, cases: cases, span: expandSpan(keyword, endBrace))
    }
    
    private func functionDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected function name.")
        let (parameters, isAsync, isThrows) = try parseSignature()
        
        var returnType: TypeExpr? = nil
        if match(.arrow) {
            returnType = try parseType()
        }
        
        var body: BlockStmt? = nil
        var endToken = name
        if match(.leftBrace) {
            body = try blockStatement()
            endToken = body!.span.end > endToken.span.end ? Token(type: .rightBrace, lexeme: "}", span: SourceSpan(start: body!.span.end, end: body!.span.end)) : name
        } else if check(.keywordPass) {
            let passTok = advance()
            endToken = passTok
            body = BlockStmt(statements: [PassStmt(keyword: passTok, span: passTok.span)], span: passTok.span)
        }
        
        return DefDecl(modifiers: modifiers, name: name, parameters: parameters, returnType: returnType, isAsync: isAsync, isThrows: isThrows, body: body, span: expandSpan(keyword, endToken))
    }
    
    private func varDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let name = try consume(.identifier(""), "Expected variable name.")
        
        var typeAnnotation: TypeExpr? = nil
        if match(.colon) {
            typeAnnotation = try parseType()
        }
        
        var initializer: Expr? = nil
        if match(.equal) {
            initializer = try expression()
        }
        
        return VarDecl(keyword: keyword, name: name, typeAnnotation: typeAnnotation, initializer: initializer, span: expandSpan(keyword, previous()))
    }
    
    private func initDeclaration(modifiers: [Token]) throws -> Decl {
        let keyword = previous()
        let (parameters, _, isThrows) = try parseSignature()
        try consume(.leftBrace, "Expected '{' before init body.")
        let body = try blockStatement()
        return InitDecl(modifiers: modifiers, parameters: parameters, isThrows: isThrows, body: body, span: expandSpan(keyword, previous()))
    }
    
    private func deinitDeclaration() throws -> Decl {
        let keyword = previous()
        try consume(.leftBrace, "Expected '{' before deinit body.")
        let body = try blockStatement()
        return DeinitDecl(body: body, span: expandSpan(keyword, previous()))
    }
    
    // MARK: - Statements
    
    private func statementAsDecl() throws -> Decl {
        let stmt = try statement()
        struct StmtDeclWrapper: Decl {
            let stmt: Stmt
            var span: SourceSpan { stmt.span }
        }
        return StmtDeclWrapper(stmt: stmt)
    }
    
    private func statement() throws -> Stmt {
        if match(.keywordIf) { return try ifStatement() }
        if match(.keywordWhile) { return try whileStatement() }
        if match(.keywordLoop) { return try loopStatement() }
        if match(.keywordFor) { return try forStatement() }
        if match(.keywordWhen) { return try whenStatement() }
        if match(.keywordReturn) { return try returnStatement() }
        if match(.keywordBreak) { return try breakStatement() }
        if match(.keywordContinue) { return try continueStatement() }
        if match(.keywordPass) { return try passStatement() }
        if match(.keywordRaise) { return try raiseStatement() }
        if match(.keywordSpawn) { return try spawnStatement() }
        if match(.keywordDefer) { return try deferStatement() }
        if match(.keywordGuard) { return try guardStatement() }
        if match(.leftBrace) { return try blockStatement() }
        
        return try expressionStatement()
    }
    
    private func ifStatement() throws -> Stmt {
        let keyword = previous()
        let condition = try expression()
        try consume(.leftBrace, "Expected '{' after if condition.")
        let thenBranch = try blockStatement()
        
        var elseBranch: Stmt? = nil
        if match(.keywordElse) {
            if match(.leftBrace) {
                elseBranch = try blockStatement()
            } else if match(.keywordIf) {
                elseBranch = try ifStatement()
            } else {
                throw error(peek(), "Expected '{' or 'if' after else.")
            }
        }
        
        let endSpan = elseBranch?.span ?? thenBranch.span
        return IfStmt(condition: condition, thenBranch: thenBranch, elseBranch: elseBranch, span: expandSpan(keyword, endSpan))
    }
    
    private func whileStatement() throws -> Stmt {
        let keyword = previous()
        let condition = try expression()
        try consume(.leftBrace, "Expected '{' after while condition.")
        let body = try blockStatement()
        return WhileStmt(condition: condition, body: body, span: expandSpan(keyword, body.span))
    }
    
    private func loopStatement() throws -> Stmt {
        let keyword = previous()
        try consume(.leftBrace, "Expected '{' after loop.")
        let body = try blockStatement()
        return LoopStmt(body: body, span: expandSpan(keyword, body.span))
    }
    
    private func forStatement() throws -> Stmt {
        let keyword = previous()
        let iterator = try consume(.identifier(""), "Expected iterator name.")
        try consume(.keywordIn, "Expected 'in' after for iterator.")
        let iterable = try expression()
        try consume(.leftBrace, "Expected '{' after for iterable.")
        let body = try blockStatement()
        return ForInStmt(iterator: iterator, iterable: iterable, body: body, span: expandSpan(keyword, body.span))
    }
    
    private func whenStatement() throws -> Stmt {
        let keyword = previous()
        
        var subject: Expr? = nil
        if !check(.leftBrace) {
            subject = try expression()
        }
        
        try consume(.leftBrace, "Expected '{' after when subject.")
        var cases: [WhenCase] = []
        var defaultCase: BlockStmt? = nil
        
        while !check(.rightBrace) && !isAtEnd() {
            if match(.keywordCase) {
                let pattern = try expression()
                try consume(.colon, "Expected ':' after case pattern.")
                let body = try match(.leftBrace) ? blockStatement() : singleStatementBlock()
                cases.append(WhenCase(pattern: pattern, body: body))
            } else if match(.keywordDefault) {
                try consume(.colon, "Expected ':' after default.")
                defaultCase = try match(.leftBrace) ? blockStatement() : singleStatementBlock()
            } else {
                throw error(peek(), "Expected 'case' or 'default' inside when block.")
            }
        }
        
        let endBrace = try consume(.rightBrace, "Expected '}' after when block.")
        return WhenStmt(subject: subject, cases: cases, defaultCase: defaultCase, span: expandSpan(keyword, endBrace))
    }
    
    private func returnStatement() throws -> Stmt {
        let keyword = previous()
        var value: Expr? = nil
        if !check(.rightBrace) && !check(.keywordCase) && !check(.keywordDefault) {
            value = try expression()
        }
        return ReturnStmt(keyword: keyword, value: value, span: expandSpan(keyword, previous()))
    }
    
    private func breakStatement() throws -> Stmt {
        let keyword = previous()
        return BreakStmt(keyword: keyword, span: keyword.span)
    }
    
    private func continueStatement() throws -> Stmt {
        let keyword = previous()
        return ContinueStmt(keyword: keyword, span: keyword.span)
    }
    
    private func passStatement() throws -> Stmt {
        let keyword = previous()
        return PassStmt(keyword: keyword, span: keyword.span)
    }
    
    private func raiseStatement() throws -> Stmt {
        let keyword = previous()
        let value = try expression()
        return RaiseStmt(keyword: keyword, value: value, span: expandSpan(keyword, previous()))
    }
    
    private func spawnStatement() throws -> Stmt {
        let keyword = previous()
        let expr = try expression()
        guard let callExpr = expr as? CallExpr else {
            throw error(previous(), "Expected function call after 'spawn'.")
        }
        return SpawnStmt(keyword: keyword, call: callExpr, span: expandSpan(keyword, previous()))
    }
    
    private func deferStatement() throws -> Stmt {
        let keyword = previous()
        try consume(.leftBrace, "Expected '{' after defer.")
        let body = try blockStatement()
        return DeferStmt(keyword: keyword, body: body, span: expandSpan(keyword, body.span))
    }
    
    private func guardStatement() throws -> Stmt {
        let keyword = previous()
        let condition = try expression()
        try consume(.keywordElse, "Expected 'else' after guard condition.")
        try consume(.leftBrace, "Expected '{' after guard else.")
        let body = try blockStatement()
        return GuardStmt(condition: condition, body: body, span: expandSpan(keyword, body.span))
    }
    
    private func blockStatement() throws -> BlockStmt {
        let startToken = previous()
        var statements: [Stmt] = []
        
        while !check(.rightBrace) && !isAtEnd() {
            if let decl = declaration() {
                statements.append(decl)
            }
        }
        
        let endBrace = try consume(.rightBrace, "Expected '}' after block.")
        return BlockStmt(statements: statements, span: expandSpan(startToken, endBrace))
    }
    
    private func singleStatementBlock() throws -> BlockStmt {
        let stmt = try statement()
        return BlockStmt(statements: [stmt], span: stmt.span)
    }
    
    private func expressionStatement() throws -> Stmt {
        let expr = try expression()
        return ExpressionStmt(expression: expr, span: expr.span)
    }
    
    // MARK: - Expressions
    
    private func expression() throws -> Expr {
        return try assignment()
    }
    
    private func assignment() throws -> Expr {
        let expr = try logicalOr()
        
        if match(.equal) {
            let equals = previous()
            let value = try assignment()
            
            if let varExpr = expr as? IdentifierExpr {
                return AssignmentExpr(name: varExpr.name, value: value, span: expandSpan(expr.span, value.span))
            } else if let getExpr = expr as? MemberAccessExpr {
                return AssignmentExpr(name: getExpr.name, value: value, span: expandSpan(expr.span, value.span))
            }
            throw error(equals, "Invalid assignment target.")
        }
        return expr
    }
    
    private func logicalOr() throws -> Expr {
        var expr = try logicalAnd()
        while match(.pipePipe) {
            let op = previous()
            let right = try logicalAnd()
            expr = LogicalExpr(left: expr, operatorToken: op, right: right, span: expandSpan(expr.span, right.span))
        }
        return expr
    }
    
    private func logicalAnd() throws -> Expr {
        var expr = try equality()
        while match(.ampersandAmpersand) {
            let op = previous()
            let right = try equality()
            expr = LogicalExpr(left: expr, operatorToken: op, right: right, span: expandSpan(expr.span, right.span))
        }
        return expr
    }
    
    private func equality() throws -> Expr {
        var expr = try comparison()
        while match(.equalEqual, .bangEqual) {
            let op = previous()
            let right = try comparison()
            expr = BinaryExpr(left: expr, operatorToken: op, right: right, span: expandSpan(expr.span, right.span))
        }
        return expr
    }
    
    private func comparison() throws -> Expr {
        var expr = try term()
        while match(.greater, .greaterEqual, .less, .lessEqual) {
            let op = previous()
            let right = try term()
            expr = BinaryExpr(left: expr, operatorToken: op, right: right, span: expandSpan(expr.span, right.span))
        }
        
        if match(.keywordIs) {
            let isTok = previous()
            let typeNode = try parseType()
            expr = CastExpr(expression: expr, asKeyword: isTok, targetType: typeNode, span: expandSpan(expr.span, previous().span))
        }
        return expr
    }
    
    private func term() throws -> Expr {
        var expr = try factor()
        while match(.minus, .plus) {
            let op = previous()
            let right = try factor()
            expr = BinaryExpr(left: expr, operatorToken: op, right: right, span: expandSpan(expr.span, right.span))
        }
        return expr
    }
    
    private func factor() throws -> Expr {
        var expr = try unary()
        while match(.slash, .star, .percent) {
            let op = previous()
            let right = try unary()
            expr = BinaryExpr(left: expr, operatorToken: op, right: right, span: expandSpan(expr.span, right.span))
        }
        return expr
    }
    
    private func unary() throws -> Expr {
        if match(.bang, .minus, .keywordTry, .keywordAwait) {
            let op = previous()
            let right = try unary()
            
            if op.type == .keywordTry {
                return TryExpr(tryKeyword: op, expression: right, span: expandSpan(op.span, right.span))
            }
            if op.type == .keywordAwait {
                return AwaitExpr(awaitKeyword: op, expression: right, span: expandSpan(op.span, right.span))
            }
            
            return UnaryExpr(operatorToken: op, right: right, span: expandSpan(op.span, right.span))
        }
        return try call()
    }
    
    private func call() throws -> Expr {
        var expr = try primary()
        
        while true {
            if match(.leftParen) {
                expr = try finishCall(callee: expr)
            } else if match(.dot) {
                let name = try consume(.identifier(""), "Expected property name after '.'.")
                expr = MemberAccessExpr(object: expr, name: name, span: expandSpan(expr.span, name.span))
            } else if match(.keywordAs) {
                let asKeyword = previous()
                let targetType = try parseType()
                expr = CastExpr(expression: expr, asKeyword: asKeyword, targetType: targetType, span: expandSpan(expr.span, previous().span))
            } else {
                break
            }
        }
        return expr
    }
    
    private func finishCall(callee: Expr) throws -> Expr {
        var arguments: [Expr] = []
        if !check(.rightParen) {
            repeat {
                if arguments.count >= 255 {
                    _ = error(peek(), "Cannot have more than 255 arguments.")
                }
                arguments.append(try expression())
            } while match(.comma)
        }
        let paren = try consume(.rightParen, "Expected ')' after arguments.")
        return CallExpr(callee: callee, paren: paren, arguments: arguments, span: expandSpan(callee.span, paren.span))
    }
    
    private func primary() throws -> Expr {
        if match(.keywordFalse) { return LiteralExpr(value: previous(), span: previous().span) }
        if match(.keywordTrue) { return LiteralExpr(value: previous(), span: previous().span) }
        if match(.keywordNone) { return LiteralExpr(value: previous(), span: previous().span) }
        
        if match(.integerLiteral(0), .floatLiteral(0.0), .stringLiteral("")) {
            return LiteralExpr(value: previous(), span: previous().span)
        }
        
        if match(.keywordSelfVal, .keywordSelfType, .keywordSuper) {
            return IdentifierExpr(name: previous(), span: previous().span)
        }
        
        if match(.identifier("")) {
            return IdentifierExpr(name: previous(), span: previous().span)
        }
        
        if match(.leftParen) {
            let expr = try expression()
            try consume(.rightParen, "Expected ')' after expression.")
            return expr
        }
        
        throw error(peek(), "Expected expression.")
    }
    
    // MARK: - Helpers
    
    private func parseType() throws -> TypeExpr {
        let name = try consume(.identifier(""), "Expected type name.")
        var type: TypeExpr = SimpleTypeExpr(name: name, span: name.span)
        
        while match(.questionMark) {
            let question = previous()
            type = OptionalTypeExpr(baseType: type, questionMark: question, span: expandSpan(type.span, question.span))
        }
        return type
    }
    
    private func parseSignature() throws -> (parameters: [Parameter], isAsync: Bool, isThrows: Bool) {
        try consume(.leftParen, "Expected '(' after name.")
        var parameters: [Parameter] = []
        if !check(.rightParen) {
            repeat {
                let inoutMod = match(.keywordInOut) ? previous() : nil
                let paramName = try consume(.identifier(""), "Expected parameter name.")
                try consume(.colon, "Expected ':' after parameter name.")
                let type = try parseType()
                parameters.append(Parameter(inoutModifier: inoutMod, name: paramName, type: type))
            } while match(.comma)
        }
        try consume(.rightParen, "Expected ')' after parameters.")
        
        let isAsync = match(.keywordAsync)
        let isThrows = match(.keywordThrows)
        
        return (parameters, isAsync, isThrows)
    }
    
    private func match(_ types: TokenType...) -> Bool {
        for type in types {
            if check(type) {
                _ = advance()
                return true
            }
        }
        return false
    }
    
    private func check(_ type: TokenType) -> Bool {
        if isAtEnd() { return false }
        
        switch (peek().type, type) {
        case (.identifier, .identifier): return true
        case (.stringLiteral, .stringLiteral): return true
        case (.integerLiteral, .integerLiteral): return true
        case (.floatLiteral, .floatLiteral): return true
        default: return peek().type == type
        }
    }
    
    private func advance() -> Token {
        if !isAtEnd() { current += 1 }
        return previous()
    }
    
    private func isAtEnd() -> Bool {
        return peek().type == .eof
    }
    
    private func peek() -> Token {
        return tokens[current]
    }
    
    private func previous() -> Token {
        return tokens[current - 1]
    }
    
    private func consume(_ type: TokenType, _ message: String) throws -> Token {
        if check(type) { return advance() }
        throw error(peek(), message)
    }
    
    private func error(_ token: Token, _ message: String) -> ParserError {
        let err = ParserError.unexpectedToken(token, message)
        errors.append(err)
        return err
    }
    
    private func synchronize() {
        _ = advance()
        
        while !isAtEnd() {
            if previous().type == .semicolon { return }
            
            switch peek().type {
            case .keywordClass, .keywordStruct, .keywordDef, .keywordVar,
                 .keywordVal, .keywordFor, .keywordIf, .keywordWhile,
                 .keywordLoop, .keywordReturn, .keywordActor, .keywordInterface:
                return
            default:
                _ = advance()
            }
        }
    }
    
    private func expandSpan(_ start: SourceSpan, _ end: SourceSpan) -> SourceSpan {
        return SourceSpan(start: start.start, end: end.end)
    }
    
    private func expandSpan(_ startToken: Token, _ endToken: Token) -> SourceSpan {
        return expandSpan(startToken.span, endToken.span)
    }
    
    private func expandSpan(_ startToken: Token, _ endSpan: SourceSpan) -> SourceSpan {
        return expandSpan(startToken.span, endSpan)
    }
}
