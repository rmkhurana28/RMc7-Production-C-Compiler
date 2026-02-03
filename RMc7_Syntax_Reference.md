# RMc7 Syntax Reference

**Version:** Phase 1 Complete | Phase 2 In Progress (~60%)  
**Target:** 90-95% ISO C Core Features

---

## Legend

- ✅ **Supported** - Fully implemented and tested
- ❌ **Not Supported** - Not planned for implementation
- 🟡 **In Progress** - Currently being implemented
- ➡️ **Planned** - Will be implemented soon

---

## 1. Declarations

### 1.1 Basic Type Specifiers
✅ `int`  
✅ `char`  
✅ `float`  
✅ `double`  
✅ `void`  
✅ `short`  
✅ `long`  
✅ `long long`  
✅ `signed`  
✅ `unsigned`  
🟡 `bool` (will be added for convenience)  
🟡 `_Bool`

**Examples:**
```c
int x;
unsigned long long count;
char ch;
double pi = 3.14159;
```

### 1.2 Storage Class Specifiers
✅ `auto`  
✅ `register`  
✅ `static`  
✅ `extern`  
➡️ `typedef`

**Examples:**
```c
static int counter;
extern double global_var;
register int fast_var;
```

### 1.3 Type Qualifiers
✅ `const`  
✅ `volatile`  
✅ `restrict`

**Examples:**
```c
const int MAX = 100;
volatile int hardware_port;
int *restrict ptr;
```

### 1.4 Multiple Declarations

✅ **Basic multiple declarations:**
```c
int a, b, c;
int *p, **pp, arr[10];
```

❌ **Private type qualifiers in subsequent declarators:**

**Design Rule:** In multiple declarations, subsequent declarators (after the first) cannot introduce their own private type qualifiers. Type qualifiers in the base type apply to all declarators. Standard pointer declarator rules apply.

**Supported:**
```c
int const a, b;           // Both a and b are const int (qualifier in base type)
int restrict* p, **q;     // p is int restrict*, q is int** (standard declarator rules)
int const* a, b;          // a is int const*, b is int (standard declarator rules)
```

**Not Supported:**
```c
int const a, const b;     // Cannot add const to second declarator - NOT ALLOWED
int *p, *restrict q;      // Cannot add restrict to second declarator - NOT ALLOWED
```

---

## 2. Declarators

### 2.1 Simple Declarators
✅ Variable declarations  
✅ Pointer declarations (single and multi-level)  
✅ Array declarations (single and multi-dimensional)  
✅ Function declarations

**Examples:**
```c
int x;                   // Simple variable
int *ptr;                // Pointer
int **pptr;              // Pointer to pointer
int arr[10];             // Array
int matrix[3][4];        // 2D array
int func(int a, int b);  // Function
```

### 2.2 Complex Declarators
✅ Pointers to arrays  
✅ Arrays of pointers  
✅ Function pointers  
✅ Arrays of function pointers  
✅ Multi-level nesting (up to 6 levels tested)

**Examples:**
```c
int (*ptr)[10];              // Pointer to array
int *arr[10];                // Array of pointers
int (*func_ptr)(int, int);   // Function pointer
int (*arr_func[5])(int);     // Array of function pointers

// Complex nesting:
int *(*(**ptr)[10])(int);    // Pointer to pointer to array of pointers to functions
```

### 2.3 Function Parameters
✅ Named parameters  
✅ Unnamed parameters  
✅ Variadic functions (`...`)  
✅ Empty parameter list `()`  
✅ `void` parameter list `(void)`

**Examples:**
```c
int add(int a, int b);           // Named parameters
int subtract(int, int);          // Unnamed parameters
int printf(const char *fmt, ...); // Variadic
int get_random(void);            // Explicit void
int legacy_func();               // Empty (old-style)
```

---

## 3. Structured Types

### 3.1 Structures
➡️ `struct` declarations  
➡️ `struct` definitions  
➡️ Nested structures  
➡️ Anonymous structures  
➡️ Structure member access (`.`)  
➡️ Structure pointer access (`->`)

### 3.2 Unions
➡️ `union` declarations  
➡️ `union` definitions  
➡️ Union member access

