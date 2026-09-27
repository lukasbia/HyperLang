import Foundation

// Generates Swift source from the parsed program.
public final class CodeGenerator {
    private var output: String = ""
    private var indentation: Int = 0
    private var configuration: CodeGeneratorConfiguration

    public init(
        configuration: CodeGeneratorConfiguration = CodeGeneratorConfiguration()
    ) {
        self.configuration = configuration
    }

    public func generate(_ program: Program) -> String {
        output.removeAll(keepingCapacity: true)
        indentation = 0

        emitHeader()

        for declaration in program.declarations {
            emitDeclaration(declaration)
            emitBlankLine()
        }

        for statement in program.statements {
            emitStatement(statement)
        }

        return output
    }

    // Writes the generated source header.
    private func emitHeader() {
        if configuration.emitGeneratedHeader {
            emitLine("// Generated source.")
            emitBlankLine()
        }
    }

    // Writes a declaration.
    private func emitDeclaration(_ declaration: Declaration) {
        switch declaration {
        case .function(let function):
            emitFunction(function)

        case .variable(let variable):
            emitVariable(variable)

        case .type(let type):
            emitType(type)

        case .classType(let classDeclaration):
            emitClass(classDeclaration)

        case .structType(let structDeclaration):
            emitStruct(structDeclaration)

        case .enumType(let enumDeclaration):
            emitEnum(enumDeclaration)

        case .template(let template):
            emitTemplate(template)

        case .protocolType(let proto):
            emitProtocol(proto)

        case .extensionType(let extensionDeclaration):
            emitExtension(extensionDeclaration)

        case .importDeclaration(let importDeclaration):
            emitImport(importDeclaration)

        case .exportDeclaration(let exportDeclaration):
            emitExport(exportDeclaration)

        case .moduleDeclaration(let moduleDeclaration):
            emitModule(moduleDeclaration)
        }
    }

