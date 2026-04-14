// =============================================================================
// COMPREHENSIVE C PARSER TEST SUITE - ALL SUPPORTED FEATURES
// =============================================================================
// This file tests ~90% of the ISO C core features supported by RMc7

// =============================================================================
// SECTION 1: BASIC TYPE SPECIFIERS
// =============================================================================

int x1;
char x2;
float x3;
double x4;
void x5_func(void);
short x6;
long x7;
long long x8;
signed x9;
unsigned x10;

// =============================================================================
// SECTION 2: STORAGE CLASS SPECIFIERS
// =============================================================================

auto int s1;
register int s2;
static int s3;
extern int s4;
const int s5 = 100;
volatile int s6;
static const int s7;
extern volatile int s8;

// =============================================================================
// SECTION 3: TYPE QUALIFIERS (No Duplicates)
// =============================================================================

const int q1;
volatile int q2;
const volatile int q3;
volatile const int q4;
int *const q5;
const int *q6;

// =============================================================================
// SECTION 4: MULTIPLE DECLARATIONS
// =============================================================================

int m1, m2, m3;
int *m4, **m5, m6;
int m7[10], m8, *m9;
float m10, m11[20], *m12;

// =============================================================================
// SECTION 5: SIMPLE DECLARATORS
// =============================================================================

int d1;
int *d2;
int **d3;
int ***d4;
int d5[10];
int d6[5][10];
int d7[2][3][4];
int d8(int, int);

// =============================================================================
// SECTION 6: COMPLEX DECLARATORS - POINTERS
// =============================================================================

int *c1;
int **c2;
int ***c3;
int ****c4;
int *****c5;

// =============================================================================
// SECTION 7: COMPLEX DECLARATORS - ARRAYS
// =============================================================================

int a1[10];
int a2[5][10];
int a3[2][3][4];
int a4[1][2][3][4];

// =============================================================================
// SECTION 8: COMPLEX DECLARATORS - POINTERS TO ARRAYS
// =============================================================================

int (*p1)[10];
int (*p2)[5][10];
int (*p3)[2][3][4];

// =============================================================================
// SECTION 9: COMPLEX DECLARATORS - ARRAYS OF POINTERS
// =============================================================================

int *ap1[10];
int **ap2[5];
int ***ap3[3];

// =============================================================================
// SECTION 10: COMPLEX DECLARATORS - FUNCTION POINTERS
// =============================================================================

int (*fp1)(int);
int (*fp2)(int, int);
int (*fp3)(int, int, int);
void (*fp4)(void);
int (*fp5)(void);

// =============================================================================
// SECTION 11: COMPLEX DECLARATORS - ARRAYS OF FUNCTION POINTERS
// =============================================================================

int (*afp1[5])(int);
int (*afp2[10])(int, int);
void (*afp3[3])(void);

// =============================================================================
// SECTION 12: MULTI-LEVEL NESTING (Deep Declarators)
// =============================================================================

int (*(*ml1)[10])(int);
int (*(*ml2[5])[10])(int);
int (*(*(*ml3)[5])[10])(int);
int *(*(*ml4)[10])[20];
int (*(*(*(*ml5)[5])[10])[20])(int);

// =============================================================================
// SECTION 13: TYPEDEF BASICS
// =============================================================================

typedef int myint;
typedef float myfloat;
typedef double mydouble;
typedef char mychar;
typedef int *intptr;
typedef int **intptrptr;

myint t1;
myfloat t2;
intptr t3;

// =============================================================================
// SECTION 14: TYPEDEF WITH COMPLEX TYPES
// =============================================================================

typedef int *IP;
typedef int **IPP;
typedef int (*FP)(int);
typedef int (*AFP[10])(int);
typedef int (*ArrPtr)[10];

IP td1;
IPP td2;
FP td3;
AFP td4;
ArrPtr td5;

// =============================================================================
// SECTION 15: STRUCT - SIMPLE DEFINITIONS
// =============================================================================

struct Point {
    int x;
    int y;
};

struct Point pt1;
struct Point *pt2;

// =============================================================================
// SECTION 16: STRUCT - INLINE DEFINITIONS WITH VARIABLES
// =============================================================================

struct Color {
    int r;
    int g;
    int b;
} col1;

struct Rectangle {
    struct Point p1;
    struct Point p2;
} rect1;

