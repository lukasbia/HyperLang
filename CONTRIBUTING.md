# Contributing to HyperLang

HyperLang is an open-source programming language and compiler project.

## Development rules

- HyperLang source files use `.hl`.
- Custom function files use `.hlf`.
- `@import` loads libraries.
- `@include` loads `.hlf` custom function files.
- Language keywords are hardcoded in the compiler lexer.
- Compiler implementations belong in `.cpp` files; headers provide stable interfaces.
- New compiler stages must have tests.
- Do not add placeholder implementations or line-count filler.
- Keep the compiler pipeline explicit: Driver, Lexer, Parser, Sema, HIL, HIL Optimization, LSC IR, LSC Machine Code, LSC Backend Optimization, and CodeGen.
- MCM is the ARC-style automatic memory/lifetime subsystem.
- LSC is the live incremental compiler and native backend system.

## Changes

Build the project with CMake, run the test suite, and verify that generated output matches the selected target before opening a pull request.
