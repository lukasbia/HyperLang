# HyperLang Compiler Architecture

HyperLang is implemented as a native compiler project. The repository follows a layered organization inspired by large production language compilers while keeping HyperLang terminology and implementation.

## Compilation pipeline

Source `.hl` files enter the compiler driver and move through these stages:

1. Driver
2. Lexer
3. Parser
4. Sema
5. HIL
6. HIL Optimization
7. LSC IR
8. LSC Machine Code
9. LSC Backend Optimization
10. CodeGen

MCM participates in lowering and ownership handling wherever object lifetime information is required.

## Source language

`.hl` is the normal HyperLang source extension. `.hlf` contains separately maintained custom functions. `@import` resolves libraries. `@include` resolves `.hlf` source. Blocks use braces and APIs use dot notation.

The lexer uses a compile-time hardcoded keyword and directive table. It does not discover or register language keywords dynamically at runtime.

## HIL

HIL means Hyper Intermediate Language. HIL is a first-class compiler IR with module, function, basic-block, value, type, instruction, constant, builder, verifier, and optimization infrastructure.

## HIL optimization

HIL optimization operates on verified HIL rather than source text. Passes are independent units so optimization can be enabled, disabled, ordered, and tested without changing the frontend.

## MCM

MCM means Memory Cleanup Management. It is HyperLang's automatic ownership and lifetime subsystem. It is designed around ARC-like behavior: objects have compiler-tracked ownership, retains and releases are inserted or eliminated from lifetime information, and destruction occurs when the compiler can prove the managed lifetime has ended.

MCM is not a thin alias for a C++ smart pointer. Its responsibility is to operate on HyperLang and HIL ownership semantics and produce explicit lifetime operations for later stages.

## LSC

LSC means Live Synchronized Compiler. LSC is HyperLang's compiler and backend system in place of LLVM. Its IR is separate from HIL. LSC lowers optimized HIL into a target-oriented representation and then lowers that representation into machine code.

LSC is incremental. A source edit invalidates only affected compilation units and dependent stages. Diagnostics are attached to source regions, so an error in one region can be reported without destroying valid state for unrelated regions. Invalid regions are not emitted as executable code until they become valid again.

## CodeGen

CodeGen is the final emission layer. It consumes validated output of the LSC backend and emits the selected target artifact.

## Build organization

Compiler areas have their own CMake entry points. Large implementation subsystems are kept in separate `.cpp` files rather than putting the implementation into headers. Headers define stable interfaces; implementation files own the algorithms and state transitions.
