# RMc7 - Level-7 Production C Compiler

A complete C compiler implementation in C++ targeting 90-95% ISO C coverage.

---

## 🎯 Project Overview

**RMc7** is a full-specification C compiler designed to handle production-level C code with advanced optimization capabilities.

**Developer:** Ridham Khurana  
**Language:** C++ (C++17)  
**Target:** x86-64 Assembly  
**Level:** 7 (Full-Spec Production Compiler)

---

## 📋 Supported Features

### **Data Types**
- [x] `int`
- [x] `double`
- [x] `float`
- [x] `char`
- [x] `bool`
- [x] `void`
- [x] `short`
- [x] `long`
- [x] `unsigned`
- [x] `signed`

### **Control Flow**
- [x] `if` / `else`
- [x] `while`
- [x] `do-while`
- [x] `for`
- [x] `switch` / `case` / `default`
- [x] `break`
- [x] `continue`
- [x] `return`
- [x] `goto`

### **Data Structures**
- [x] `struct`
- [x] `union`
- [x] `enum`
- [x] Arrays
- [x] Pointers (single and multi-level)
- [x] `typedef`

### **Storage Classes & Qualifiers**
- [x] `static`
- [x] `extern`
- [x] `const`
- [x] `auto`
- [x] `register`
- [x] `volatile`

### **Operators**

**Arithmetic:**
- [x] `+` `-` `*` `/` `%`
- [x] `++` `--`

**Relational:**
- [x] `<` `<=` `>` `>=` `==` `!=`

**Logical:**
- [x] `&&` `||` `!`

**Bitwise:**
- [x] `&` `|` `^` `~` `<<` `>>`

**Assignment:**
- [x] `=` `+=` `-=` `*=` `/=` `%=` `&=` `|=` `^=` `<<=` `>>=`

**Other:**
- [x] `?:` (ternary)
- [x] `.` (member access)
- [x] `->` (pointer member access)
- [x] `sizeof`

### **Preprocessor Directives**
- [x] `#include`
- [x] `#define`
- [x] `#ifdef` / `#ifndef`
- [x] `#if` / `#elif` / `#else` / `#endif`
- [x] `#undef`
- [x] `#pragma`

### **Advanced Features**
- [x] Functions (declaration, definition, calls, recursion)
- [x] Dynamic memory allocation (`malloc`, `calloc`, `realloc`, `free`)
- [x] Multi-file compilation and linking
- [x] String literals
- [x] Character literals
- [x] Escape sequences (`\n`, `\t`, `\\`, etc.)
- [x] Comments (`//` and `/* */`)

---

## 🏗️ Compiler Architecture

### **Pipeline Phases:**
1. **Lexical Analysis** - Tokenization (In Progress)
2. **Syntax Analysis** - Parsing & AST generation
3. **Semantic Analysis** - Type checking & symbol tables
4. **Intermediate Code Generation** - Three-Address Code
5. **Optimization** - Multiple optimization passes
6. **Code Generation** - x86-64 Assembly output
7. **Assembly & Linking** - Executable generation

---

## 🚀 Current Status

**Phase 1 - Lexical Analysis:** ✅ COMPLETE
- Full tokenization for all C syntax (~140 token types)
- All operators (single & multi-character)
- String/char literals with escape sequences
- Number literals (int, float, double)
- Comments (single & multi-line)
- Line/column tracking

**Phase 2 - Syntax Analysis (Parser):** 🟡 IN PROGRESS (~35%)
- ✅ Type collection algorithm (storage classes, qualifiers, sign/size modifiers)
- ✅ Type validation system with compatibility rules (87 test cases, 100% pass rate)
- ✅ Typedef expansion (recursive resolution)
- ✅ Declarator parsing (variables, arrays, pointers, function pointers)
- ✅ Complex declarator patterns (multi-level nesting, deferred star application)
- ✅ Function parameter evaluation (named/unnamed, variadic functions)
- ✅ Syntax validation (variable vs function classification, invalid pattern detection)
- ⏭️ Next: Expression parsing with operator precedence
- ⏭️ Next: Statement parsing (control flow)

---

## 📊 Language Coverage Target

**Target:** 95-100% of ISO C core features

---

## 🛠️ Build Instructions

```bash
# Compile the project
make

# Run the compiler
./rmc7

# Clean build artifacts
make clean
```

---

## 📁 Project Structure

```
RMc7/
├── src/
│   ├── lexer/
│   │   ├── Token.h          # Token definitions (~140 types)
│   │   ├── Lexer.h          # Lexer interface
│   │   └── Lexer.cpp        # Lexer implementation (complete)
│   ├── parser/
│   │   ├── Parser.h         # Parser interface
│   │   ├── Parser.cpp       # Parser implementation (in progress)
│   │   ├── Helper.h         # Type system utilities
│   │   ├── Helper.cpp       # Type validation & typedef expansion
│   │   ├── ASTNode.h/cpp    # Base AST node class
│   │   ├── DeclarationNode.h/cpp    # Declaration AST nodes
│   │   ├── ExpressionNode.h/cpp     # Expression AST nodes
│   │   └── StatementNode.h/cpp      # Statement AST nodes
│   └── Main.cpp             # Entry point
├── tests/
│   └── test_input.c         # Test C programs
├── Makefile                 # Build automation
├── dictionary.txt           # Character & syntax reference
└── README.md               # This file
```

---

## 📝 License

**Educational Portfolio License**

Copyright © 2026 Ridham Khurana (RM)

This software is provided as a portfolio demonstration and educational resource.

**Permitted Uses:**
- View, clone, and run this code for evaluation and review purposes
- Study the implementation to learn compiler design concepts and techniques
- Test functionality and modify test cases locally for assessment
- Use for recruitment, hiring, interview evaluation, and skills assessment
- Reference specific implementations in technical discussions with proper attribution

**Prohibited Uses:**
- Incorporating this code into your own projects, products, or portfolio
- Submitting this code (modified or unmodified) as part of academic coursework
- Redistributing or publishing this code or derivative works
- Using this code for commercial purposes without explicit permission
- Presenting this work as your own or without proper attribution to the author

**Note:** This code is shared to demonstrate the author's technical capabilities 
and to serve as an educational resource for those learning compiler design. 
For any use case beyond evaluation and learning, please contact the author.

**Contact:** khurana.ridham222@gmail.com

---

**Status:** 🚧 Active Development
