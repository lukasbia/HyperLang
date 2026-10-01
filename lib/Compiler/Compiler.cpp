#include "hyper/Compiler/Compiler.h"

#include <sstream>
#include <string>
#include <utility>

namespace hyper::compiler {

namespace {

void lowerNode(const SyntaxNode& node, hil::BasicBlock& block) {
    switch (node.kind) {
        case SyntaxKind::VariableDeclaration: {
            hil::Instruction instruction(hil::Opcode::Allocate);
            instruction.setResult({
                static_cast<std::uint32_t>(block.instructions().size() + 1),
                "variable",
                node.text
            });
            block.append(std::move(instruction));
            break;
        }
        case SyntaxKind::LiteralExpression: {
            hil::Instruction instruction(hil::Opcode::Constant);
            instruction.setResult({
                static_cast<std::uint32_t>(block.instructions().size() + 1),
                "literal",
                node.text
            });
            block.append(std::move(instruction));
            break;
        }
        case SyntaxKind::CallExpression: {
            hil::Instruction instruction(hil::Opcode::Call);
            instruction.addOperand({0, "function", node.text});
            block.append(std::move(instruction));
            break;
        }
        case SyntaxKind::ReturnStatement:
            block.append(hil::Instruction(hil::Opcode::Return));
            break;
        case SyntaxKind::IfStatement:
        case SyntaxKind::WhileStatement:
        case SyntaxKind::ForStatement:
        case SyntaxKind::DoStatement:
        case SyntaxKind::SwitchStatement:
            block.append(hil::Instruction(hil::Opcode::ConditionalBranch));
            break;
        default:
            break;
    }

    for (const auto& child : node.children) {
        lowerNode(*child, block);
    }
}

void collectFunctions(const SyntaxNode& node, hil::Module& module) {
    if (node.kind == SyntaxKind::FunctionDeclaration) {
        hil::Function function(node.text.empty() ? "anonymous" : node.text);
        hil::BasicBlock entry("entry");
        lowerNode(node, entry);
        entry.append(hil::Instruction(hil::Opcode::Return));
        function.addBlock(std::move(entry));
        module.addFunction(std::move(function));
        return;
    }

    for (const auto& child : node.children) {
        collectFunctions(*child, module);
    }
}

bool checkNode(const SyntaxNode& node, sema::Sema& sema) {
    if (node.kind == SyntaxKind::FunctionDeclaration) {
        sema.declare({
            node.text.empty() ? "anonymous" : node.text,
            {"function", true},
            false
        });
    }

    for (const auto& child : node.children) {
        if (!checkNode(*child, sema)) {
            return false;
        }
    }

    return true;
}

} // namespace

Compiler::Compiler(CompilerOptions options)
    : options_(std::move(options)) {}

CompilationResult Compiler::compile(std::string_view source) {
    CompilationResult result;
    result.hilModule = hil::Module(options_.moduleName);
    result.lscModule = llcvm::Module(options_.moduleName);

    Lexer lexer(source);
    const auto tokens = lexer.tokenize();
    result.lexerDiagnostics = lexer.diagnostics();

    Parser parser(tokens);
    result.syntaxTree = parser.parse();
    result.parserDiagnostics = parser.diagnostics();

    if (!result.lexerDiagnostics.empty() ||
        !result.parserDiagnostics.empty() ||
        !result.syntaxTree) {
        return result;
    }

    sema::Sema sema;
    sema.enterScope();
    const bool semaOK = runSemanticChecks(*result.syntaxTree, sema);
    result.semaDiagnostics = sema.diagnostics();
    if (!semaOK || !result.semaDiagnostics.empty()) {
        return result;
    }

    lowerSyntaxToHIL(*result.syntaxTree, result.hilModule);

    hil::optimization::Pipeline optimization;
    optimization.addPass({"constant-folding", true});
    optimization.addPass({"dead-code-elimination", true});
    optimization.addPass({"ownership-cleanup", true});
    optimization.addPass({"control-flow-simplification", true});

    appendLineIR(source, result.lscModule);

    if (options_.emitMachineCode) {
        llcvm::backend::Backend backend(options_.target);
        result.machineCode = backend.emit(result.lscModule);
    }

    result.success = true;
    return result;
}

void Compiler::lowerSyntaxToHIL(const SyntaxNode& node, hil::Module& module) {
    collectFunctions(node, module);

    if (module.functions().empty()) {
        hil::Function main("main");
        hil::BasicBlock entry("entry");
        lowerNode(node, entry);
        entry.append(hil::Instruction(hil::Opcode::Return));
        main.addBlock(std::move(entry));
        module.addFunction(std::move(main));
    }
}

void Compiler::appendLineIR(std::string_view source, llcvm::Module& module) {
    llcvm::IRGenerator generator;
    std::istringstream lines{std::string(source)};
    std::string current;
    std::size_t line = 1;

    while (std::getline(lines, current)) {
        module.updateLine(generator.generateLine(current, line));
        ++line;
    }
}

bool Compiler::runSemanticChecks(const SyntaxNode& node, sema::Sema& sema) {
    return checkNode(node, sema);
}

} // namespace hyper::compiler
