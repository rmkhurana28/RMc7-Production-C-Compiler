// Basic Cases
int a;
int *p;
int **pp;
int ***ppp;

// Arrays
int arr1[];
int arr2[][];
int arr3[][][];
int *arr4[];
int (*pa1)[];
int (*pa2)[][];

// Function Pointers
int (*fp1)(int a, char b);
int (*afp1[])(int, char);
int *func1(int a);
int **func2(int a);

// Function Returning Function Pointer
int (*func3(int a))(char b);
int (*func4(int a))[];
int (*(*fp2)(int a))(char b);

// Variadic
int printf(const char *fmt, ...);
int func5(int a, ...);

// Storage & Qualifiers
static int b;
extern int c;
const int *q;
volatile int *r;
const volatile int **s;

// Multiple Declarations
int x, y, z;
int *p1, **p2, ***p3;
int arr5[], *p4, d;

// Previously Failing Cases
void (*signal(int sig, void (*handler)(int)))(int);
int (*(*func6(int a, char *b))(float c, int (*d)(char)))(double e);
static int *(*func7(const int *a, volatile char **b))(int, ...);

// Nested Function Pointers
int (*(*(*fp3)(int))(char))(float);
void (*(*(*(*crazy)(int))(char))(float))(double);

// Complex Combinations - All with named parameters
int (*arr6[])(char, float);
int (*func8(int (*arr7[])(char)))(float);
int (*(*arr8[])(char))(float, double);
void (*(*func9(void (*a)(int), void (*b)(char)))(float))(double);
int (*func10(int (*(*a)(char))(float)))(double);
int (*(*(*fp4[])(void))(char *))[];
extern int (*func11)(const char *s, ...);

// Edge Cases
int main(int argc, char *argv[]);
void (*func12(void))(void);