// =============================================================================
// SECTION 17: STRUCT - ANONYMOUS STRUCTS
// =============================================================================

struct {
    int id;
    char name[50];
} employee1;

struct {
    float salary;
    int bonus;
} payment1;

// =============================================================================
// SECTION 18: STRUCT - SELF-REFERENCING (Linked Lists, Trees)
// =============================================================================

struct Node {
    int data;
    struct Node *next;
};

struct Node node1;
struct Node *node_ptr;

struct Tree {
    int value;
    struct Tree *left;
    struct Tree *right;
};

// =============================================================================
// SECTION 19: STRUCT - FORWARD DECLARATIONS
// =============================================================================

struct LinkedList;
struct LinkedList *list_ptr;

// =============================================================================
// SECTION 20: UNION BASICS
// =============================================================================

union Data {
    int i;
    float f;
    char c;
};

union Data data1;
union Data *data_ptr;

// =============================================================================
// SECTION 21: UNION - INLINE DEFINITIONS
// =============================================================================

union Value {
    int int_val;
    float float_val;
} val1;

// =============================================================================
// SECTION 22: UNION - ANONYMOUS UNIONS
// =============================================================================

union {
    int x;
    float y;
} u_anon1;

// =============================================================================
// SECTION 23: UNION - SELF-REFERENCING AND NESTING
// =============================================================================

union LinkedValue {
    int data;
    union LinkedValue *next;
};

union U1 {
    int a;
    struct Point p;
};

// =============================================================================
// SECTION 24: FUNCTION DECLARATIONS - SIMPLE
// =============================================================================

int add(int a, int b);
float divide(float x, float y);
void print_int(int num);
char get_char(void);
int *return_ptr(void);

// =============================================================================
// SECTION 25: FUNCTION DECLARATIONS - WITH COMPLEX PARAMETERS
// =============================================================================

int func_ptr_param(int (*callback)(int));
int array_ptr_param(int (*arr)[10]);
void complex_param(int (*(*arr)[5])(int));

// =============================================================================
// SECTION 26: FUNCTION DECLARATIONS - VARIADIC FUNCTIONS
// =============================================================================

int printf_like(const char *fmt, ...);
int variadic_func(int first, ...);

// =============================================================================
// SECTION 27: FUNCTION DECLARATIONS - FUNCTION POINTERS AS RETURN TYPE
// =============================================================================

int (*return_func_ptr(void))(int);
int (*(*return_complex(void))[10])(int);

// =============================================================================
// SECTION 28: FUNCTION DEFINITIONS - SIMPLE
// =============================================================================

int main(void) {
    return 0;
}

int simple_add(int a, int b) {
    return a + b;
}

float simple_mul(float x, float y) {
    return x * y;
}

void simple_print(int n) {
    return;
}

// =============================================================================
// SECTION 29: FUNCTION DEFINITIONS - RECURSIVE
// =============================================================================

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// =============================================================================
// SECTION 30: EXPRESSION - PRIMARY EXPRESSIONS
// =============================================================================

int exp_primary(void) {
    int x = 42;
    float f = 3.14;
    char c = 'A';
    int sz = sizeof(int);
    int casted = (int)3.14;
    return x;
}

// =============================================================================
// SECTION 31: EXPRESSION - ARITHMETIC OPERATORS
// =============================================================================

int expr_arithmetic(int a, int b) {
    int add = a + b;
    int sub = a - b;
    int mul = a * b;
    int div = a / b;
    int mod = a % b;
    int inc = ++a;
    int dec = --b;
    int post_inc = a++;
    return add + sub + mul + div + mod + inc + dec;
}

// =============================================================================
// SECTION 32: EXPRESSION - RELATIONAL OPERATORS
// =============================================================================

int expr_relational(int x, int y) {
    int lt = x < y;
    int le = x <= y;
    int gt = x > y;
    int ge = x >= y;
    int eq = x == y;
    int ne = x != y;
    return eq && ne;
}

// =============================================================================
// SECTION 33: EXPRESSION - LOGICAL OPERATORS
// =============================================================================

int expr_logical(int p, int q, int r) {
    int and = p && q;
    int or = p || q;
    int not = !r;
    return and || or || not;
}