### 3.3 Enumerations
➡️ `enum` declarations  
➡️ `enum` definitions  
➡️ Explicit enumerator values  
➡️ Anonymous enums

---

## 4. Expressions

### 4.1 Primary Expressions
✅ Identifiers  
✅ Constants (integer, float, char, string)  
✅ Parenthesized expressions  
✅ `sizeof` operator  
✅ Type casts

**Examples:**
```c
x                    // Identifier
42                   // Integer constant
3.14                 // Float constant
'A'                  // Character constant
"Hello"              // String literal
(x + y)              // Parenthesized
sizeof(int)          // sizeof
(int)3.14            // Type cast
```

**Design Rules for `sizeof` and type casts:**

❌ **Nested brackets are NOT allowed inside the parentheses of `sizeof` and type cast operators:**
```c
// NOT SUPPORTED:
sizeof(int[10])      // Array brackets inside sizeof
sizeof(int(*)[10])   // Function/pointer brackets
(int[10])x           // NOT SUPPORTED - array in cast
```

**Supported:**
```c
sizeof(int)          // Simple type
sizeof(int *)        // Pointer type
(int)x               // Simple cast
(int *)ptr           // Pointer cast
```

**Not Supported:**
```c
sizeof(int[10])      // Brackets [] inside ()
sizeof(int(*)(int))  // Function pointer brackets
(int(*)[5])ptr       // Array pointer cast
sizeof x             // Parentheses are required - NOT ALLOWED
```

### 4.2 Arithmetic Operators
✅ `+` (addition)  
✅ `-` (subtraction)  
✅ `*` (multiplication)  
✅ `/` (division)  
✅ `%` (modulo)  
✅ `++` (increment, prefix and postfix)  
✅ `--` (decrement, prefix and postfix)  
✅ `+` (unary plus)  
✅ `-` (unary minus)

**Examples:**
```c
x + y
x - y
x * y
x / y
x % y
++x
x++
--x
x--
+x
-x
```

### 4.3 Relational Operators
✅ `<` (less than)  
✅ `<=` (less than or equal)  
✅ `>` (greater than)  
✅ `>=` (greater than or equal)  
✅ `==` (equal)  
✅ `!=` (not equal)

**Examples:**
```c
x < y
x <= y
x > y
x >= y
x == y
x != y
```

### 4.4 Logical Operators
✅ `&&` (logical AND)  
✅ `||` (logical OR)  
✅ `!` (logical NOT)

**Examples:**
```c
x && y
x || y
!x
```

### 4.5 Bitwise Operators
✅ `&` (bitwise AND)  
✅ `|` (bitwise OR)  
✅ `^` (bitwise XOR)  
✅ `~` (bitwise NOT)  
✅ `<<` (left shift)  
✅ `>>` (right shift)

**Examples:**
```c
x & y
x | y
x ^ y
~x
x << 2
x >> 3
```

### 4.6 Assignment Operators
✅ `=` (simple assignment)  
✅ `+=` `-=` `*=` `/=` `%=` (compound arithmetic)  
✅ `&=` `|=` `^=` (compound bitwise)  
✅ `<<=` `>>=` (compound shift)

**Examples:**
```c
x = 10
x += 5
x -= 3
x *= 2
x /= 4
x %= 3
x &= 0xFF
x |= 0x01
x ^= 0x0F
x <<= 2
x >>= 1
```

### 4.7 Other Operators
✅ `? :` (ternary/conditional)  
✅ `,` (comma)  
✅ `[]` (array subscript)  
✅ `.` (member access)  
✅ `->` (pointer member access)  
✅ `&` (address-of)  
✅ `*` (dereference)  
✅ `()` (function call)

**Examples:**
```c
x > 0 ? x : -x        // Ternary
x = 1, y = 2          // Comma
arr[5]                // Array subscript
point.x               // Member access
ptr->x                // Pointer member
&x                    // Address-of
*ptr                  // Dereference
func(a, b)            // Function call
```

🟡 **Ternary operator** - Currently has known bugs with complex nesting, fix in progress.

### 4.8 Operator Precedence
✅ All 15 precedence levels correctly implemented  
✅ Correct associativity (left-to-right, right-to-left)  
✅ Precedence climbing algorithm