    // Writes a function.
    private func emitFunction(_ function: FunctionDeclaration) {
        let modifiers = emitModifiers(function.modifiers)
        let parameters = function.parameters
            .map(emitParameter)
            .joined(separator: ", ")

        var signature = "\(modifiers)func \(function.name)(\(parameters))"

        if let returnType = function.returnType {
            signature += " -> \(emitTypeReference(returnType))"
        }

        emitLine(signature + " {")

        indentation += 1

        for statement in function.body {
            emitStatement(statement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a variable declaration.
    private func emitVariable(_ variable: VariableDeclaration) {
        let keyword = variable.isMutable ? "var" : "let"
        var line = "\(keyword) \(variable.name)"

        if let type = variable.type {
            line += ": \(emitTypeReference(type))"
        }

        if let initializer = variable.initializer {
            line += " = \(emitExpression(initializer))"
        }

        emitLine(line)
    }

    // Writes a type alias.
    private func emitType(_ type: TypeDeclaration) {
        var line = "typealias \(type.name)"

        if let underlying = type.underlyingType {
            line += " = \(emitTypeReference(underlying))"
        }

        emitLine(line)
    }

    // Writes a class declaration.
    private func emitClass(_ declaration: ClassDeclaration) {
        let modifiers = emitModifiers(declaration.modifiers)
        var line = "\(modifiers)class \(declaration.name)"

        if !declaration.inheritedTypes.isEmpty {
            line += ": "
            line += declaration.inheritedTypes
                .map(emitTypeReference)
                .joined(separator: ", ")
        }

        emitLine(line + " {")
        indentation += 1

        for member in declaration.members {
            emitDeclaration(member)
            emitBlankLineIfNeeded()
        }

        for statement in declaration.statements {
            emitStatement(statement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a struct declaration.
    private func emitStruct(_ declaration: StructDeclaration) {
        let modifiers = emitModifiers(declaration.modifiers)
        var line = "\(modifiers)struct \(declaration.name)"

        if !declaration.inheritedTypes.isEmpty {
            line += ": "
            line += declaration.inheritedTypes
                .map(emitTypeReference)
                .joined(separator: ", ")
        }

        emitLine(line + " {")
        indentation += 1

        for member in declaration.members {
            emitDeclaration(member)
            emitBlankLineIfNeeded()
        }

        for statement in declaration.statements {
            emitStatement(statement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes an enum declaration.
    private func emitEnum(_ declaration: EnumDeclaration) {
        let modifiers = emitModifiers(declaration.modifiers)
        emitLine("\(modifiers)enum \(declaration.name) {")
        indentation += 1

        for enumCase in declaration.cases {
            var line = "case \(enumCase.name)"

            if !enumCase.associatedValues.isEmpty {
                line += "("
                line += enumCase.associatedValues
                    .map(emitTypeReference)
                    .joined(separator: ", ")
                line += ")"
            }

            emitLine(line)
        }

        if !declaration.members.isEmpty {
            emitBlankLine()

            for member in declaration.members {
                emitDeclaration(member)
                emitBlankLineIfNeeded()
            }
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a template declaration as a generic Swift declaration.
    private func emitTemplate(_ declaration: TemplateDeclaration) {
        let modifiers = emitModifiers(declaration.modifiers)
        let parameters = declaration.parameters.joined(separator: ", ")

        emitLine(
            "\(modifiers)struct \(declaration.name)<\(parameters)> {"
        )

        indentation += 1

        for member in declaration.members {
            emitDeclaration(member)
            emitBlankLineIfNeeded()
        }

        for statement in declaration.statements {
            emitStatement(statement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a protocol declaration.
    private func emitProtocol(_ declaration: ProtocolDeclaration) {
        let modifiers = emitModifiers(declaration.modifiers)
        var line = "\(modifiers)protocol \(declaration.name)"

        if !declaration.inheritedTypes.isEmpty {
            line += ": "
            line += declaration.inheritedTypes
                .map(emitTypeReference)
                .joined(separator: ", ")
        }

        emitLine(line + " {")
        indentation += 1

        for requirement in declaration.requirements {
            emitProtocolRequirement(requirement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a protocol requirement.
    private func emitProtocolRequirement(_ declaration: Declaration) {
        switch declaration {
        case .function(let function):
            let modifiers = emitModifiers(function.modifiers)
            let parameters = function.parameters
                .map(emitParameter)
                .joined(separator: ", ")

            var line = "\(modifiers)func \(function.name)(\(parameters))"

            if let returnType = function.returnType {
                line += " -> \(emitTypeReference(returnType))"
            }

            emitLine(line)

        case .variable(let variable):
            let keyword = variable.isMutable ? "var" : "let"
            var line = "\(keyword) \(variable.name)"

            if let type = variable.type {
                line += ": \(emitTypeReference(type))"
            }

            emitLine(line)

        default:
            emitDeclaration(declaration)
        }
    }

    // Writes an extension declaration.
    private func emitExtension(_ declaration: ExtensionDeclaration) {
        let modifiers = emitModifiers(declaration.modifiers)
        var line = "\(modifiers)extension \(emitTypeReference(declaration.target))"

        if !declaration.inheritedTypes.isEmpty {
            line += ": "
            line += declaration.inheritedTypes
                .map(emitTypeReference)
                .joined(separator: ", ")
        }

        emitLine(line + " {")
        indentation += 1

        for member in declaration.members {
            emitDeclaration(member)
            emitBlankLineIfNeeded()
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes an import.
    private func emitImport(_ declaration: ImportDeclaration) {
        emitLine("import \(declaration.path.joined(separator: "."))")
    }

    // Writes an export.
    private func emitExport(_ declaration: ExportDeclaration) {
        emitLine("// Exported declaration: \(declaration.name)")
    }

    // Writes a module declaration.
    private func emitModule(_ declaration: ModuleDeclaration) {
        emitLine("// Module: \(declaration.name)")
    }

    // Writes a statement.
    private func emitStatement(_ statement: Statement) {
        switch statement {
        case .expression(let expression):
            emitLine(emitExpression(expression))

        case .variable(let variable):
            emitVariable(variable)

        case .returnStatement(let expression):
            if let expression {
                emitLine("return \(emitExpression(expression))")
            } else {
                emitLine("return")
            }

        case .ifStatement(let statement):
            emitIf(statement)

        case .guardStatement(let statement):
            emitGuard(statement)

        case .matchStatement(let statement):
            emitMatch(statement)

        case .whileStatement(let statement):
            emitWhile(statement)

        case .forStatement(let statement):
            emitFor(statement)

        case .breakStatement:
            emitLine("break")

        case .continueStatement:
            emitLine("continue")

        case .throwStatement(let expression):
            emitLine("throw \(emitExpression(expression))")

        case .tryStatement(let statement):
            emitTry(statement)

        case .deferStatement(let body):
            emitLine("defer {")
            indentation += 1

            for statement in body {
                emitStatement(statement)
            }

            indentation -= 1
            emitLine("}")

        case .block(let statements):
            emitLine("{")
            indentation += 1

            for statement in statements {
                emitStatement(statement)
            }

            indentation -= 1
            emitLine("}")
        }
    }

    // Writes an if statement.
    private func emitIf(_ statement: IfStatement) {
        emitLine("if \(emitExpression(statement.condition)) {")
        indentation += 1

        for bodyStatement in statement.body {
            emitStatement(bodyStatement)
        }

        indentation -= 1

        guard let elseBody = statement.elseBody else {
            emitLine("}")
            return
        }

        emitLine("} else {")
        indentation += 1

        for bodyStatement in elseBody {
            emitStatement(bodyStatement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a guard statement.
    private func emitGuard(_ statement: GuardStatement) {
        emitLine("guard \(emitExpression(statement.condition)) else {")
        indentation += 1

        for bodyStatement in statement.body {
            emitStatement(bodyStatement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a match statement.
    private func emitMatch(_ statement: MatchStatement) {
        emitLine("switch \(emitExpression(statement.expression)) {")
        indentation += 1

        for matchCase in statement.cases {
            if matchCase.isDefault {
                emitLine("default:")
            } else if let pattern = matchCase.pattern {
                emitLine("case \(emitExpression(pattern)):")
            } else {
                emitLine("default:")
            }

            indentation += 1

            for caseStatement in matchCase.statements {
                emitStatement(caseStatement)
            }

            indentation -= 1
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a while loop.
    private func emitWhile(_ statement: WhileStatement) {
        emitLine("while \(emitExpression(statement.condition)) {")
        indentation += 1

        for bodyStatement in statement.body {
            emitStatement(bodyStatement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a for loop.
    private func emitFor(_ statement: ForStatement) {
        emitLine(
            "for \(statement.name) in \(emitExpression(statement.sequence)) {"
        )

        indentation += 1

        for bodyStatement in statement.body {
            emitStatement(bodyStatement)
        }

        indentation -= 1
        emitLine("}")
    }

    // Writes a try and catch sequence.
    private func emitTry(_ statement: TryStatement) {
        emitLine("do {")
        indentation += 1

        for bodyStatement in statement.body {
            emitStatement(bodyStatement)
        }

        indentation -= 1

        for catchClause in statement.catches {
            emitCatch(catchClause)
        }

        emitLine("}")
    }

    // Writes a catch clause.
    private func emitCatch(_ clause: CatchClause) {
        if let name = clause.name {
            if let type = clause.type {
                emitLine("} catch let \(name) as \(emitTypeReference(type)) {")
            } else {
                emitLine("} catch let \(name) {")
            }
        } else {
            emitLine("} catch {")
        }

        indentation += 1

        for statement in clause.body {
            emitStatement(statement)
        }

        indentation -= 1
    }

    // Writes an expression.
    private func emitExpression(_ expression: Expression) -> String {
        switch expression {
        case .identifier(let value):
            return value

        case .integer(let value):
            return value

        case .floating(let value):
            return value

        case .string(let value):
            return "\"\(escapeString(value))\""

        case .character(let value):
            return "'\(escapeCharacter(value))'"

        case .boolean(let value):
            return value ? "true" : "false"

        case .nilValue:
            return "nil"

        case .selfExpression:
            return "self"

        case .unary(let operation, let operand):
            return emitUnary(
                operation,
                operand
            )

        case .binary(let left, let operation, let right):
            return emitBinary(
                left,
                operation,
                right
            )

        case .assignment(let left, let operation, let right):
            return emitAssignment(
                left,
                operation,
                right
            )

        case .call(let callee, let arguments):
            let values = arguments
                .map(emitExpression)
                .joined(separator: ", ")

            return "\(emitExpression(callee))(\(values))"

        case .member(let base, let member):
            return "\(emitExpression(base)).\(member)"

        case .subscriptExpression(let base, let arguments):
            let values = arguments
                .map(emitExpression)
                .joined(separator: ", ")

            return "\(emitExpression(base))[\(values)]"

        case .functionLiteral(let parameters, let statements):
            return emitFunctionLiteral(
                parameters: parameters,
                statements: statements
            )

        case .parenthesized(let value):
            return "(\(emitExpression(value)))"
        }
    }

    // Writes a unary expression.
    private func emitUnary(
        _ operation: TokenKind,
        _ operand: Expression
    ) -> String {
        let operatorText = swiftOperator(operation)
        return "\(operatorText)\(emitExpression(operand))"
    }

    // Writes a binary expression.
    private func emitBinary(
        _ left: Expression,
        _ operation: TokenKind,
        _ right: Expression
    ) -> String {
        let leftText = emitExpression(left)
        let rightText = emitExpression(right)
        let operatorText = swiftOperator(operation)

        return "\(leftText) \(operatorText) \(rightText)"
    }

    // Writes an assignment expression.
    private func emitAssignment(
        _ left: Expression,
        _ operation: TokenKind,
        _ right: Expression
    ) -> String {
        let leftText = emitExpression(left)
        let rightText = emitExpression(right)
        let operatorText = swiftOperator(operation)

        if configuration.expandCompoundAssignments {
            switch operation {
            case .plusEqual:
                return "\(leftText) = \(leftText) + \(rightText)"

            case .minusEqual:
                return "\(leftText) = \(leftText) - \(rightText)"

            case .starEqual:
                return "\(leftText) = \(leftText) * \(rightText)"

            case .slashEqual:
                return "\(leftText) = \(leftText) / \(rightText)"

            case .percentEqual:
                return "\(leftText) = \(leftText) % \(rightText)"

            default:
                break
            }
        }

        return "\(leftText) \(operatorText) \(rightText)"
    }

    // Writes a function literal.
    private func emitFunctionLiteral(
        parameters: [ParameterDeclaration],
        statements: [Statement]
    ) -> String {
        let parametersText = parameters
            .map(emitParameter)
            .joined(separator: ", ")

        var value = "{ \(parametersText) in"

        if !statements.isEmpty {
            value += " "

            for statement in statements {
                value += emitInlineStatement(statement)
                value += " "
            }
        }

        value += "}"
        return value
    }

    // Writes an inline statement.
    private func emitInlineStatement(
        _ statement: Statement
    ) -> String {
        switch statement {
        case .returnStatement(let expression):
            if let expression {
                return "return \(emitExpression(expression))"
            }

            return "return"

        case .expression(let expression):
            return emitExpression(expression)

        case .variable(let variable):
            let keyword = variable.isMutable ? "var" : "let"
            var value = "\(keyword) \(variable.name)"

            if let type = variable.type {
                value += ": \(emitTypeReference(type))"
            }

            if let initializer = variable.initializer {
                value += " = \(emitExpression(initializer))"
            }

            return value

        default:
            return ""
        }
    }

    // Writes a parameter.
    private func emitParameter(
        _ parameter: ParameterDeclaration
    ) -> String {
        var result = ""

        if let externalName = parameter.externalName {
            result += "\(externalName) "
        }

        result += parameter.name

        if let type = parameter.type {
            result += ": \(emitTypeReference(type))"
        }

        if let defaultValue = parameter.defaultValue {
            result += " = \(emitExpression(defaultValue))"
        }

        return result
    }

    // Writes a type reference.
    private func emitTypeReference(
        _ type: TypeReference
    ) -> String {
        switch type {
        case .named(let name):
            return mapTypeName(name)

        case .optional(let wrapped):
            return "\(emitTypeReference(wrapped))?"

        case .generic(let name, let arguments):
            let values = arguments
                .map(emitTypeReference)
                .joined(separator: ", ")

            return "\(mapTypeName(name))<\(values)>"

        case .function(let parameters, let returnType):
            let parameterText = parameters
                .map(emitTypeReference)
                .joined(separator: ", ")

            if let returnType {
                return "(\(parameterText)) -> \(emitTypeReference(returnType))"
            }

            return "(\(parameterText)) -> Void"
        }
    }

    // Maps source language type names to Swift.
    private func mapTypeName(_ name: String) -> String {
        switch name {
        case "int":
            return "Int"

        case "uint":
            return "UInt"

        case "float":
            return "Float"

        case "double":
            return "Double"

        case "bool":
            return "Bool"

        case "string":
            return "String"

        case "char":
            return "Character"

        case "void":
            return "Void"

        case "auto":
            return "some Any"

        default:
            return name
        }
    }

    // Writes modifiers.
    private func emitModifiers(
        _ modifiers: [String]
    ) -> String {
        guard !modifiers.isEmpty else {
            return ""
        }

        return modifiers
            .map(mapModifier)
            .joined(separator: " ") + " "
    }

    // Maps source modifiers to Swift.
    private func mapModifier(_ modifier: String) -> String {
        switch modifier {
        case "public":
            return "public"

        case "private":
            return "private"

        case "protected":
            return "internal"

        case "static":
            return "static"

        case "final":
            return "final"

        case "const":
            return ""

        default:
            return modifier
        }
    }

    // Maps operators to Swift syntax.
    private func swiftOperator(_ kind: TokenKind) -> String {
        switch kind {
        case .equal:
            return "="

        case .equalEqual:
            return "=="

        case .notEqual:
            return "!="

        case .less:
            return "<"

        case .lessEqual:
            return "<="

        case .greater:
            return ">"

        case .greaterEqual:
            return ">="

        case .plus:
            return "+"

        case .minus:
            return "-"

        case .star:
            return "*"

        case .slash:
            return "/"

        case .percent:
            return "%"

        case .plusEqual:
            return "+="

        case .minusEqual:
            return "-="

        case .starEqual:
            return "*="

        case .slashEqual:
            return "/="

        case .percentEqual:
            return "%="

        case .and:
            return "&&"

        case .or:
            return "||"

        case .not:
            return "!"

        case .exclamation:
            return "!"

        default:
            return ""
        }
    }

    // Escapes a generated string.
    private func escapeString(_ value: String) -> String {
        value
            .replacingOccurrences(of: "\\", with: "\\\\")
            .replacingOccurrences(of: "\"", with: "\\\"")
            .replacingOccurrences(of: "\n", with: "\\n")
            .replacingOccurrences(of: "\r", with: "\\r")
            .replacingOccurrences(of: "\t", with: "\\t")
    }

    // Escapes a generated character.
    private func escapeCharacter(_ value: String) -> String {
        value
            .replacingOccurrences(of: "\\", with: "\\\\")
            .replacingOccurrences(of: "'", with: "\\'")
    }

    // Writes one source line.
    private func emitLine(_ line: String) {
        let prefix = String(
            repeating: configuration.indentationUnit,
            count: indentation
        )

        output += prefix
        output += line
        output += "\n"
    }

    // Writes an empty line.
    private func emitBlankLine() {
        output += "\n"
    }

    // Writes an empty line when needed.
    private func emitBlankLineIfNeeded() {
        if !output.hasSuffix("\n\n") {
            output += "\n"
        }
    }
}

// Code generator configuration.
public struct CodeGeneratorConfiguration {
    public var indentationUnit: String
    public var emitGeneratedHeader: Bool
    public var expandCompoundAssignments: Bool

    public init(
        indentationUnit: String = "    ",
        emitGeneratedHeader: Bool = true,
        expandCompoundAssignments: Bool = true
    ) {
        self.indentationUnit = indentationUnit
        self.emitGeneratedHeader = emitGeneratedHeader
        self.expandCompoundAssignments = expandCompoundAssignments
    }
}

// Transpiler result.
public struct TranspilationResult {
    public let source: String
    public let diagnostics: [ParserDiagnostic]

    public init(
        source: String,
        diagnostics: [ParserDiagnostic] = []
    ) {
        self.source = source
        self.diagnostics = diagnostics
    }

    public var succeeded: Bool {
        diagnostics.allSatisfy {
            $0.severity != .error
        }
    }
}

// Complete source-to-Swift transpiler.
public final class Transpiler {
    private let parserConfiguration: ParserConfiguration
    private let generatorConfiguration: CodeGeneratorConfiguration

    public init(
        parserConfiguration: ParserConfiguration = ParserConfiguration(),
        generatorConfiguration: CodeGeneratorConfiguration = CodeGeneratorConfiguration()
    ) {
        self.parserConfiguration = parserConfiguration
        self.generatorConfiguration = generatorConfiguration
    }

    public func transpile(_ source: String) -> TranspilationResult {
        let parser = Parser(
            source: source,
            configuration: parserConfiguration
        )

        let (program, diagnostics) = parser.parseWithDiagnostics()

        guard diagnostics.allSatisfy({
            $0.severity != .error
        }) else {
            return TranspilationResult(
                source: "",
                diagnostics: diagnostics
            )
        }

        let generator = CodeGenerator(
            configuration: generatorConfiguration
        )

        let swiftSource = generator.generate(program)

        return TranspilationResult(
            source: swiftSource,
            diagnostics: diagnostics
        )
    }
}

// Transpiler convenience API.
public enum LanguageTranspiler {
    public static func transpile(
        _ source: String
    ) -> TranspilationResult {
        Transpiler().transpile(source)
    }

    public static func transpile(
        _ source: String,
        configuration: CodeGeneratorConfiguration
    ) -> TranspilationResult {
        Transpiler(
            generatorConfiguration: configuration
        ).transpile(source)
    }
}

// Generated source container.
public struct GeneratedSource {
    public let language: String
    public let source: String

    public init(
        language: String = "Swift",
        source: String
    ) {
        self.language = language
        self.source = source
    }

    public var isEmpty: Bool {
        source.trimmingCharacters(
            in: .whitespacesAndNewlines
        ).isEmpty
    }
}

// Code generation statistics.
public struct CodeGenerationStatistics {
    public var sourceLength: Int
    public var sourceLines: Int
    public var generatedLength: Int
    public var generatedLines: Int

    public init(
        source: String,
        generated: String
    ) {
        sourceLength = source.count
        sourceLines = source.split(
            separator: "\n",
            omittingEmptySubsequences: false
        ).count

        generatedLength = generated.count
        generatedLines = generated.split(
            separator: "\n",
            omittingEmptySubsequences: false
        ).count
    }
}

// Code generation service.
public final class CodeGenerationService {
    private let transpiler: Transpiler

    public init(
        parserConfiguration: ParserConfiguration = ParserConfiguration(),
        generatorConfiguration: CodeGeneratorConfiguration = CodeGeneratorConfiguration()
    ) {
        transpiler = Transpiler(
            parserConfiguration: parserConfiguration,
            generatorConfiguration: generatorConfiguration
        )
    }

    public func generate(
        source: String
    ) -> GeneratedSource {
        let result = transpiler.transpile(source)

        return GeneratedSource(
            source: result.source
        )
    }

    public func generateWithDiagnostics(
        source: String
    ) -> TranspilationResult {
        transpiler.transpile(source)
    }
}

// Code generation test cases.
public enum CodeGenerationTestCases {
    public static let hello = """
    func hello() {
        print("Hello")
    }
    """

    public static let arithmetic = """
    func add(a: int, b: int) -> int {
        return a + b
    }
    """

    public static let variables = """
    let value: int = 10
    var total: int = value
    total += 5
    """

    public static let condition = """
    func check(value: int) -> string {
        if value > 0 {
            return "positive"
        } else {
            return "zero or negative"
        }
    }
    """

    public static let loop = """
    func count(values: int) {
        for value in values {
            print(value)
        }
    }
    """

    public static let type = """
    struct Point {
        var x: int
        var y: int
    }
    """
}

// Code generator smoke tests.
public enum CodeGeneratorTests {
    public static func run() -> Bool {
        testHello() &&
        testArithmetic() &&
        testVariables() &&
        testCondition() &&
        testLoop() &&
        testType()
    }

    private static func testHello() -> Bool {
        let result = LanguageTranspiler.transpile(
            CodeGenerationTestCases.hello
        )

        return result.succeeded &&
            result.source.contains("func hello")
    }

    private static func testArithmetic() -> Bool {
        let result = LanguageTranspiler.transpile(
            CodeGenerationTestCases.arithmetic
        )

        return result.succeeded &&
            result.source.contains("return a + b")
    }

    private static func testVariables() -> Bool {
        let result = LanguageTranspiler.transpile(
            CodeGenerationTestCases.variables
        )

        return result.succeeded &&
            result.source.contains("total = total + 5")
    }

    private static func testCondition() -> Bool {
        let result = LanguageTranspiler.transpile(
            CodeGenerationTestCases.condition
        )

        return result.succeeded &&
            result.source.contains("if value > 0")
    }

    private static func testLoop() -> Bool {
        let result = LanguageTranspiler.transpile(
            CodeGenerationTestCases.loop
        )

        return result.succeeded &&
            result.source.contains("for value in values")
    }

    private static func testType() -> Bool {
        let result = LanguageTranspiler.transpile(
            CodeGenerationTestCases.type
        )

        return result.succeeded &&
            result.source.contains("struct Point")
    }
}

// Code generator test runner.
public enum CodeGeneratorTestRunner {
    public static func run() -> Bool {
        let result = CodeGeneratorTests.run()

        print(
            "code generation tests: " +
            (result ? "passed" : "failed")
        )

        return result
    }
}

// Code generation entry point.
public enum CodeGeneratorEntryPoint {
    public static func generate(
        program: Program
    ) -> String {
        CodeGenerator().generate(program)
    }

    public static func transpile(
        source: String
    ) -> TranspilationResult {
        LanguageTranspiler.transpile(source)
    }
}

// Code generation version.
public enum CodeGeneratorVersion {
    public static let major = 0
    public static let minor = 1
    public static let patch = 0

    public static var string: String {
        "\(major).\(minor).\(patch)"
    }
}

// Code generation feature information.
public enum CodeGeneratorFeatures {
    public static let swiftOutput = true
    public static let functions = true
    public static let variables = true
    public static let classes = true
    public static let structs = true
    public static let enums = true
    public static let templates = true
    public static let protocols = true
    public static let extensions = true
    public static let imports = true
    public static let controlFlow = true
    public static let expressions = true
    public static let typeMapping = true
    public static let compoundAssignmentExpansion = true
}

// Source generation result builder.
public final class GeneratedSourceBuilder {
    private var source: String = ""

    public init() {}

    public func append(_ value: String) {
        source += value
    }

    public func appendLine(_ value: String) {
        source += value
        source += "\n"
    }

    public func appendBlankLine() {
        source += "\n"
    }

    public func build() -> GeneratedSource {
        GeneratedSource(source: source)
    }

    public func clear() {
        source.removeAll(keepingCapacity: true)
    }
}

// Generated source validator.
public enum GeneratedSourceValidator {
    public static func hasSwiftFunction(
        _ source: String
    ) -> Bool {
        source.contains("func ")
    }

    public static func hasSwiftStruct(
        _ source: String
    ) -> Bool {
        source.contains("struct ")
    }

    public static func hasSwiftClass(
        _ source: String
    ) -> Bool {
        source.contains("class ")
    }

    public static func hasSwiftEnum(
        _ source: String
    ) -> Bool {
        source.contains("enum ")
    }
}

// Source generation pipeline.
public final class SourceGenerationPipeline {
    private let parserConfiguration: ParserConfiguration
    private let generatorConfiguration: CodeGeneratorConfiguration

    public init(
        parserConfiguration: ParserConfiguration = ParserConfiguration(),
        generatorConfiguration: CodeGeneratorConfiguration = CodeGeneratorConfiguration()
    ) {
        self.parserConfiguration = parserConfiguration
        self.generatorConfiguration = generatorConfiguration
    }

    public func run(
        source: String
    ) -> TranspilationResult {
        let parser = Parser(
            source: source,
            configuration: parserConfiguration
        )

        let (program, diagnostics) = parser.parseWithDiagnostics()

        if diagnostics.contains(where: {
            $0.severity == .error
        }) {
            return TranspilationResult(
                source: "",
                diagnostics: diagnostics
            )
        }

        let generator = CodeGenerator(
            configuration: generatorConfiguration
        )

        return TranspilationResult(
            source: generator.generate(program),
            diagnostics: diagnostics
        )
    }
}

// Generator diagnostics formatter.
public enum CodeGenerationDiagnostics {
    public static func format(
        _ result: TranspilationResult
    ) -> String {
        guard !result.diagnostics.isEmpty else {
            return "code generation succeeded"
        }

        return result.diagnostics
            .map(ParserDiagnosticFormatter.format)
            .joined(separator: "\n")
    }
}

// Generator metadata.
public enum CodeGeneratorModule {
    public static let name = "CodeGenerator"
    public static let implementationLanguage = "Swift"
    public static let outputLanguage = "Swift"
    public static let version = CodeGeneratorVersion.string
}

// Generator presets.
public enum CodeGeneratorPresets {
    public static let standard = CodeGeneratorConfiguration()

    public static let compact = CodeGeneratorConfiguration(
        indentationUnit: "  ",
        emitGeneratedHeader: false,
        expandCompoundAssignments: true
    )

    public static let preserveOperators = CodeGeneratorConfiguration(
        indentationUnit: "    ",
        emitGeneratedHeader: true,
        expandCompoundAssignments: false
    )
}