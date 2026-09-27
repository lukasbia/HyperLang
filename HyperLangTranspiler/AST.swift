// MARK: - Source Tracking

// Represents a specific point in the source code.
public struct SourceLocation: Equatable {
    public let line: Int
    public let column: Int
    public let offset: Int
}

// Represents a range of characters in the source code.
public struct SourceSpan: Equatable {
    public let start: SourceLocation
    public let end: SourceLocation
}

// MARK: - Core AST Protocols

// The base node from which all AST nodes derive.
public protocol ASTNode {
    var span: SourceSpan { get }
}

// Represents an expression that evaluates to a value.
public protocol Expr: ASTNode {
    func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult
}

// Represents a statement that performs an action but yields no value.
public protocol Stmt: ASTNode {
    func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult
}

// Represents a declaration that binds a name to a structure, type, or value.
public protocol Decl: Stmt {
    func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult
}

// Represents a type annotation.
public protocol TypeExpr: ASTNode {
    func accept<V: TypeVisitor>(visitor: V) throws -> V.TypeResult
}

// MARK: - Visitor Interfaces

public protocol ExprVisitor {
    associatedtype ExprResult
    func visit(identifierExpr: IdentifierExpr) throws -> ExprResult
    func visit(literalExpr: LiteralExpr) throws -> ExprResult
    func visit(binaryExpr: BinaryExpr) throws -> ExprResult
    func visit(unaryExpr: UnaryExpr) throws -> ExprResult
    func visit(logicalExpr: LogicalExpr) throws -> ExprResult
    func visit(callExpr: CallExpr) throws -> ExprResult
    func visit(memberAccessExpr: MemberAccessExpr) throws -> ExprResult
    func visit(assignmentExpr: AssignmentExpr) throws -> ExprResult
    func visit(tryExpr: TryExpr) throws -> ExprResult
    func visit(awaitExpr: AwaitExpr) throws -> ExprResult
    func visit(castExpr: CastExpr) throws -> ExprResult
}

public protocol StmtVisitor {
    associatedtype StmtResult
    func visit(expressionStmt: ExpressionStmt) throws -> StmtResult
    func visit(blockStmt: BlockStmt) throws -> StmtResult
    func visit(ifStmt: IfStmt) throws -> StmtResult
    func visit(whileStmt: WhileStmt) throws -> StmtResult
    func visit(loopStmt: LoopStmt) throws -> StmtResult
    func visit(forInStmt: ForInStmt) throws -> StmtResult
    func visit(whenStmt: WhenStmt) throws -> StmtResult
    func visit(returnStmt: ReturnStmt) throws -> StmtResult
    func visit(breakStmt: BreakStmt) throws -> StmtResult
    func visit(continueStmt: ContinueStmt) throws -> StmtResult
    func visit(passStmt: PassStmt) throws -> StmtResult
    func visit(raiseStmt: RaiseStmt) throws -> StmtResult
    func visit(spawnStmt: SpawnStmt) throws -> StmtResult
    func visit(deferStmt: DeferStmt) throws -> StmtResult
    func visit(guardStmt: GuardStmt) throws -> StmtResult
}

public protocol DeclVisitor {
    associatedtype DeclResult
    func visit(moduleDecl: ModuleDecl) throws -> DeclResult
    func visit(includeDecl: IncludeDecl) throws -> DeclResult
    func visit(varDecl: VarDecl) throws -> DeclResult
    func visit(defDecl: DefDecl) throws -> DeclResult
    func visit(classDecl: ClassDecl) throws -> DeclResult
    func visit(structDecl: StructDecl) throws -> DeclResult
    func visit(interfaceDecl: InterfaceDecl) throws -> DeclResult
    func visit(actorDecl: ActorDecl) throws -> DeclResult
    func visit(enumDecl: EnumDecl) throws -> DeclResult
    func visit(initDecl: InitDecl) throws -> DeclResult
    func visit(deinitDecl: DeinitDecl) throws -> DeclResult
}

public protocol TypeVisitor {
    associatedtype TypeResult
    func visit(simpleTypeExpr: SimpleTypeExpr) throws -> TypeResult
    func visit(optionalTypeExpr: OptionalTypeExpr) throws -> TypeResult
}

