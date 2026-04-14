/*
 * RMc7 Comprehensive Complex Test Suite
 * Strictly follows RMc7_Syntax_Reference.md
 * All features marked as ✅ supported
 */

/* ========================================================================
   SECTION 1: BASIC TYPES & STORAGE CLASSES
   ======================================================================== */

int basic_int;
char basic_char;
float basic_float;
double basic_double;
void basic_void_func(void);
short basic_short;
long basic_long;
long long basic_long_long;
signed basic_signed;
unsigned basic_unsigned;

/* ========================================================================
   SECTION 2: STORAGE CLASSES
   ======================================================================== */

static int static_int;
extern int extern_int;
auto int auto_int;
register int register_int;

/* ========================================================================
   SECTION 3: TYPE QUALIFIERS & COMBINATIONS
   ======================================================================== */

const int const_int;
volatile int volatile_int;
const volatile int const_volatile_int;
int *restrict restrict_ptr;
const int *const_ptr;
int * const const_ptr_value;
volatile int *volatile_ptr;

/* ========================================================================
   SECTION 4: MULTIPLE DECLARATIONS (ALL SUPPORTED PATTERNS)
   ======================================================================== */

int multi_a, multi_b, multi_c;
int *multi_p1, **multi_p2, multi_array[10];
int multi_const_a, multi_const_b;
int *multi_pm1, **multi_pm2;
int const *multi_cpa, multi_cpb;

/* ========================================================================
   SECTION 5: SIMPLE DECLARATORS
   ======================================================================== */

int simple_var;
int *simple_ptr;
int **simple_pptr;
int ***simple_ppptr;
int simple_arr[20];
int simple_2d[5][10];
int simple_3d[2][3][4];

/* ========================================================================
   SECTION 6: COMPLEX DECLARATORS - POINTERS TO ARRAYS
   ======================================================================== */

int (*ptr_to_arr)[10];
int (*ptr_to_2d)[5][10];
int (*ptr_to_3d)[2][3][4];

/* ========================================================================
   SECTION 7: COMPLEX DECLARATORS - ARRAYS OF POINTERS
   ======================================================================== */

int *arr_of_ptr[10];
int **arr_of_pptr[5];
int *arr_of_ptr_2d[3][4];

/* ========================================================================
   SECTION 8: FUNCTION POINTERS & ARRAYS OF FUNCTION POINTERS
   ======================================================================== */

int (*func_ptr)(int);
int (*func_ptr_two_args)(int, int);
void (*void_func_ptr)(void);
double (*double_return_func)(float);
int (*arr_func_ptr[5])(int);
int (*arr_func_ptr_multi[3][2])(int, double);

/* ========================================================================
   SECTION 9: MULTI-LEVEL COMPLEX NESTING (UP TO 6 LEVELS)
   ======================================================================== */

int *(*nested_level1)[10];
int (*(*nested_level2)[10])(int);
int *(*(*nested_level3)[5])(int);
int (*(*(*nested_level4)[10])[5])(int);
int *(*(*(*nested_level5)[10])[5])(int, double);
int (*(*(*(*nested_level6)[10])[5])(int))[20];

/* ========================================================================
   SECTION 10: TYPEDEF - SIMPLE TYPES
   ======================================================================== */

typedef int MyInt;
typedef char MyChar;
typedef float MyFloat;
typedef double MyDouble;
typedef unsigned int UInt;
typedef long long LongLong;

MyInt typedef_var1;
MyChar typedef_var2;
MyFloat typedef_var3;

/* ========================================================================
   SECTION 11: TYPEDEF - POINTERS
   ======================================================================== */

typedef int *IntPtr;
typedef int **IntPtrPtr;
typedef char *CharPtr;
typedef float *FloatPtr;

IntPtr td_ptr_var;
IntPtrPtr td_pptr_var;
CharPtr td_char_ptr_var;

/* ========================================================================
   SECTION 12: TYPEDEF - ARRAYS
   ======================================================================== */

typedef int IntArray[10];
typedef int IntMatrix[5][10];
typedef float FloatArray[20];

IntArray td_arr_var;
IntMatrix td_matrix_var;

