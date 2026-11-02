#include "tests/test_macro_header.h"

// 1. Object-like macros (Constants, Chaining, Recursion guard)
#define BUFFER_SIZE 1024
#define CHAIN_BASE 5
#define CHAIN_EXP (CHAIN_BASE + 10)
#define x (x + 1)

// 2. Function-like macros (Basic, Nested parens, Multi-call)
#define ADD(a, b) a + b

// 3. Stringification (#)
#define TO_STR(s) #s

// 4. Token Pasting (##)
#define CONCAT(a, b) a ## b
#define MAKE_OP(op) op ## =

// 5. Variadic Macros (...)
#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)
#define VAR_TO_STR(...) #__VA_ARGS__

int main() {
    // Test 1: From included header
    int h_val = HEADER_CONST;
    int h_mult = MULT(3, 4);

    // Test 2: Object macros
    int buf = BUFFER_SIZE;
    int chain = CHAIN_EXP;
    int x = 5;

    // Test 3: Function macro multiple invocations (testing nested_set fix)
    int sum1 = ADD(10, 20);
    int sum2 = ADD(30, 40);

    // Test 4: Function macro with protected comma inside parens
    int pair = ADD((1, 2), 7);

    // Test 5: Stringification (#)
    char* s1 = TO_STR(hello_world);
    char* s2 = TO_STR("quoted");

    // Test 6: Token Pasting (##)
    int CONCAT(my_, var) = 100;
    int pasted_num = CONCAT(12, 34);
    int op_test = 10;
    op_test MAKE_OP(+) 5;

    // Test 7: Variadic Macros
    LOG("val: %d, %d", 1, 2);
    char* va_str = VAR_TO_STR(x, y, z);

    return 0;
}