// =============================================================================
// SECTION 34: EXPRESSION - BITWISE OPERATORS
// =============================================================================

int expr_bitwise(int x, int y) {
    int and = x & y;
    int or = x | y;
    int xor = x ^ y;
    int not = ~x;
    int lshift = x << 2;
    int rshift = y >> 1;
    return and | or ^ xor;
}

// =============================================================================
// SECTION 35: EXPRESSION - ASSIGNMENT OPERATORS
// =============================================================================

int expr_assignment(int x) {
    x = 10;
    x += 5;
    x -= 3;
    x *= 2;
    x /= 4;
    x %= 3;
    x &= 0xFF;
    x |= 0x01;
    x ^= 0x0F;
    x <<= 1;
    x >>= 2;
    return x;
}

// =============================================================================
// SECTION 36: EXPRESSION - TERNARY AND COMMA
// =============================================================================

int expr_ternary(int x) {
    int max = x > 0 ? x : -x;
    int result = (x = 1, x + 2);
    return max + result;
}

// =============================================================================
// SECTION 37: EXPRESSION - ARRAY SUBSCRIPT AND MEMBER ACCESS
// =============================================================================

int expr_member_access(void) {
    int arr[10];
    struct Point p;
    struct Point *ptr = &p;

    arr[0] = 5;
    p.x = 10;
    p.y = 20;
    ptr->x = 15;

    return arr[0] + p.x + ptr->y;
}

// =============================================================================
// SECTION 38: EXPRESSION - ADDRESS-OF AND DEREFERENCE
// =============================================================================

int expr_ptr_ops(int x) {
    int *ptr = &x;
    int val = *ptr;
    *ptr = 20;
    return val + *ptr;
}

// =============================================================================
// SECTION 39: EXPRESSION - FUNCTION CALL
// =============================================================================

int expr_func_call(void) {
    int result = add(5, 3);
    float f_result = divide(10.0, 2.0);
    return result;
}

// =============================================================================
// SECTION 40: OPERATOR PRECEDENCE AND ASSOCIATIVITY
// =============================================================================

int precedence_test(int a, int b, int c) {
    int r1 = a + b * c;
    int r2 = (a + b) * c;
    int r3 = a < b ? c + 1 : c - 1;
    int r4 = a || b && c;
    int r5 = a + b << c;
    int r6 = a * b + c / 2 - 1;
    return r1 + r2 + r3 + r4 + r5 + r6;
}

// =============================================================================
// SECTION 41: STATEMENTS - SELECTION (if/else)
// =============================================================================

int stmt_if(int x) {
    if (x > 10) {
        return x;
    }
    return 0;
}

int stmt_if_else(int x) {
    if (x > 0) {
        return x;
    } else {
        return -x;
    }
}

int stmt_nested_if(int x, int y) {
    if (x > 0) {
        if (y > 0) {
            return x + y;
        }
        return x;
    }
    return y;
}

// =============================================================================
// SECTION 42: STATEMENTS - SELECTION WITHOUT BRACES
// =============================================================================

int stmt_if_no_braces(int x) {
    if (x > 10)
        return x;
    return 0;
}

int stmt_if_else_no_braces(int x) {
    if (x > 0)
        return x;
    else
        return -x;
}

// =============================================================================
// SECTION 43: STATEMENTS - SWITCH/CASE/DEFAULT
// =============================================================================

int stmt_switch(int x) {
    switch (x) {
        case 1:
            return 10;
        case 2:
            return 20;
        case 3:
        case 4:
            return 30;
        default:
            return 0;
    }
    return 0;
}

int switch_with_break(int x) {
    switch (x) {
        case 1:
            x = x + 10;
            break;
        case 2:
            x = x + 20;
            break;
        default:
            x = 0;
    }
    return x;
}

// =============================================================================
// SECTION 44: STATEMENTS - WHILE LOOP
// =============================================================================

int stmt_while(int x) {
    int sum = 0;
    while (x > 0) {
        sum = sum + x;
        x = x - 1;
    }
    return sum;
}

int while_no_braces(int x) {
    int sum = 0;
    while (x > 0)
        sum = sum + x;
    return sum;
}

// =============================================================================
// SECTION 45: STATEMENTS - DO-WHILE LOOP
// =============================================================================

int stmt_do_while(int x) {
    int count = 0;
    do {
        count++;
        x--;
    } while (x > 0);
    return count;
}

