import Foundation

// MARK: - Semantic Errors

public enum SemanticError: Error, CustomStringConvertible {
    case duplicateSymbol(Token, String)
    case unresolvedSymbol(Token, String)
    case typeMismatch(Token, String)
    case invalidInclude(Token, String)
    
    public var description: String {
        switch self {
        case .duplicateSymbol(let token, let name):
            return "[Semantic Error at \(token.span.start)] Symbol '\(name)' is already defined."
        case .unresolvedSymbol(let token, let name):
            return "[Semantic Error at \(token.span.start)] Use of unresolved symbol '\(name)'."
        case .typeMismatch(let token, let message):
            return "[Semantic Error at \(token.span.start)] Type mismatch: \(message)"
        case .invalidInclude(let token, let module):
            return "[Semantic Error at \(token.span.start)] Could not resolve included module '\(module)'."
        }
    }
}

// MARK: - Symbol Table Scope

public final class Scope {
    private var symbols: [String: Bool] = [:]
    public let parent: Scope?
    
    public init(parent: Scope? = nil) {
        self.parent = parent
    }
    
    public func define(name: String) -> Bool {
        if symbols[name] != nil { return false }
        symbols[name] = true
        return true
    }
    
    public func resolve(name: String) -> Bool {
        if symbols[name] != nil { return true }
        return parent?.resolve(name: name) ?? false
    }
}

// MARK: - Semantic Analyzer

public final class SemanticAnalyzer: DeclVisitor, StmtVisitor, ExprVisitor, TypeVisitor {
    
    public typealias DeclResult = Void
    public typealias StmtResult = Void
    public typealias ExprResult = Void
    public typealias TypeResult = Void
    
    private var currentScope = Scope()
    private var errors: [SemanticError] = []
    private var importedModules: Set<String> = []
    
    public init() {}
    
    public func analyze(declarations: [Decl]) throws -> [SemanticError] {
        errors.removeAll()
        for decl in declarations {
            try decl.accept(visitor: self)
        }
        return errors
    }
    
    private func beginScope() {
        currentScope = Scope(parent: currentScope)
    }
    
    private func endScope() {
        if let parent = currentScope.parent {
            currentScope = parent
        }
    }
    
    // MARK: - DeclVisitor
    
