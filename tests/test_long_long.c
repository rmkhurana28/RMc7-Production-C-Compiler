// Test long long type

// Basic long long
long long ll1;
long long int ll2;

// With sign modifiers
signed long long sll1;
unsigned long long ull1;
signed long long int sll2;
unsigned long long int ull2;

// With pointers
long long *llp1;
long long int **llp2;
unsigned long long ***ullp1;
signed long long int ****sllp1;

// With storage classes
static long long sll;
extern long long int ell;
register unsigned long long rull;

// With type qualifiers
const long long cll;
volatile long long int vll;
const volatile unsigned long long cvull;

// With qualifiers and pointers
const long long *cllp;
volatile long long int **vllp;
unsigned long long const *ullcp;

// Multi-declarators
long long m1, m2, m3;
long long int *mp1, **mp2;
unsigned long long u1, *u2, **u3;

// Complex combinations
static const unsigned long long int scull;
extern volatile signed long long int evsll;
register const long long *rcllp;

// Mixed with other types
int i1;
long long ll3;
char c1;
unsigned long long ull3;
short s1;