int do_while_no_braces(int x) {
    int count = 0;
    do
        count++;
    while (--x > 0);
    return count;
}

// =============================================================================
// SECTION 46: STATEMENTS - FOR LOOP (Declaration Init)
// =============================================================================

int stmt_for_decl(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum = sum + i;
    }
    return sum;
}

// =============================================================================
// SECTION 47: STATEMENTS - FOR LOOP (Expression Init)
// =============================================================================

int stmt_for_expr(int start) {
    int sum = 0;
    for (int i = start; i < 20; i = i + 2) {
        sum = sum + i;
    }
    return sum;
}

// =============================================================================
// SECTION 48: STATEMENTS - FOR LOOP (Empty Components)
// =============================================================================

int for_empty_all(void) {
    int i = 0;
    for (;;) {
        if (i >= 5) break;
        i++;
    }
    return i;
}

int for_empty_cond(int start) {
    int i = start;
    for (int x = 0; ; x++) {
        if (x > 5) break;
        i++;
    }
    return i;
}

int for_empty_incr(void) {
    int sum = 0;
    for (int i = 0; i < 10;) {
        sum = sum + i;
        i = i + 1;
    }
    return sum;
}

// =============================================================================
// SECTION 49: STATEMENTS - FOR LOOP NO BRACES
// =============================================================================

int for_no_braces(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++)
        sum = sum + i;
    return sum;
}

// =============================================================================
// SECTION 50: STATEMENTS - BREAK AND CONTINUE
// =============================================================================

int stmt_break(int x) {
    int sum = 0;
    while (x > 0) {
        if (x == 5) break;
        sum = sum + x;
        x--;
    }
    return sum;
}

int stmt_continue(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) continue;
        sum = sum + i;
    }
    return sum;
}

// =============================================================================
// SECTION 51: STATEMENTS - RETURN
// =============================================================================

int stmt_return_void(void) {
    return;
}

int stmt_return_value(int x) {
    return x + 1;
}

int stmt_return_multi(int x) {
    if (x > 0) {
        return x;
    } else if (x < 0) {
        return -x;
    }
    return 0;
}

// =============================================================================
// SECTION 52: STATEMENTS - GOTO AND LABELS
// =============================================================================

int stmt_goto(int x) {
    if (x > 10) goto end;
    x = x + 1;
    goto skip;
    x = 0;
    skip:
    x = x + 1;
    end:
    return x;
}

// =============================================================================
// SECTION 53: STATEMENTS - NESTED BLOCKS
// =============================================================================

int stmt_nested_blocks(int x) {
    {
        {
            {
                x = x + 1;
            }
        }
    }
    return x;
}

// =============================================================================
// SECTION 54: COMPLEX EXPRESSIONS IN STATEMENTS
// =============================================================================

int complex_expr_stmt(int a, int b, int c) {
    int x = a + b * c - (a / b) % c;
    int y = x > 0 ? x + 1 : x - 1;
    int z = a & b | c ^ a;
    return x + y + z;
}

// =============================================================================
// SECTION 55: TYPEDEF STRUCT
// =============================================================================

typedef struct {
    int x;
    int y;
} Point_t;

typedef struct {
    int r;
    int g;
    int b;
} Color_t;

Point_t pt = {0, 0};
Color_t col = {0, 0, 0};

// =============================================================================
// SECTION 56: TYPEDEF UNION
// =============================================================================

typedef union {
    int i;
    float f;
} Data_t;

Data_t data = {0};

// =============================================================================
// SECTION 57: TYPEDEF WITH POINTERS AND ARRAYS
// =============================================================================

typedef int *IntPtr;
typedef int IntArray[10];
typedef int IntArray2D[5][10];

IntPtr iptr = 0;
IntArray iarr = {0};

// =============================================================================
// SECTION 58: COMPLEX TYPEDEF COMBINATIONS
// =============================================================================

typedef int (*FuncPtrType)(int, int);
typedef int (*ArrayFuncPtr[10])(int);
typedef int (*(*ComplexPtr)[10])(int);

FuncPtrType fp = 0;
ArrayFuncPtr afp = {0};

// =============================================================================
// SECTION 59: STRUCT WITH POINTERS AND ARRAYS
// =============================================================================

