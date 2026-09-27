import Foundation

// MARK: - HyperLang Native Shell (shell.swift)

func runPrompt() {
    print("HyperLang Interactive Shell (Swift)")
    print("Type your code or 'exit' to quit.\n")
    
    while true {
        print("HyperLang> ", terminator: "")
        guard let line = readLine(), line != "exit" else {
            print("Goodbye!")
            break
        }
        
        if line.trimmingCharacters(in: .whitespaces).isEmpty {
            continue
        }
        
        executeSource(line)
    }
}

func runFile(path: String) {
    do {
        let sourceCode = try String(contentsOfFile: path, encoding: .utf8)
        executeSource(sourceCode)
    } catch {
        print("Error: Could not read file at '\(path)': \(error)")
    }
}

func executeSource(_ source: String) {
    do {
        // 1. Lexing
        let lexer = Lexer(source: source)
        let tokens = try lexer.tokenize()
        
        // 2. Parsing
        let parser = Parser(tokens: tokens)
        let ast = try parser.parse()
        
        // 3. Semantic Analysis
        let analyzer = SemanticAnalyzer()
        let errors = try analyzer.analyze(declarations: ast)
        
        if !errors.isEmpty {
            for error in errors {
                print(error)
            }
            return
        }
        
        // 4. Code Generation (Transpile to Swift)
        let config = TranspilerConfig(targetLanguage: .swift)
        let transpiler = Transpiler(ast: ast, config: config)
        let output = transpiler.generate()
        
        print("\n--- Transpiled Swift Output ---")
        print(output)
        print("-------------------------------\n")
        
    } catch {
        print("Compilation Error: \(error)")
    }
}

// MARK: - Entry Point
let arguments = CommandLine.arguments

if arguments.count > 1 {
    // Run file mode: swift shell.swift source.hl
    runFile(path: arguments[1])
} else {
    // Run interactive REPL mode: swift shell.swift
    runPrompt()
}
