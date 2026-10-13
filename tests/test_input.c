#define max 128\
129

// #include "stdio.h"

// ==========================================
// 1. Basic Literals & Constants
// ==========================================
#define BUFFER_SIZE 1024
#define PI 3.14159
#define STATUS_OK 0
#define APP_NAME "RMc7 Compiler"
#define NEWLINE '\n'

// ==========================================
// 2. Expressions & Glued / Spaced Tokens
// ==========================================
#define GLUED 5*5
#define SPACED 5 * 5
#define CALC_AREA 2 * PI * 10
#define COMPLEX_EXPR (5 + 3) * (10 - 2)

// ==========================================
// 3. Empty / Flag Macros
// ==========================================
#define DEBUG_MODE
#define __USE_EXTENSIONS__

// ==========================================
// 4. Comments (Must NOT enter defMap)
// ==========================================
#define MAX_USERS 500 // maximum active sessions
#define TIMEOUT /* wait duration */ 60 /* seconds */

// ==========================================
// 5. Multi-Statement Macros
// ==========================================
#define RESET_COORDS x = 0; y = 0;

// ==========================================
// 6. Identifier Variety & C Keywords
// ==========================================
#define _INTERNAL_FLAG 1
#define __CUSTOM_SYS_H__ 1
#define REG_32BIT 0x20
#define VAR_A1_B2 99

// C keywords as macro names (allowed in C preprocessor)
#define goto 125
#define inline
#define auto 77

// ==========================================
// 7. Benign Redefinition (Exact same tokens)
// ==========================================
#define LIMIT 100
#define LIMIT 100

// ==========================================
// 8. Line Continuation (\)
// ==========================================
#define LONG_FORMULA 10 + \
                     20 + \
                     30

#define INIT_ARRAY { \
    1, 2, 3, \
    4, 5, 6  \
}

// Dummy main function so downstream parser has valid code to inspect
int main() {
    int x = 0;
    int y = 0;
    return 0;
}

// =======================================================
// NEGATIVE TESTS (Uncomment ONE by ONE to verify errors):
// =======================================================
// #define __LINE__ 50       // Error: Cannot redefine built-in __LINE__
// #define defined 1         // Error: 'defined' cannot be used as macro name
// #define 123BAD 456        // Error: Invalid macro name starting with digit
// #define + 10              // Error: Operator '+' cannot be macro name
// #define LIMIT 200         // Error: Conflicting redefinition of LIMIT (was 100)




void func(){};