struct ComplexStruct {
    int *ptr;
    int arr[10];
    int (*arr_ptr)[5];
    int (*func_ptr)(int);
    struct ComplexStruct *self_ref;
};

struct ComplexStruct cs;

// =============================================================================
// SECTION 60: UNION WITH STRUCT AND COMPLEX TYPES
// =============================================================================

union ComplexUnion {
    int i;
    struct Point p;
    int arr[10];
    int *ptr;
    int (*func_ptr)(int);
};

union ComplexUnion cu;

// =============================================================================
// SECTION 61: MULTI-DIMENSIONAL OPERATIONS
// =============================================================================

int matrix_access(void) {
    int matrix[3][4][5];
    matrix[0][1][2] = 10;
    int (*ptr2d)[4][5] = &matrix[0];
    return matrix[0][1][2];
}

// =============================================================================
// SECTION 62: FUNCTION POINTER USAGE
// =============================================================================

int apply_func(int (*func)(int), int x) {
    return func(x);
}

int (*get_func(void))(int) {
    return &simple_add;
}

// =============================================================================
// SECTION 63: VARIADIC FUNCTION DEFINITIONS
// =============================================================================

int variadic_sum(int count, ...) {
    return count;
}

void variadic_void(int first, ...) {
    return;
}

// =============================================================================
// SECTION 64: PARAMETER COMBINATIONS
// =============================================================================

int params_named(int a, int b, int c) {
    return a + b + c;
}

int params_unnamed(int, int, int) {
    return 0;
}

int params_mixed(int a, int, int c) {
    return a + c;
}

int params_ptr(int *p, char *str, float *arr) {
    return *p;
}

int params_func_ptr(int (*callback)(int), int x) {
    return callback(x);
}

// =============================================================================
// SECTION 65: COMPLEX NESTED STRUCTURES
// =============================================================================

struct Level1 {
    int id;
    struct Level2 {
        int value;
        struct Level3 {
            int deep;
        } l3;
    } l2;
};

struct Level1 nested;

// =============================================================================
// SECTION 66: COMBINED OPERATIONS
// =============================================================================

int combined_test(void) {
    int a[10][20];
    int *p[30];
    int (*q)[40];
    int (*fp[50])(int);

    a[0][0] = 5;
    p[0] = &a[0][0];
    q = &a[0];

    return a[0][0] + *p[0];
}

// =============================================================================
// SECTION 67: TYPEDEF IN COMPLEX CONTEXTS
// =============================================================================

typedef int (*MultiLevelPtr[5])[10];

MultiLevelPtr mptr = {0};

// =============================================================================
// SECTION 68: ALL OPERATORS IN ONE FUNCTION
// =============================================================================

int all_operators(int a, int b, int c) {
    // Arithmetic
    int r1 = a + b - c;
    int r2 = a * b / c;
    int r3 = a % b;

    // Relational
    int r4 = a < b;
    int r5 = a > c;
    int r6 = a == b;

    // Logical
    int r7 = a && b || c;
    int r8 = !a;

    // Bitwise
    int r9 = a & b;
    int r10 = a | c;
    int r11 = a ^ b;

    // Shift
    int r12 = a << 1;
    int r13 = b >> 1;

    // Ternary
    int r14 = a > 0 ? a : -a;

    return r1 + r2 + r3 + r4 + r7 + r9 + r12;
}

// =============================================================================
// SECTION 69: MULTIPLE FOR LOOP CASES
// =============================================================================

int loop_decl_init(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum = sum + i;
    }
    return sum;
}

int loop_expr_init(void) {
    int i = 0;
    int sum = 0;
    for (i = 0; i < 10; i++) {
        sum = sum + i;
    }
    return sum;
}

int loop_empty_init(void) {
    int i = 0;
    int sum = 0;
    for (; i < 10; i++) {
        sum = sum + i;
    }
    return sum;
}

int loop_multiple_decl_init(void) {
    int sum = 0;
    for (int i = 0, j = 10; i < j; i++, j--) {
        sum = sum + i;
    }
    return sum;
}

// =============================================================================
// SECTION 70: ALL CONTROL FLOW IN MAIN
// =============================================================================

// (Already in main above)

// =============================================================================
// END OF COMPREHENSIVE TEST SUITE
// =============================================================================
