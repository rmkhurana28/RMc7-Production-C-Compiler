/* Test all 4 modifiers with pointers */
unsigned *p1;
signed *p2;
short *p3;
long *p4;
long long *p5;

/* With multiple pointer levels */
unsigned **p6;
long ***p7;

/* With storage classes */
static unsigned *p8;
extern long *p9;

/* With qualifiers */
const unsigned *p10;
volatile long *p11;

/* Multi-declarators */
unsigned *a1, **a2, ***a3;
long *b1, **b2;