/* ========================================================================
   SECTION 13: TYPEDEF - FUNCTION POINTERS
   ======================================================================== */

typedef int (*FuncPtr1)(int);
typedef int (*FuncPtr2)(int, int);
typedef void (*VoidFuncPtr)(void);
typedef double (*MathFunc)(float);

FuncPtr1 td_func_ptr1;
FuncPtr2 td_func_ptr2;

/* ========================================================================
   SECTION 14: TYPEDEF - COMPLEX NESTED
   ======================================================================== */

typedef int *(*ComplexType1)[10];
typedef int (*(*ComplexType2)[10])(int);
typedef int (*(*(*ComplexType3)[10])[5])(int);

ComplexType1 td_complex1;
ComplexType2 td_complex2;
ComplexType3 td_complex3;

/* ========================================================================
   SECTION 15: STRUCTURES - SIMPLE & NAMED
   ======================================================================== */

struct Point {
    int x;
    int y;
};
struct Point struct_point;
struct Point *struct_point_ptr;

struct Color {
    int r;
    int g;
    int b;
};
struct Color struct_color;

/* ========================================================================
   SECTION 16: STRUCTURES - INLINE DEFINITIONS
   ======================================================================== */

struct Size {
    int width;
    int height;
} struct_size_var;

struct Rectangle {
    int left;
    int top;
    int right;
    int bottom;
} struct_rect;

/* ========================================================================
   SECTION 17: STRUCTURES - ANONYMOUS
   ======================================================================== */

struct {
    int id;
    char name[50];
} struct_anon_employee;

struct {
    float x;
    float y;
    float z;
} struct_anon_3d_point;

struct {
    int day;
    int month;
    int year;
} struct_anon_date;

/* ========================================================================
   SECTION 18: STRUCTURES - SELF-REFERENCING (LINKED LISTS, TREES)
   ======================================================================== */

struct Node {
    int data;
    struct Node *next;
};

struct TreeNode {
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct DblLinkedNode {
    int data;
    struct DblLinkedNode *prev;
    struct DblLinkedNode *next;
};

/* ========================================================================
   SECTION 19: STRUCTURES - NESTED
   ======================================================================== */

struct Address {
    char street[100];
    char city[50];
    int zip;
};

struct Person {
    char name[50];
    int age;
    struct Address addr;
};

struct Company {
    char company_name[100];
    struct Person ceo;
    struct Address headquarters;
};

/* ========================================================================
   SECTION 20: STRUCTURES - FORWARD DECLARATIONS & TYPEDEF
   ======================================================================== */

struct ForwardDeclStruct;
struct ForwardDeclStruct *forward_ptr;

typedef struct {
    int x;
    int y;
} Point_t;
Point_t typedef_struct_point;

typedef struct LinkedListNode {
    int data;
    struct LinkedListNode *next;
} LinkedListNode_t;
LinkedListNode_t typedef_list_node;

/* ========================================================================
   SECTION 21: UNIONS - SIMPLE & NAMED
   ======================================================================== */

union Data {
    int i;
    float f;
    char c;
};
union Data union_data;
union Data *union_data_ptr;

union Value {
    int int_val;
    float float_val;
    double double_val;
};
union Value union_value;

/* ========================================================================
   SECTION 22: UNIONS - INLINE DEFINITIONS
   ======================================================================== */

union Register {
    int int_reg;
    char byte_reg[4];
} union_register;

union Variant {
    int i;
    float f;
    char str[20];
} union_variant;

/* ========================================================================
   SECTION 23: UNIONS - ANONYMOUS
   ======================================================================== */

union {
    int integer;
    float floating;
} union_anon1;

union {
    double d;
    int parts[2];
} union_anon2;

/* ========================================================================
   SECTION 24: UNIONS - FORWARD DECLARATIONS
   ======================================================================== */

union ForwardUnion;
union ForwardUnion *fwd_union_ptr;

/* ========================================================================
   SECTION 25: UNIONS - TYPEDEF
   ======================================================================== */

typedef union {
    int i;
    float f;
} Data_t;
Data_t typedef_union_var;

typedef union ComplexUnion {
    struct Point pt;
    int values[3];
} ComplexUnion_t;
ComplexUnion_t typedef_complex_union;

/* ========================================================================
   SECTION 26: ENUMERATIONS - SIMPLE WITH AUTO-ASSIGN
   ======================================================================== */

enum Color {
    RED,
    GREEN,
    BLUE
};
enum Color enum_color;

enum Status {
    OK,
    ERROR,
    PENDING,
    PROCESSING
};
enum Status enum_status;

/* ========================================================================
   SECTION 27: ENUMERATIONS - EXPLICIT VALUES
   ======================================================================== */

enum StatusCode {
    HTTP_OK = 200,
    HTTP_NOTFOUND = 404,
    HTTP_ERROR = 500
};
enum StatusCode enum_http_status;

enum Level {
    LOW = 1,
    MEDIUM = 5,
    HIGH = 10,
    CRITICAL = 100
};
enum Level enum_level;

/* ========================================================================
   SECTION 28: ENUMERATIONS - MIXED EXPLICIT & IMPLICIT
   ======================================================================== */

enum Mixed {
    M1,
    M2 = 10,
    M3,
    M4 = 20,
    M5
};
enum Mixed enum_mixed;

enum Priority {
    LOWEST,
    LOW = 3,
    MEDIUM,
    HIGH = 10,
    CRITICAL
};
enum Priority enum_priority;

/* ========================================================================
   SECTION 29: ENUMERATIONS - INLINE WITH VARIABLE
   ======================================================================== */

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} enum_direction_inline;

