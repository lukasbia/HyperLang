// MARK: - Code Generation Target Configuration

public struct TranspilerConfig {
    public var targetLanguage: TargetLanguage
    public var indentSize: Int = 4
    
    public enum TargetLanguage {
        case c
        case cpp
        case swift
        // Python, etc. can be added here
    }
}

// MARK: - Transpiler Core

public final class Transpiler {
    private let declarations: [Decl]
    private let config: TranspilerConfig
    
    private var output: String = ""
    private var indentLevel: Int = 0
    
    public init(ast: [Decl], config: TranspilerConfig = TranspilerConfig(targetLanguage: .swift)) {
        self.declarations = ast
        self.config = config
    }
    
    public func generate() -> String {
        output = ""
        emitHeader()
        
        for decl in declarations {
            visit(decl: decl)
            emit("\n\n")
        }
        
        return output
    }
    
    // MARK: - Output Helpers
    
    private func emit(_ text: String) {
        output += text
    }
    
    private func emitLine(_ text: String = "") {
        if text.isEmpty {
            output += "\n"
        } else {
            let indentation = String(repeating: " ", count: indentLevel * config.indentSize)
            output += "\(indentation)\(text)\n"
        }
    }
    
    private func indent() { indentLevel += 1 }
    private func dedent() { indentLevel -= 1 }
    
    private func emitHeader() {
        switch config.targetLanguage {
        case .c, .cpp:
            emitLine("// Transpiled from HyperLang")
            emitLine("#include <stdio.h>")
            emitLine("#include <stdlib.h>")
            emitLine("#include <stdbool.h>")
            emitLine()
        case .swift:
            emitLine("// Transpiled from HyperLang")
            emitLine("import Foundation")
            emitLine()
        }
    }
    
    // MARK: - Declaration Visitors
    
    private func visit(decl: Decl) {
        switch decl {
        case let module as ModuleDecl: visit(module: module)
        case let include as IncludeDecl: visit(include: include)
        case let cls as ClassDecl: visit(class: cls)
        case let def as DefDecl: visit(def: def)
        case let vr as VarDecl: visit(varDecl: vr)
        // Add routing for StructDecl, InterfaceDecl, etc.
        default:
            emitLine("// TODO: Unsupported declaration type")
        }
    }
    
    private func visit(module: ModuleDecl) {
        emitLine("// module \(module.name.lexeme)")
    }
    
    private func visit(include: IncludeDecl) {
        switch config.targetLanguage {
        case .c, .cpp:
            emitLine("#include \"\(include.modulePath.lexeme).h\"")
        case .swift:
            emitLine("import \(include.modulePath.lexeme)")
        }
    }
    
    private func visit(def: DefDecl) {
        // Example for Swift target
        let modifiers = def.modifiers.map { $0.lexeme }.joined(separator: " ")
        let modsPrefix = modifiers.isEmpty ? "" : "\(modifiers) "
        
        emit("\(modsPrefix)func \(def.name.lexeme)(")
        
        for (i, param) in def.parameters.enumerated() {
            let inoutPrefix = param.inoutModifier != nil ? "inout " : ""
            emit("\(inoutPrefix)\(param.name.lexeme): ")
            visit(type: param.type)
            if i < def.parameters.count - 1 { emit(", ") }
        }
        
        emit(")")
        
        if def.isAsync { emit(" async") }
        if def.isThrows { emit(" throws") }
        
        if let returnType = def.returnType {
            emit(" -> ")
            visit(type: returnType)
        }
        
        emit(" ")
        
        if let body = def.body {
            visit(block: body)
        } else {
            emitLine("{} // Pass/Empty body")
        }
    }
    
    private func visit(varDecl: VarDecl) {
        let keyword = varDecl.keyword.type == .keywordVal ? "let" : "var"
        emit("\(keyword) \(varDecl.name.lexeme)")
        
        if let type = varDecl.typeAnnotation {
            emit(": ")
            visit(type: type)
        }
        
        if let initializer = varDecl.initializer {
            emit(" = ")
            visit(expr: initializer)
        }
        emitLine()
    }
    
    // MARK: - Statement Visitors
    
    private func visit(stmt: Stmt) {
        switch stmt {
        case let exprStmt as ExpressionStmt:
            let indentation = String(repeating: " ", count: indentLevel * config.indentSize)
            emit(indentation)
            visit(expr: exprStmt.expression)
            emitLine()
        case let block as BlockStmt: visit(block: block)
        case let ifStmt as IfStmt: visit(ifStmt: ifStmt)
        case let ret as ReturnStmt:
            let indentation = String(repeating: " ", count: indentLevel * config.indentSize)
            emit("\(indentation)return")
            if let val = ret.value {
                emit(" ")
                visit(expr: val)
            }
            emitLine()
        // Route WhileStmt, LoopStmt, ForInStmt, etc.
        default:
            emitLine("// TODO: Unsupported statement")
        }
    }
    
    private func visit(block: BlockStmt) {
        emitLine("{")
        indent()
        for stmt in block.statements {
            visit(stmt: stmt)
        }
        dedent()
        let indentation = String(repeating: " ", count: indentLevel * config.indentSize)
        emitLine("\(indentation)}")
    }
    
    private func visit(ifStmt: IfStmt) {
        let indentation = String(repeating: " ", count: indentLevel * config.indentSize)
        emit("\(indentation)if ")
        visit(expr: ifStmt.condition)
        emit(" ")
        visit(block: ifStmt.thenBranch)
        
        if let elseBranch = ifStmt.elseBranch {
            emit("\(indentation)else ")
            if let elseIf = elseBranch as? IfStmt {
                visit(ifStmt: elseIf)
            } else if let elseBlock = elseBranch as? BlockStmt {
                visit(block: elseBlock)
            }
        }
    }
    
    // MARK: - Expression Visitors
    
    private func visit(expr: Expr) {
        switch expr {
        case let literal as LiteralExpr:
            emit(literal.value.lexeme)
        case let ident as IdentifierExpr:
            emit(ident.name.lexeme)
        case let binary as BinaryExpr:
            visit(expr: binary.left)
            emit(" \(binary.operatorToken.lexeme) ")
            visit(expr: binary.right)
        case let call as CallExpr:
            visit(expr: call.callee)
            emit("(")
            for (i, arg) in call.arguments.enumerated() {
                visit(expr: arg)
                if i < call.arguments.count - 1 { emit(", ") }
            }
            emit(")")
        case let member as MemberAccessExpr:
            visit(expr: member.object)
            emit(".\(member.name.lexeme)")
        // Add unary, logical, assignment, try/await, etc.
        default:
            emit("/* unsupported expr */")
        }
    }
    
    // MARK: - Type Visitors
    
    private func visit(type: TypeExpr) {
        switch type {
        case let simple as SimpleTypeExpr:
            emit(simple.name.lexeme)
        case let optional as OptionalTypeExpr:
            visit(type: optional.baseType)
            emit("?")
        default:
            emit("Any")
        }
    }
}
