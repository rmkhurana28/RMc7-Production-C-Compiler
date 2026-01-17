/* Complex pointer chains */
int ***p2;
const int ****p3;

/* Many qualifiers */
const volatile int x1;
volatile const char x2;

/* Sign + size + base */
unsigned long int x3;
signed short int x4;

/* Multi-declarators with mix */
int a, *b, **c, ***d;
const int *e, **f;
static int g, *h;