enum Month {
    JAN = 1,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC
} enum_month_inline;

/* ========================================================================
   SECTION 30: ENUMERATIONS - ANONYMOUS
   ======================================================================== */

enum {
    OPTION_A,
    OPTION_B,
    OPTION_C
} enum_anon1;

enum {
    STATE_IDLE = 0,
    STATE_RUNNING = 1,
    STATE_STOPPED = 2
} enum_anon2;

enum {
    FIRST,
    SECOND = 100,
    THIRD
} enum_anon3;

/* ========================================================================
   SECTION 31: ENUMERATIONS - TYPEDEF
   ======================================================================== */

typedef enum {
    ON,
    OFF
} Switch;
Switch enum_switch;

typedef enum Color2 {
    CYAN,
    MAGENTA,
    YELLOW
} Color2_t;
Color2_t enum_color2;

/* ========================================================================
   SECTION 32: EXPRESSIONS - PRIMARY EXPRESSIONS
   ======================================================================== */

int expr_primary_demo(void) {
    int x = 42;
    float f = 3.14;
    char c = 'A';
    char *str = "Hello";
    int result = (x + 42);
    int sz = sizeof(int);
    int cast_val = (int)f;
    return 0;
}

/* ========================================================================
   SECTION 33: EXPRESSIONS - ARITHMETIC OPERATORS
   ======================================================================== */

int expr_arithmetic_demo(void) {
    int a = 10;
    int b = 3;
    int add = a + b;
    int sub = a - b;
    int mul = a * b;
    int div = a / b;
    int mod = a % b;
    int pre_inc = ++a;
    int post_inc = a++;
    int pre_dec = --b;
    int post_dec = b--;
    int unary_plus = +a;
    int unary_minus = -a;
    return 0;
}

/* ========================================================================
   SECTION 34: EXPRESSIONS - RELATIONAL OPERATORS
   ======================================================================== */

int expr_relational_demo(void) {
    int a = 10;
    int b = 5;
    int less = a < b;
    int less_eq = a <= b;
    int greater = a > b;
    int greater_eq = a >= b;
    int equal = a == b;
    int not_equal = a != b;
    return 0;
}

/* ========================================================================
   SECTION 35: EXPRESSIONS - LOGICAL OPERATORS
   ======================================================================== */

int expr_logical_demo(void) {
    int a = 1;
    int b = 0;
    int and_result = a && b;
    int or_result = a || b;
    int not_result = !a;
    return 0;
}

/* ========================================================================
   SECTION 36: EXPRESSIONS - BITWISE OPERATORS
   ======================================================================== */

