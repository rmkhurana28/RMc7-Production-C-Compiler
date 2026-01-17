// Test 1: Basic types
int a;
char b;
short c;
long d;
float e;
double f;

// Test 2: Multiple pointers
int *p1;
int **p2;
int ***p3;
char ****p4;

// Test 3: Type qualifiers
const int ci;
volatile int vi;
const volatile int cvi;
int const ic;
int volatile iv;

// Test 4: Qualifiers with pointers
const int *cp1;
int const *cp2;
volatile char *vp1;
const volatile int **cvp;

// Test 5: Storage classes
static int si;
extern int ei;
register int ri;
auto int ai;

// Test 6: Storage classes with pointers
static int *sp;
extern char **ep;

// Test 7: Sign modifiers
signed int sgi;
unsigned int usi;
signed char sgc;
unsigned char usc;
signed short sgs;
unsigned short uss;
signed long sgl;
unsigned long usl;

// Test 8: Sign modifiers with pointers
signed int *sgp;
unsigned char **usp;

// Test 9: Size modifiers
short int shi;
long int li;
long double ld;

// Test 10: Size modifiers with pointers
short int *shp;

// Test 11: Complex combinations
static const int sci;
extern volatile char evc;
register const int *rcp;
static unsigned int sui;

// Test 12: Multi-declarators simple
int x1, x2, x3;
char y1, y2;

// Test 13: Multi-declarators with pointers
int *m1, **m2, ***m3;
char *c1, **c2;

// Test 14: Multi-declarators mixed
int a1, *a2, **a3;
const int b1, *b2, **b3;

// Test 15: Multi-declarators with qualifiers
const int *q1, *q2, *q3;
volatile char *vq1, **vq2;

// Test 16: Multi-declarators with storage
static int s1, *s2, **s3;
extern char *e1, **e2;

// Test 17: Complex multi-declarators
static const int *sc1, **sc2;
extern volatile char ev1, *ev2, **ev3;
register unsigned int ru1, *ru2;

// Test 18: Sign and size combinations
unsigned short int ushi;

// Test 19: All modifiers together
static const unsigned short int scusi;
extern volatile signed short int evssi;
register const unsigned int *rcuip;

// Test 20: Multiple qualifiers with pointers
const volatile int *cvip;
volatile const char **vccp;
