// ============================================================================
// RMc7 STRESS TEST - VALID COMPLEX CASES ONLY
// ============================================================================

// --- Multi-level Pointers (up to 6 levels) ---
int ******_p6;
const volatile unsigned long long ******_extreme_ptr;
static const int *****_scp5;

// --- Complex Arrays ---
int _arr5d[2][3][4][5][6];
unsigned long long _big[10][20][30];
static const int _sca[5][5][5][5];

// --- Arrays of Pointers ---
int *****_ap5[10];
int ***_ap3_multi[5][10][15];
const volatile int ****_cvap4[20];

// --- Pointers to Arrays ---
int (*_pa3)[10][20][30];
int (***_pppa)[10][20];
const unsigned long long (*_cullpa)[50][50];

// --- Function Pointers (no void params) ---
int (*_fp4)(int, char, double, float);
double (*_dfp)(double, double);
unsigned long long (*_ullfp)(unsigned long long, unsigned long long);
int (*_varfp)(int, const char *, ...);

// --- Multi-level Function Pointers ---
int (*****_fp5)(int, int, int);
double (***_dfp3)(double, double);

// --- Arrays of Function Pointers ---
int (*_afp3d[5][10][15])(int, double);
char *(*_cpafp[20])(const char *, int);
int (*_vfparr[10])(const char *, ...);

// --- Pointers to Arrays of Function Pointers ---
int (*(*_pafp)[10])(int);
int (*(**_ppafp)[5][10])(int, int);
double (*(*_dpafp)[20][30])(double, double);

// --- Function Pointers Returning Pointers ---
int ***(*_fprp3)(int);
int **(****_fprp4)(int, int, int);
unsigned long long *(*_ullfprp)(int, double);

// --- Extreme Nesting (6 levels) ---
int *(*(**_nest1)[10])(int);
int (*(*(**_nest2)[5])[10])(int, int);
int *(*(*(*_nest3)[5])[10])(int);
int *(*(*(*(*_nest4)[2])[3])[4])(int, int);

// --- Mixed Pointer/Array Complex ---
int *(*_mix1[10])[20];
int **(*_mix2[5][10])[20];
int *(*(*_mix3)[10])[20];
int (*(*_mix4[10])[20])(int);
int *(*(*_mix5[5])[10])(int, int);

// --- Named Complex Parameters ---
int _f1(int (*_fp)(int));
int _f2(int (*_fp)(int, int), int (*_fp2)(double, double));
int _f3(int (*_arr)[10]);
int _f4(int (*_arr)[10][20], int (*_fp)(int));
int _f5(int (**_pp)[10]);
int _f6(int (*(*_pafp)[10])(int));
int _f7(int (*_fp)(int), int (*_arr)[10], int ***_ppp);
int _f8(const int (*_carr)[100], volatile double (*_vfp)(double));
int _f9(int *(*(**_extreme)[10])(int), double ***_dpp);

// --- Unnamed Simple Parameters ---
int _u1(int, int, int);
int _u2(char, double, float, long, short);
int _u3(int *, char **, double ***, unsigned long ****);
int _u4(const int, volatile double, unsigned long long);
int _u5(const char *, volatile int **, unsigned long ***);

// --- Mixed Named/Unnamed ---
int _m1(int _a, int, int _c);
int _m2(int *, double _d, char **);
int _m3(const int _a, volatile double, unsigned long long ***_p);

// --- Variadic ---
int _v1(int _a, ...);
int _v2(const char *_fmt, ...);
int _v3(int _n, double _d, const char *_s, ...);

// --- Empty Parameters ---
int _e1();
int *_e2();
int **_e3();
int (*_e4())(int);
int *(*_e5())(int, int);

// --- Complex Return Types ---
int ***_r1(int);
int (*_r2(int))[10];
int *(*_r3(int, int))[20];
int (*_r4(int))(int);
int *(*_r5(int))(int, int);
int (*(*_r6(int))[10])(int);

// --- Complex Expressions ---
int _ex1 = 1 + 2 * 3 - 4 / 2 % 3 << 1 >> 2;
int _ex2 = (1 + 2) * ((3 - 4) / (2 + 1)) << (1 + 1);
int _ex3 = 1 & 2 | 3 ^ 4 & 5 | 6 ^ 7;
int _ex4 = 1 < 2 && 3 > 4 || 5 <= 6 && 7 >= 8 != 9 == 10;
int _ex5 = 1 ? 2 : 3 ? 4 : 5 ? 6 : 7;
int _ex6 = (1 ? 2 : 3) ? (4 ? 5 : 6) : (7 ? 8 : 9);
int _ex7 = ~!-+-+-+1;
int _ex8 = 1 || 2 && 3 | 4 ^ 5 & 6 == 7 < 8 << 9 + 10 * ~-11;

// --- sizeof and Casts (valid - no nested brackets) ---
int _s1 = sizeof(int);
int _s2 = sizeof(unsigned long long);
int _s3 = sizeof(const volatile int ***);
int _c1 = (int)3.14;
int *_c2 = (int *)0;
int ***_c3 = (const int ***)0;
unsigned long long _c4 = (unsigned long long)(int)3.14;

// --- Storage + Qualifiers + Complex ---
static const int *(*(*_ultimate1)[10])(int, double);
extern volatile unsigned long long ***(*_ultimate2[5])(int);
static const volatile int *(*(**_ultimate3)[10][20])(const char *, ...);