// MARK: - Expressions

// An identifier referring to a variable, function, or type.
public struct IdentifierExpr: Expr {
    public let name: Token
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(identifierExpr: self)
    }
}

// A constant value such as a string, number, or boolean.
public struct LiteralExpr: Expr {
    public let value: Token
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(literalExpr: self)
    }
}

// An expression consisting of two operands and an operator.
public struct BinaryExpr: Expr {
    public let left: Expr
    public let operatorToken: Token
    public let right: Expr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(binaryExpr: self)
    }
}

// An expression consisting of a single operand and a prefix operator.
public struct UnaryExpr: Expr {
    public let operatorToken: Token
    public let right: Expr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(unaryExpr: self)
    }
}

// A logical operation (&& or ||) with short-circuit evaluation.
public struct LogicalExpr: Expr {
    public let left: Expr
    public let operatorToken: Token
    public let right: Expr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(logicalExpr: self)
    }
}

// A function or method invocation.
public struct CallExpr: Expr {
    public let callee: Expr
    public let paren: Token
    public let arguments: [Expr]
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(callExpr: self)
    }
}

// Access to a property or method on an instance.
public struct MemberAccessExpr: Expr {
    public let object: Expr
    public let name: Token
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(memberAccessExpr: self)
    }
}

// An assignment of a value to a mutable identifier or property.
public struct AssignmentExpr: Expr {
    public let name: Token
    public let value: Expr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(assignmentExpr: self)
    }
}

// An expression that evaluates an operation that may throw an error.
public struct TryExpr: Expr {
    public let tryKeyword: Token
    public let expression: Expr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(tryExpr: self)
    }
}

// An expression that suspends execution until an asynchronous operation completes.
public struct AwaitExpr: Expr {
    public let awaitKeyword: Token
    public let expression: Expr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(awaitExpr: self)
    }
}

// A type cast operation (e.g., `value as String`).
public struct CastExpr: Expr {
    public let expression: Expr
    public let asKeyword: Token
    public let targetType: TypeExpr
    public let span: SourceSpan
    
    public func accept<V: ExprVisitor>(visitor: V) throws -> V.ExprResult {
        return try visitor.visit(castExpr: self)
    }
}

// MARK: - Statements

// A statement consisting solely of an expression.
public struct ExpressionStmt: Stmt {
    public let expression: Expr
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(expressionStmt: self)
    }
}

// A lexical scope enclosing a sequence of statements.
public struct BlockStmt: Stmt {
    public let statements: [Stmt]
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(blockStmt: self)
    }
}

// A conditional branch statement.
public struct IfStmt: Stmt {
    public let condition: Expr
    public let thenBranch: BlockStmt
    public let elseBranch: Stmt?
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(ifStmt: self)
    }
}

// A loop that executes as long as a condition evaluates to true.
public struct WhileStmt: Stmt {
    public let condition: Expr
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(whileStmt: self)
    }
}

// An infinite loop construct.
public struct LoopStmt: Stmt {
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(loopStmt: self)
    }
}

// A sequence iteration loop over an iterable expression.
public struct ForInStmt: Stmt {
    public let iterator: Token
    public let iterable: Expr
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(forInStmt: self)
    }
}

// An advanced multi-branch conditional matching block.
public struct WhenStmt: Stmt {
    public let subject: Expr?
    public let cases: [WhenCase]
    public let defaultCase: BlockStmt?
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(whenStmt: self)
    }
}

// Represents a single pattern-matching branch inside a `when` statement.
public struct WhenCase {
    public let pattern: Expr
    public let body: BlockStmt
}

// Returns control and an optional value to the calling scope.
public struct ReturnStmt: Stmt {
    public let keyword: Token
    public let value: Expr?
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(returnStmt: self)
    }
}

// Terminates the innermost loop or switch statement immediately.
public struct BreakStmt: Stmt {
    public let keyword: Token
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(breakStmt: self)
    }
}

// Skips the remaining execution of the current loop iteration.
public struct ContinueStmt: Stmt {
    public let keyword: Token
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(continueStmt: self)
    }
}

