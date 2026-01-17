// Test const after pointer syntax

// Basic const after pointer
int * const cp1;
char * const cp2;

// Multiple levels
int ** const cpp1;
int * const * cpp2;
int * const * const cpp3;

// With type qualifiers before
const int * const cpc1;
volatile char * const vpc1;
const volatile int * const cvpc1;

// With storage classes
static int * const scp1;
extern char * const ecp1;

// With sign modifiers
unsigned int * const ucp1;
signed char * const scp2;

// With size modifiers
short int * const shcp1;

// Multi-declarators with const after pointer
int * const mc1, * const mc2;
char * const mc3, * const mc4, * const mc5;

// Mixed multi-declarators
int *m1, * const m2, **m3;
const int *cm1, * const cm2;

// Complex combinations
static const unsigned int * const scucp1;
extern volatile signed char * const evscp1;