int expr_bitwise_demo(void) {
    int a = 12;
    int b = 10;
    int and_bit = a & b;
    int or_bit = a | b;
    int xor_bit = a ^ b;
    int not_bit = ~a;
    int left_shift = a << 2;
    int right_shift = a >> 1;
    return 0;
}

/* ========================================================================
   SECTION 37: EXPRESSIONS - ASSIGNMENT OPERATORS
   ======================================================================== */

int expr_assignment_demo(void) {
    int x = 10;
    x = 20;
    x += 5;
    x -= 3;
    x *= 2;
    x /= 4;
    x %= 3;
    x &= 0xFF;
    x |= 0x01;
    x ^= 0x0F;
    x <<= 2;
    x >>= 1;
    return 0;
}

/* ========================================================================
   SECTION 38: EXPRESSIONS - TERNARY & COMMA
   ======================================================================== */

int expr_ternary_comma_demo(void) {
    int x = 10;
    int y = 5;
    int max = x > y ? x : y;
    int min = x < y ? x : y;
    int comma_result = (x = 1, y = 2);
    return 0;
}

/* ========================================================================
   SECTION 39: EXPRESSIONS - COMPOUND OPERATOR PRECEDENCE
   ======================================================================== */

int expr_precedence_demo(void) {
    int a = 2;
    int b = 3;
    int c = 4;

    int result1 = a + b * c;
    int result2 = (a + b) * c;
    int result3 = a * b + c;
    int result4 = a << b + c;
    int result5 = a && b || c;
    int result6 = a || b && c;
    int result7 = a & b | c;
    int result8 = a + b < c;
    int result9 = a < b && b < c;
    int result10 = a > 0 ? b : c;

    return 0;
}

/* ========================================================================
   SECTION 40: FUNCTION DECLARATIONS & VARIOUS PARAMETER PATTERNS
   ======================================================================== */

int func_named_params(int a, int b);
int func_unnamed_params(int, int);
int func_mixed_params(int a, double, char c);
void func_void_return(void);
int func_variadic(int first, ...);
int func_empty_params();
double func_double_return(float x);

/* ========================================================================
   SECTION 41: FUNCTION DECLARATIONS WITH POINTER PARAMETERS
   ======================================================================== */

int func_ptr_param(int *ptr);
int func_multi_ptr_param(int *, double **, char **);
void func_func_ptr_param(int (*fp)(int));
int func_array_ptr_param(int (*arr)[10]);
int func_complex_ptr_param(int (*(*fp)[10])(int));

/* ========================================================================
   SECTION 42: FUNCTION DEFINITIONS - SIMPLE
   ======================================================================== */

int func_def_simple_add(int a, int b) {
    return a + b;
}

void func_def_print_int(int num) {
    int x = num;
}

double func_def_square(double x) {
    return x * x;
}

/* ========================================================================
   SECTION 43: FUNCTION DEFINITIONS - RECURSIVE
   ======================================================================== */

int func_factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * func_factorial(n - 1);
}

int func_fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return func_fibonacci(n - 1) + func_fibonacci(n - 2);
}

/* ========================================================================
   SECTION 44: STATEMENTS - EXPRESSION STATEMENTS
   ======================================================================== */

int stmt_expr_demo(void) {
    int x = 10;
    x = 20;
    x++;
    x + 10;
    x * 2;
    return 0;
}

/* ========================================================================
   SECTION 45: STATEMENTS - IF/ELSE
   ======================================================================== */

int stmt_if_else_demo(int x) {
    if (x > 10) {
        return x;
    } else if (x > 5) {
        return 5;
    } else {
        return 0;
    }
}

int stmt_if_no_braces(int x) {
    if (x > 10)
        return x;
    else
        return 0;
}

int stmt_nested_if(int x, int y) {
    if (x > 0) {
        if (y > 0) {
            return x + y;
        } else {
            return x - y;
        }
    } else {
        return 0;
    }
}

/* ========================================================================
   SECTION 46: STATEMENTS - SWITCH/CASE/DEFAULT
   ======================================================================== */

int stmt_switch_demo(int choice) {
    switch (choice) {
        case 1:
            return 10;
        case 2:
            return 20;
        case 3:
            return 30;
        default:
            return 0;
    }
}

