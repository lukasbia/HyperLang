# HyperLang Keywords

HyperLang has exactly 97 language keywords. `#` is not part of the keyword system. `@` is used only where the language needs a directive-style construct.

## Import and include

1. `@include`
2. `@import`

`@import` loads libraries. `@include` loads custom `.hlf` function files.

## Core language

3. `func`
4. `else`
5. `if`
6. `then`
7. `endif`
8. `while`
9. `true`
10. `false`
11. `do`
12. `loop`
13. `endLoop`
14. `let`
15. `var`
16. `string`
17. `panic`
18. `input`
19. `output`
20. `init`
21. `deinit`
22. `int`
23. `num`
24. `enum`
25. `nil`

## Swift-inspired language features

26. `class`
27. `struct`
28. `protocol`
29. `extension`
30. `typealias`
31. `guard`
32. `switch`
33. `case`
34. `default`
35. `for`
36. `in`
37. `break`
38. `continue`
39. `return`
40. `defer`
41. `throw`
42. `throws`
43. `catch`
44. `async`
45. `await`
46. `some`
47. `any`
48. `self`
49. `where`
50. `get`
51. `set`
52. `mutating`
53. `static`
54. `final`
55. `private`
56. `public`
57. `internal`
58. `operator`
59. `subscript`
60. `associatedtype`
61. `required`
62. `convenience`
63. `override`
64. `weak`
65. `unowned`
66. `borrow`
67. `consume`
68. `yield`
69. `macro`
70. `attribute`

## HyperLang compiler and runtime features

71. `module`
72. `package`
73. `namespace`
74. `source`
75. `file`
76. `function`
77. `property`
78. `event`
79. `signal`
80. `asynclet`
81. `actor`
82. `task`
83. `detach`
84. `isolated`
85. `nonisolated`
86. `sendable`
87. `move`
88. `copy`
89. `weakref`
90. `strongref`
91. `own`
92. `shared`
93. `observe`
94. `synchronize`
95. `compile`
96. `extern`
97. `@` directive namespace

The final entry represents the reserved `@` syntax family rather than a standalone word. Current concrete directives are `@include` and `@import`.
