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

**Phase 1 - Lexical Analysis:** In Development  
- Token definitions complete (~140 token types)
- Lexer architecture designed
- Helper functions implemented
- Token output system functional

**Next:** Complete lexer implementation for all token types

---

## 📊 Language Coverage Target

- **Current (RMc4):** ~23% of C language
- **Target (RMc7):** 95-100% of ISO C core features

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
│   │   ├── Token.h          # Token definitions
│   │   ├── Lexer.h          # Lexer interface
│   │   └── Lexer.cpp        # Lexer implementation
│   └── Main.cpp             # Entry point
├── tests/
│   └── test_input.c         # Test C programs
├── Makefile                 # Build automation
├── dictionary.txt           # Character & syntax reference
└── README.md               # This file
```

---

## 📝 License

Educational project - Thapar Institute of Engineering & Technology

---

**Status:** 🚧 Active Development  
**Last Updated:** 14 December 2025
