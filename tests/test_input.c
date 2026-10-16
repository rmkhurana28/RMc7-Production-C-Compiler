#define max 128\
129

// #include "stdio.h"

// // ==========================================
// // 1. Basic Literals & Constants
// // ==========================================
// #define BUFFER_SIZE 1024
// #define PI 3.14159
// #define STATUS_OK 0
// #define APP_NAME "RMc7 Compiler"
// #define NEWLINE '\n'

// // ==========================================
// // 2. Expressions & Glued / Spaced Tokens
// // ==========================================
// #define GLUED 5*5
// #define SPACED 5 * 5
// #define CALC_AREA 2 * PI * 10
// #define COMPLEX_EXPR (5 + 3) * (10 - 2)

// // ==========================================
// // 3. Empty / Flag Macros
// // ==========================================
// #define DEBUG_MODE
// #define __USE_EXTENSIONS__

// // ==========================================
// // 4. Comments (Must NOT enter defMap)
// // ==========================================
// #define MAX_USERS 500 // maximum active sessions
// #define TIMEOUT /* wait duration */ 60 /* seconds */

// // ==========================================
// // 5. Multi-Statement Macros
// // ==========================================
// #define RESET_COORDS x = 0; y = 0;

// // ==========================================
// // 6. Identifier Variety & C Keywords
// // ==========================================
// #define _INTERNAL_FLAG 1
// #define __CUSTOM_SYS_H__ 1
// #define REG_32BIT 0x20
// #define VAR_A1_B2 99

// // C keywords as macro names (allowed in C preprocessor)
// #define goto 125
// #define inline
// #define auto 77

// // ==========================================
// // 7. Benign Redefinition (Exact same tokens)
// // ==========================================
// #define LIMIT 100
// #define LIMIT 100

// // ==========================================
// // 8. Line Continuation (\)
// // ==========================================
// #define LONG_FORMULA 10 + \
//                      20 + \
//                      30

// #define INIT_ARRAY { \
//     1, 2, 3, \
//     4, 5, 6  \
// }

// // Dummy main function so downstream parser has valid code to inspect
// int main() {
//     int x = 0;
//     int y = 0;
//     return 0;
// }

// 1. Multi-argument expression
// #define ADD(a, b) ((a) + (b))

// // 2. Zero-argument function macro
// #define GET_TEN() 10

// // 3. Empty-body function macro
// #define LOG(msg)

// int main() {
//     return 0;
// }

// ========================================================
// 1. Zero Arguments
// ========================================================
#define ZERO_ARGS() 42
#define EMPTY_ZERO_ARGS()

// ========================================================
// 2. Single Argument (Identity, Expressions & Stringizing)
// ========================================================
#define IDENTITY(x) x
#define SQUARE(x) ((x) * (x))
#define STRINGIFY(s) #s

// ========================================================
// 3. Multi-Arguments (2, 3, and 5 Parameters)
// ========================================================
#define ADD(a, b) ((a) + (b))
#define CLAMP(val, min_val, max_val) (((val) < (min_val)) ? (min_val) : (((val) > (max_val)) ? (max_val) : (val)))
#define SUM5(a, b, c, d, e) ((a) + (b) + (c) + (d) + (e))

// ========================================================
// 4. Empty Body with Arguments (e.g. Debug Logging)
// ========================================================
#define LOG_DEBUG(msg)
#define IGNORE_TWO(x, y)

// ========================================================
// 5. C Keywords as Names and Parameters
// ========================================================
#define inline(func) static inline func
#define TYPE_CONVERT(int, double) (int + double)

// ========================================================
// 6. Token Pasting (##) in Body
// ========================================================
#define CONCAT(x, y) x ## y
#define MAKE_VAR(prefix, id) prefix##_##id

// ========================================================
// 7. Multi-line Function Macro with Line Continuation (\)
// ========================================================
#define MULTI_LINE_MACRO(x, \
                         y) \
    ((x) * 10 + \
     (y) * 20)

// ========================================================
// 8. Comments (Inline & Trailing must NOT leak into body/args)
// ========================================================
#define COMMENTED(a /* first */, b /* second */) ((a) + (b)) // trailing comment

// ========================================================
// 9. Benign Redefinition (Exact same arguments and tokens)
// ========================================================
#define REPEAT_ME(x, y) ((x) ^ (y))
#define REPEAT_ME(x, y) ((x) ^ (y))

// ========================================================
// 10. Complex Statements & Blocks in Body
// ========================================================
#define SWAP(a, b) { int temp = a; a = b; b = temp; }

int main() {
    return 0;
}

// ========================================================
// NEGATIVE TESTS (Uncomment ONE by ONE to verify errors):
// ========================================================
// #define DUP_PARAM(x, x) (x + x)              // Error: Duplicate macro parameter: x
// #define NUM_PARAM(1, y) (1 + y)              // Error: Func Arg Error in macro
// #define OP_PARAM(x, +) (x + 1)               // Error: Func Arg Error in macro
// #define NO_COMMA(x y) (x + y)                // Error: Func define parameter error
// #define TRAILING_COMMA(a, b,) (a + b)        // Error: Func Arg Error in macro
// #define CONFLICT(x) (x + 1)
// #define CONFLICT(x) (x + 2)                  // Error: Redefinition of fnc macro

// =======================================================
// NEGATIVE TESTS (Uncomment ONE by ONE to verify errors):
// =======================================================
// #define __LINE__ 50       // Error: Cannot redefine built-in __LINE__
// #define defined 1         // Error: 'defined' cannot be used as macro name
// #define 123BAD 456        // Error: Invalid macro name starting with digit
// #define + 10              // Error: Operator '+' cannot be macro name
// #define LIMIT 200         // Error: Conflicting redefinition of LIMIT (was 100)




void func(){};