**Precedence Table (highest to lowest):**
1. `()` `[]` `.` `->` (postfix `++` `--`)
2. (prefix `++` `--`) `+` `-` (unary) `!` `~` `*` (deref) `&` (address) `sizeof` `(cast)`
3. `*` `/` `%`
4. `+` `-` (binary)
5. `<<` `>>`
6. `<` `<=` `>` `>=`
7. `==` `!=`
8. `&` (bitwise)
9. `^`
10. `|`
11. `&&`
12. `||`
13. `?:`
14. `=` `+=` `-=` `*=` `/=` `%=` `&=` `|=` `^=` `<<=` `>>=`
15. `,`

---

## 5. Statements

### 5.1 Expression Statements
✅ Standalone expressions ending with `;`

**Examples:**
```c
x = 10;
func(a, b);
x++;
a + b;  // Valid but useless
```

### 5.2 Selection Statements
➡️ `if` statement  
➡️ `if-else` statement  
➡️ `switch` statement with `case` and `default`

### 5.3 Iteration Statements
➡️ `while` loop  
➡️ `do-while` loop  
➡️ `for` loop

### 5.4 Jump Statements
➡️ `break`  
➡️ `continue`  
➡️ `return`  
➡️ `goto`

### 5.5 Labeled Statements
➡️ Labels for `goto`  
➡️ `case` labels  
➡️ `default` label

### 5.6 Compound Statements
➡️ Blocks `{ }`  
➡️ Nested blocks  
➡️ Block-scoped declarations

---

## 6. Functions

✅ Function declarations  
✅ Function calls  
➡️ Function definitions  
➡️ Return statements  
➡️ Recursive functions

---

## 7. Preprocessor

➡️ **Planned after Phase 2**

The RMc7 preprocessor will be implemented after Phase 2.  
The goal is to support full ISO C preprocessing, including:

### Planned Features
➡️ #define object-like macros  
➡️ #define function-like macros  
➡️ Macro expansion and rescan  
➡️ #include with full system and local search paths  
➡️ #if / #ifdef / #ifndef / #elif / #else / #endif  
➡️ Stringizing (#) and token pasting (##)  
➡️ Multi-line macros using backslash continuation  
➡️ #line directive  
➡️ Built-in macros (__FILE__, __LINE__, etc.)

### Design Notes
- Preprocessing will occur before lexical analysis and produce a normalized token stream.  
- #if constant-expression evaluation will reuse the main expression parser.  
- No GNU or MSVC extensions are planned for Phase 1 of the preprocessor.

---

## 8. Testing Status

### Completed Tests (620+ test cases, 100% pass rate)

- ✅ **Type System Validation:** 87 tests
- ✅ **Declarator Parsing:** 100+ tests (simple pointers, arrays, complex nesting, function pointers)
- ✅ **Function Parameters:** 50+ tests (named, unnamed, variadic)
- ✅ **Expression Parsing:** ~370 tests (all operators, precedence, associativity)
- ✅ **Type Cast & Sizeof:** ~370 tests

---

## 9. Error Handling

➡️ **Planned after Phase 2**

RMc7 will include a dedicated diagnostic system added after Phase 2.

### Planned Features
➡️ Error messages will always include line and column numbers  
➡️ Parser will stop or recover depending on the severity  
➡️ Error reporting will be integrated into both parsing and semantic phases

---

## 10. Compliance Target

**Target:** 90-95% ISO C core features

**Current Coverage (Parser Phase):** ~60%
- ✅ Complete type system
- ✅ Complete declarator system
- ✅ Complete expression system
- 🟡 Statement system (in progress)
- ➡️ Function definitions
- ➡️ Preprocessor

---

## 11. Implementation Timeline

**Completed:**
- ✅ **Phase 1:** Lexical Analysis (100%)
- ✅ **Phase 2 (60%):** Type system, declarators, expressions

**In Progress:**
- 🟡 Ternary operator bug fix
- 🟡 Statement parsing

**Upcoming:**
- ➡️ Control flow statements (if, while, for, switch)
- ➡️ Function definitions
- ➡️ Preprocessor
- ➡️ Semantic analysis
- ➡️ IR generation
- ➡️ Optimization
- ➡️ Code generation

---

**Note:** This document reflects the current state of the RMc7 compiler and will be updated as new features are implemented.
