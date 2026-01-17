// ========================================
// COMPREHENSIVE C DECLARATION TEST SUITE
// ========================================

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

// Test 5: Const after pointer (single const)
int * const cpc1;
char * const cpc2;
int ** const cpc3;
int * const * cpc4;

// Test 6: Storage classes
static int si;
extern int ei;
register int ri;
auto int ai;

// Test 7: Storage classes with pointers
static int *sp;
extern char **ep;

// Test 8: Sign modifiers
signed int sgi;
unsigned int usi;
signed char sgc;
unsigned char usc;
signed short sgs;
unsigned short uss;

// Test 9: Sign modifiers with pointers
signed int *sgp;
unsigned char **usp;

// Test 10: Size modifiers
short int shi;
long int li;
long double ld;

// Test 11: Size modifiers with pointers
short int *shp;

// Test 12: Complex combinations
static const int sci;
extern volatile char evc;
register const int *rcp;
static unsigned int sui;

// Test 13: Multi-declarators simple
int x1, x2, x3;
char y1, y2;

// Test 14: Multi-declarators with pointers
int *m1, **m2, ***m3;
char *c1, **c2;

// Test 15: Multi-declarators mixed
int a1, *a2, **a3;
const int b1, *b2, **b3;

// Test 16: Multi-declarators with qualifiers
const int *q1, *q2, *q3;
volatile char *vq1, **vq2;

// Test 17: Multi-declarators with storage
static int s1, *s2, **s3;
extern char *e1, **e2;

// Test 18: Complex multi-declarators
static const int *sc1, **sc2;
extern volatile char ev1, *ev2, **ev3;
register unsigned int ru1, *ru2;

// Test 19: Sign and size combinations
unsigned short int ushi;
signed long int sli;

// Test 20: All modifiers together
static const unsigned short int scusi;
extern volatile signed short int evssi;
register const unsigned int *rcuip;

// Test 21: Multiple qualifiers with pointers
const volatile int *cvip;
volatile const char **vccp;

// Test 22: Long long type (without pointers)
long long ll1;
long long int ll2;
signed long long sll1;
unsigned long long ull1;
const long long cll;
volatile long long int vll;

// Test 23: Long long with storage classes
static long long sll;
extern unsigned long long eull;

// Test 24: Unsigned/signed without explicit int
unsigned u1;
signed s1;

// Test 25: Unsigned/signed with pointers (FIXED BUG)
unsigned *up1;
signed *sp1;
short *shp1;
long *lp1;
long long *llp1;

// Test 26: Multiple levels with implicit int
unsigned **up2;
long ***lp2;

// Test 27: Storage classes with implicit int + pointers
static unsigned *sup;
extern long *elp;

// Test 28: Qualifiers with implicit int + pointers
const unsigned *cup;
volatile long *vlp;

// Test 29: Multi-declarators with implicit int
unsigned *ua1, **ua2, ***ua3;
long *la1, **la2;

// Test 30: Complex pointer chains
int ****cp1;
const int *****cp2;

// Test 31: Many qualifiers
const volatile int cv1;
volatile const char cv2;

// Test 32: Mix everything
unsigned long int mix1;
signed short int mix2;
static const unsigned *mix3;
extern volatile long **mix4;
