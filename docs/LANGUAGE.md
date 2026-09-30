# HyperLang Language Rules

## Source extensions

`.hl` is the normal HyperLang source file extension.

`.hlf` is the custom function file extension. HLF files hold reusable, potentially large functions that can be included from HyperLang source.

## Import versus include

`@import` and `@include` are intentionally separate language features.

`@import` is for libraries:

```text
@import Foundation
@import Decimal
@import DataScience
@import HyperUi
```

`@include` is for custom function files:

```text
@include MathFunctions.hlf
```

The compiler should resolve these two forms through different source-management paths.

## Syntax

HyperLang uses `{` and `}` to delimit blocks.

Dot notation is the normal way to access members, namespaces, library APIs, and related functionality:

```text
Foundation.print("Hello")
object.value
object.method()
```

Symbols are used for operators while readable keyword names are used for language constructs.

## Compiler architecture

- HIL — Hyper Intermediate Language.
- LSC — Live Synchronized Compiler.
- MCM — Memory Cleanup Management, providing ARC-style automatic lifetime management.