int stmt_switch_fallthrough(int x) {
    switch (x) {
        case 1:
        case 2:
            return 100;
        case 3:
            return 200;
        default:
            return 0;
    }
}

/* ========================================================================
   SECTION 47: STATEMENTS - WHILE LOOP
   ======================================================================== */

int stmt_while_demo(int x) {
    int sum = 0;
    while (x > 0) {
        sum = sum + x;
        x--;
    }
    return sum;
}

int stmt_while_no_braces(int x) {
    int count = 0;
    while (x > 0)
        count = count + 1;
    return count;
}

/* ========================================================================
   SECTION 48: STATEMENTS - DO-WHILE LOOP
   ======================================================================== */

int stmt_do_while_demo(int x) {
    int sum = 0;
    do {
        sum = sum + x;
        x--;
    } while (x > 0);
    return sum;
}

int stmt_do_while_no_braces(int x) {
    int count = 0;
    do
        count = count + 1;
    while (x > 0);
    return count;
}

/* ========================================================================
   SECTION 49: STATEMENTS - FOR LOOP (ALL VARIANTS)
   ======================================================================== */

int stmt_for_basic(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum = sum + i;
    }
    return sum;
}

int stmt_for_empty_condition(int x) {
    for (int i = 0; ; i++) {
        if (i > x)
            break;
    }
    return 0;
}

int stmt_for_all_empty(void) {
    int i = 0;
    for (;;) {
        if (i > 5)
            break;
        i++;
    }
    return 0;
}

int stmt_for_no_braces(void) {
    int sum = 0;
    for (int i = 0; i < 5; i++)
        sum = sum + i;
    return sum;
}

int stmt_for_nested(void) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sum = sum + i;
        }
    }
    return sum;
}

/* ========================================================================
   SECTION 50: STATEMENTS - BREAK & CONTINUE
   ======================================================================== */

int stmt_break_demo(void) {
    int i = 0;
    while (1) {
        if (i > 5)
            break;
        i++;
    }
    return i;
}

int stmt_continue_demo(void) {
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0)
            continue;
        sum = sum + i;
    }
    return sum;
}

/* ========================================================================
   SECTION 51: STATEMENTS - GOTO & LABELS
   ======================================================================== */

int stmt_goto_demo(int x) {
    if (x > 10)
        goto end;
    x = x + 1;
    end:
    return x;
}

int stmt_goto_loop(int x) {
    int sum = 0;
    loop:
    sum = sum + x;
    x--;
    if (x > 0)
        goto loop;
    return sum;
}

/* ========================================================================
   SECTION 52: STATEMENTS - RETURN (VOID & VALUED)
   ======================================================================== */

void stmt_return_void(void) {
    return;
}

int stmt_return_valued(int x) {
    return x + 1;
}

int stmt_return_conditional(int x) {
    if (x > 10)
        return x;
    return 0;
}

/* ========================================================================
   SECTION 53: STATEMENTS - NESTED BLOCKS & BLOCK-SCOPED DECLARATIONS
   ======================================================================== */

int stmt_nested_blocks_demo(int x) {
    int outer = 10;
    {
        int inner = 20;
        {
            int deeper = 30;
            outer = inner + deeper;
        }
    }
    return outer;
}

int stmt_block_scope_vars(int x) {
    int a = 1;
    {
        int b = 2;
        a = a + b;
    }
    {
        int c = 3;
        a = a + c;
    }
    return a;
}

/* ========================================================================
   SECTION 54: COMBINED COMPLEX FEATURES
   ======================================================================== */

/* Complex structure with self-reference */
struct GraphNode {
    int value;
    struct GraphNode *next;
    struct GraphNode *prev;
};

/* Complex typedef with function pointer */
typedef int (*CompareFn)(int a, int b);

/* Complex function using multiple features */
int complex_function(int *arr, int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

/* Complex nested declarations */
int (*(*complex_declarator_1)(int))[10];
struct {
    int x;
    int y;
    int (*method)(int, int);
} complex_anon_struct;

typedef union {
    struct Point pt;
    int values[2];
} ComplexMixedType;

/* ========================================================================
   SECTION 55: MAIN FUNCTION
   ======================================================================== */

int main(void) {
    return 0;
}