// An explicit null statement or placeholder.
public struct PassStmt: Stmt {
    public let keyword: Token
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(passStmt: self)
    }
}

// Raises a runtime error condition.
public struct RaiseStmt: Stmt {
    public let keyword: Token
    public let value: Expr
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(raiseStmt: self)
    }
}

// Launches a concurrent background task.
public struct SpawnStmt: Stmt {
    public let keyword: Token
    public let call: CallExpr
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(spawnStmt: self)
    }
}

// Schedules a block of code to execute prior to the current scope exiting.
public struct DeferStmt: Stmt {
    public let keyword: Token
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(deferStmt: self)
    }
}

// An early-exit assertion statement.
public struct GuardStmt: Stmt {
    public let condition: Expr
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        return try visitor.visit(guardStmt: self)
    }
}

// MARK: - Declarations

// Declares an explicit compilation boundary.
public struct ModuleDecl: Decl {
    public let name: Token
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(moduleDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        // Fallback for statements visitor mapping
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Brings an external module into the current file scope.
public struct IncludeDecl: Decl {
    public let modulePath: Token
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(includeDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares a value or variable (`val` or `var`).
public struct VarDecl: Decl {
    public let keyword: Token // val or var
    public let name: Token
    public let typeAnnotation: TypeExpr?
    public let initializer: Expr?
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(varDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Represents a parameter in a function or initialization signature.
public struct Parameter {
    public let inoutModifier: Token?
    public let name: Token
    public let type: TypeExpr
}

// Declares a function or method.
public struct DefDecl: Decl {
    public let modifiers: [Token]
    public let name: Token
    public let parameters: [Parameter]
    public let returnType: TypeExpr?
    public let isAsync: Bool
    public let isThrows: Bool
    public let body: BlockStmt?
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(defDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares an ARC-managed reference type container.
public struct ClassDecl: Decl {
    public let modifiers: [Token]
    public let name: Token
    public let superclass: TypeExpr?
    public let members: [Decl]
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(classDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares a value-type structure.
public struct StructDecl: Decl {
    public let modifiers: [Token]
    public let name: Token
    public let members: [Decl]
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(structDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares a behavioral contract interface.
public struct InterfaceDecl: Decl {
    public let modifiers: [Token]
    public let name: Token
    public let members: [Decl]
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(interfaceDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares a thread-safe concurrent state container.
public struct ActorDecl: Decl {
    public let modifiers: [Token]
    public let name: Token
    public let members: [Decl]
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(actorDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares a value enumeration.
public struct EnumDecl: Decl {
    public let modifiers: [Token]
    public let name: Token
    public let cases: [EnumCase]
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(enumDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Represents a branch/case within an enumeration.
public struct EnumCase {
    public let name: Token
    public let associatedTypes: [TypeExpr]?
}

// Declares an instance constructor block.
public struct InitDecl: Decl {
    public let modifiers: [Token]
    public let parameters: [Parameter]
    public let isThrows: Bool
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(initDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// Declares an ARC deallocation cleanup hook.
public struct DeinitDecl: Decl {
    public let body: BlockStmt
    public let span: SourceSpan
    
    public func accept<V: DeclVisitor>(visitor: V) throws -> V.DeclResult {
        return try visitor.visit(deinitDecl: self)
    }
    
    public func accept<V: StmtVisitor>(visitor: V) throws -> V.StmtResult {
        fatalError("Decl must be visited using DeclVisitor")
    }
}

// MARK: - Types

// A standard, unadorned type identifier.
public struct SimpleTypeExpr: TypeExpr {
    public let name: Token
    public let span: SourceSpan
    
    public func accept<V: TypeVisitor>(visitor: V) throws -> V.TypeResult {
        return try visitor.visit(simpleTypeExpr: self)
    }
}

// A type that allows nullability (`Type?`).
public struct OptionalTypeExpr: TypeExpr {
    public let baseType: TypeExpr
    public let questionMark: Token
    public let span: SourceSpan
    
    public func accept<V: TypeVisitor>(visitor: V) throws -> V.TypeResult {
        return try visitor.visit(optionalTypeExpr: self)
    }
}