    public func visit(moduleDecl: ModuleDecl) throws {
        let name = moduleDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(moduleDecl.name, name))
        }
    }
    
    public func visit(includeDecl: IncludeDecl) throws {
        let moduleName = includeDecl.modulePath.lexeme
        // Register or validate the included module path
        importedModules.insert(moduleName)
    }
    
    public func visit(varDecl: VarDecl) throws {
        let name = varDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(varDecl.name, name))
        }
        if let initializer = varDecl.initializer {
            try initializer.accept(visitor: self)
        }
        if let typeAnnotation = varDecl.typeAnnotation {
            try typeAnnotation.accept(visitor: self)
        }
    }
    
    public func visit(defDecl: DefDecl) throws {
        let name = defDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(defDecl.name, name))
        }
        
        beginScope()
        for param in defDecl.parameters {
            _ = currentScope.define(name: param.name.lexeme)
            try param.type.accept(visitor: self)
        }
        
        if let returnType = defDecl.returnType {
            try returnType.accept(visitor: self)
        }
        
        if let body = defDecl.body {
            try body.accept(visitor: self)
        }
        endScope()
    }
    
    public func visit(classDecl: ClassDecl) throws {
        let name = classDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(classDecl.name, name))
        }
        
        beginScope()
        if let superclass = classDecl.superclass {
            try superclass.accept(visitor: self)
        }
        for member in classDecl.members {
            try member.accept(visitor: self)
        }
        endScope()
    }
    
    public func visit(structDecl: StructDecl) throws {
        let name = structDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(structDecl.name, name))
        }
        
        beginScope()
        for member in structDecl.members {
            try member.accept(visitor: self)
        }
        endScope()
    }
    
    public func visit(interfaceDecl: InterfaceDecl) throws {
        let name = interfaceDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(interfaceDecl.name, name))
        }
        
        beginScope()
        for member in interfaceDecl.members {
            try member.accept(visitor: self)
        }
        endScope()
    }
    
    public func visit(actorDecl: ActorDecl) throws {
        let name = actorDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(actorDecl.name, name))
        }
        
        beginScope()
        for member in actorDecl.members {
            try member.accept(visitor: self)
        }
        endScope()
    }
    
    public func visit(enumDecl: EnumDecl) throws {
        let name = enumDecl.name.lexeme
        if !currentScope.define(name: name) {
            errors.append(.duplicateSymbol(enumDecl.name, name))
        }
    }
    
    public func visit(initDecl: InitDecl) throws {
        beginScope()
        for param in initDecl.parameters {
            _ = currentScope.define(name: param.name.lexeme)
            try param.type.accept(visitor: self)
        }
        try initDecl.body.accept(visitor: self)
        endScope()
    }
    
    public func visit(deinitDecl: DeinitDecl) throws {
        beginScope()
        try deinitDecl.body.accept(visitor: self)
        endScope()
    }
    
    // MARK: - StmtVisitor
    
    public func visit(expressionStmt: ExpressionStmt) throws {
        try expressionStmt.expression.accept(visitor: self)
    }
    
    public func visit(blockStmt: BlockStmt) throws {
        beginScope()
        for stmt in blockStmt.statements {
            try stmt.accept(visitor: self)
        }
        endScope()
    }
    
    public func visit(ifStmt: IfStmt) throws {
        try ifStmt.condition.accept(visitor: self)
        try ifStmt.thenBranch.accept(visitor: self)
        if let elseBranch = ifStmt.elseBranch {
            try elseBranch.accept(visitor: self)
        }
    }
    
    public func visit(whileStmt: WhileStmt) throws {
        try whileStmt.condition.accept(visitor: self)
        try whileStmt.body.accept(visitor: self)
    }
    
    public func visit(loopStmt: LoopStmt) throws {
        try loopStmt.body.accept(visitor: self)
    }
    
    public func visit(forInStmt: ForInStmt) throws {
        beginScope()
        _ = currentScope.define(name: forInStmt.iterator.lexeme)
        try forInStmt.iterable.accept(visitor: self)
        try forInStmt.body.accept(visitor: self)
        endScope()
    }
    
    public func visit(whenStmt: WhenStmt) throws {
        if let subject = whenStmt.subject {
            try subject.accept(visitor: self)
        }
        for c in whenStmt.cases {
            try c.pattern.accept(visitor: self)
            try c.body.accept(visitor: self)
        }
        if let defaultCase = whenStmt.defaultCase {
            try defaultCase.accept(visitor: self)
        }
    }
    
    public func visit(returnStmt: ReturnStmt) throws {
        if let value = returnStmt.value {
            try value.accept(visitor: self)
        }
    }
    
    public func visit(breakStmt: BreakStmt) throws {}
    public func visit(continueStmt: ContinueStmt) throws {}
    public func visit(passStmt: PassStmt) throws {}
    
    public func visit(raiseStmt: RaiseStmt) throws {
        try raiseStmt.value.accept(visitor: self)
    }
    
    public func visit(spawnStmt: SpawnStmt) throws {
        try spawnStmt.call.accept(visitor: self)
    }
    
    public func visit(deferStmt: DeferStmt) throws {
        try deferStmt.body.accept(visitor: self)
    }
    
    public func visit(guardStmt: GuardStmt) throws {
        try guardStmt.condition.accept(visitor: self)
        try guardStmt.body.accept(visitor: self)
    }
    
    // MARK: - ExprVisitor
    
    public func visit(identifierExpr: IdentifierExpr) throws {
        let name = identifierExpr.name.lexeme
        // Allow self/super or check symbol table
        if name != "self" && name != "super" && !currentScope.resolve(name: name) {
            errors.append(.unresolvedSymbol(identifierExpr.name, name))
        }
    }
    
    public func visit(literalExpr: LiteralExpr) throws {}
    
    public func visit(binaryExpr: BinaryExpr) throws {
        try binaryExpr.left.accept(visitor: self)
        try binaryExpr.right.accept(visitor: self)
    }
    
    public func visit(unaryExpr: UnaryExpr) throws {
        try unaryExpr.right.accept(visitor: self)
    }
    
    public func visit(logicalExpr: LogicalExpr) throws {
        try logicalExpr.left.accept(visitor: self)
        try logicalExpr.right.accept(visitor: self)
    }
    
    public func visit(callExpr: CallExpr) throws {
        try callExpr.callee.accept(visitor: self)
        for arg in callExpr.arguments {
            try arg.accept(visitor: self)
        }
    }
    
    public func visit(memberAccessExpr: MemberAccessExpr) throws {
        try memberAccessExpr.object.accept(visitor: self)
    }
    
    public func visit(assignmentExpr: AssignmentExpr) throws {
        let name = assignmentExpr.name.lexeme
        if !currentScope.resolve(name: name) {
            errors.append(.unresolvedSymbol(assignmentExpr.name, name))
        }
        try assignmentExpr.value.accept(visitor: self)
    }
    
    public func visit(tryExpr: TryExpr) throws {
        try spectroscopic(tryExpr.expression)
    }
    
    private func spectroscopic(_ expr: Expr) throws {
        try expr.accept(visitor: self)
    }
    
    public func visit(awaitExpr: AwaitExpr) throws {
        try awaitExpr.expression.accept(visitor: self)
    }
    
    public func visit(castExpr: CastExpr) throws {
        try castExpr.expression.accept(visitor: self)
        try castExpr.targetType.accept(visitor: self)
    }
    
    // MARK: - TypeVisitor
    
    public func visit(simpleTypeExpr: SimpleTypeExpr) throws {
        let name = simpleTypeExpr.name.lexeme
        // Basic primitive types bypass or check table
        let primitives = ["Int", "Float", "Double", "String", "Bool", "Void", "Any"]
        if !primitives.contains(name) && !currentScope.resolve(name: name) {
            errors.append(.unresolvedSymbol(simpleTypeExpr.name, name))
        }
    }
    
    public func visit(optionalTypeExpr: OptionalTypeExpr) throws {
        try optionalTypeExpr.baseType.accept(visitor: self)
    }
}
