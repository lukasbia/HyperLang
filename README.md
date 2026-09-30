# HyperLang

HyperLang is a high-level programming language using the `.hl` source extension.

## File Types

- `.hl` — HyperLang source files.
- `.hlf` — HyperLang custom function files. These are intended for large reusable functions stored separately from normal source files.

## Imports and Includes

`@import` and `@include` have different purposes.

- `@import` loads a HyperLang library such as `Foundation`, `Decimal`, `DataScience`, or `HyperUi`.
- `@include` loads a custom `.hlf` function file containing reusable functions.

Example:

```text
@import Foundation
@include Utilities.hlf

func main() {
    output.print("Hello from HyperLang")
}
```

HyperLang uses `{}` for blocks, dot notation for member and API access, and symbolic operators where appropriate. The compiler is written in C++ and is organized around the lexer, parser, semantic analysis, HIL, LSC, and MCM systems.
