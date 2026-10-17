// ========================================================
// 1. MACRO DEFINITIONS
// ========================================================
#define BUFFER_SIZE 1024
#define PI 3.14159
#define GREETING "Hello, RMc7!"
#define NEWLINE '\n'

// Multi-token mathematical expression
#define EXPR (10 + 20 * 2)

// Chained / Nested Macros (Macro inside another macro)
#define BASE_VAL 10
#define STEP_VAL (BASE_VAL * 2)
#define TOTAL_VAL (STEP_VAL + 5)

// Self-referencing macro (Infinite loop guard check)
#define x (x + 1)

// Mutual / Indirect recursion (Loop guard check)
#define ALPHA (BETA + 1)
#define BETA (ALPHA + 1)

// Empty macro
#define EMPTY_MODIFIER

// ========================================================
// 2. CODE INVOCATIONS (Where expansion should happen)
// ========================================================
int main() {
    // 1. Basic constants
    int size = BUFFER_SIZE;
    float pi_val = PI;
    char* str = GREETING;
    char nl = NEWLINE;

    // 2. Multi-token expression
    int calc = EXPR;

    // 3. Chained expansion (TOTAL_VAL -> STEP_VAL -> BASE_VAL)
    int chain = TOTAL_VAL;

    // 4. Self-referencing (Must expand ONCE to (x + 1) and STOP)
    int x = 5;
    int self_ref = x;

    // 5. Mutual recursion (ALPHA -> BETA -> ALPHA, must STOP)
    int mutual = ALPHA;

    // 6. Empty macro (EMPTY_MODIFIER should vanish)
    EMPTY_MODIFIER int empty_test = 100;

    // 7. Non-expansion checks (These must NOT be replaced!):
    //    - Inside string literals:
    char* literal_test = "BUFFER_SIZE is not a macro here";
    //    - As part of longer variable names:
    int BUFFER_SIZE_MAX = 2048;

    return 0;
}