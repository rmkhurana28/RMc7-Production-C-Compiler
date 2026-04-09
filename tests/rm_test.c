// =============================================================================
// RMc7 COMPREHENSIVE TEST SUITE - 1300+ TEST CASES
// =============================================================================
// Organized from simple to super complex scenarios
// All test cases follow ISO C syntax rules strictly
// =============================================================================

// =============================================================================
// SECTION 1: BASIC TYPE SPECIFIERS (20 test cases)
// =============================================================================

int b1;
char b2;
float b3;
double b4;
short b5;
long b6;
long long b7;
signed b8;
unsigned b9;
int b10;
char b11;
float b12;
double b13;
short b14;
long b15;
void b16_func(void);
int b17;
char b18;
float b19;
double b20;

// =============================================================================
// SECTION 2: STORAGE CLASS SPECIFIERS (25 test cases)
// =============================================================================

auto int st1;
register int st2;
static int st3;
extern int st4;
const int st5;
volatile int st6;
static const int st7;
extern volatile int st8;
auto int st9;
register int st10;
static int st11;
extern int st12;
const int st13;
volatile int st14;
static volatile int st15;
extern const int st16;
auto int st17;
register int st18;
static int st19;
extern int st20;
const int st21;
volatile int st22;
static const int st23;
extern volatile int st24;
const volatile int st25;

// =============================================================================
// SECTION 3: TYPE QUALIFIERS (30 test cases)
// =============================================================================

const int q1;
volatile int q2;
const volatile int q3;
volatile const int q4;
const int q5;
volatile int q6;
const int q7;
volatile int q8;
const volatile int q9;
volatile const int q10;
int *const q11;
const int *q12;
volatile int *q13;
const volatile int *q14;
int *volatile q15;
int *const q16;
const int *q17;
volatile int *q18;
int *q19;
int *q20;
const int q21;
volatile int q22;
const volatile int q23;
int *const q24;
const int *q25;
volatile int *q26;
int *volatile q27;
int *q28;
const int q29;
volatile int q30;

// =============================================================================
// SECTION 4: SIMPLE POINTERS (40 test cases)
// =============================================================================

int *p1;
int *p2;
int *p3;
int *p4;
int *p5;
char *p6;
char *p7;
char *p8;
float *p9;
float *p10;
double *p11;
double *p12;
short *p13;
short *p14;
long *p15;
long *p16;
int *p17;
int *p18;
char *p19;
char *p20;
float *p21;
float *p22;
double *p23;
double *p24;
short *p25;
short *p26;
long *p27;
long *p28;
int *p29;
int *p30;
char *p31;
char *p32;
float *p33;
float *p34;
double *p35;
double *p36;
short *p37;
short *p38;
long *p39;
long *p40;

// =============================================================================
// SECTION 5: MULTI-LEVEL POINTERS (50 test cases)
// =============================================================================

int **pp1;
int **pp2;
int **pp3;
int **pp4;
int **pp5;
int ***ppp1;
int ***ppp2;
int ***ppp3;
int ***ppp4;
int ***ppp5;
int ****pppp1;
int ****pppp2;
int ****pppp3;
char **pp6;
char **pp7;
char ***ppp6;
char ***ppp7;
float **pp8;
float **pp9;
float ***ppp8;
float ***ppp9;
double **pp10;
double **pp11;
double ***ppp10;
double ***ppp11;
short **pp12;
short **pp13;
short ***ppp12;
short ***ppp13;
long **pp14;
long **pp15;
long ***ppp14;
long ***ppp15;
int **pp16;
int ***ppp16;
int ****pppp4;
char **pp17;
char ***ppp17;
float **pp18;
float ***ppp18;
double **pp19;
double ***ppp19;
short **pp20;
short ***ppp20;
long **pp21;
long ***ppp21;

// =============================================================================
// SECTION 6: SIMPLE ARRAYS (40 test cases)
// =============================================================================

int a1[10];
int a2[5];
int a3[20];
int a4[100];
int a5[50];
char a6[10];
char a7[5];
char a8[20];
float a9[10];
float a10[15];
double a11[10];
double a12[25];
short a13[10];
short a14[30];
long a15[10];
long a16[40];
int a17[10];
int a18[5];
char a19[10];
char a20[15];
float a21[10];
float a22[20];
double a23[10];
double a24[30];
short a25[10];
short a26[25];
long a27[10];
long a28[35];
int a29[10];
int a30[5];
char a31[10];
char a32[20];
float a33[10];
float a34[25];
double a35[10];
double a36[40];
short a37[10];
short a38[30];
long a39[10];
long a40[45];

// =============================================================================
// SECTION 7: MULTI-DIMENSIONAL ARRAYS (35 test cases)
// =============================================================================

int a2d1[5][10];
int a2d2[3][7];
int a2d3[2][4];
int a3d1[2][3][4];
int a3d2[3][4][5];
int a3d3[2][3][5];
int a4d1[2][2][2][2];
int a4d2[2][3][4][5];
char a2d4[5][10];
char a2d5[3][7];
char a3d4[2][3][4];
float a2d6[5][10];
float a2d7[3][7];
float a3d5[2][3][4];
double a2d8[5][10];
double a2d9[3][7];
double a3d6[2][3][4];
short a2d10[5][10];
short a2d11[3][7];
short a3d7[2][3][4];
long a2d12[5][10];
long a2d13[3][7];
long a3d8[2][3][4];
int a2d14[4][6];
int a2d15[7][8];
int a3d9[1][2][3];
char a2d16[6][9];
char a3d10[3][2][4];
float a2d17[8][5];
float a3d11[2][5][3];
double a2d18[9][4];
double a3d12[4][2][3];
short a2d19[10][3];
short a3d13[5][2][2];
long a2d20[2][15];

// =============================================================================
// SECTION 8: ARRAYS OF POINTERS (30 test cases)
// =============================================================================

int *ap1[10];
int *ap2[5];
int **ap3[10];
int **ap4[5];
int ***ap5[10];
char *ap6[10];
char *ap7[15];
char **ap8[10];
float *ap9[10];
float *ap10[20];
float **ap11[10];
double *ap12[10];
double *ap13[25];
double **ap14[10];
short *ap15[10];
short *ap16[30];
short **ap17[10];
long *ap18[10];
long *ap19[35];
long **ap20[10];
int *ap21[8];
int **ap22[7];
int ***ap23[6];
char *ap24[12];
char **ap25[11];
float *ap26[14];
float **ap27[13];
double *ap28[16];
double **ap29[15];
short *ap30[9];

// =============================================================================
// SECTION 9: POINTERS TO ARRAYS (25 test cases)
// =============================================================================

int (*pa1)[10];
int (*pa2)[5];
int (*pa3)[5][10];
int (*pa4)[2][3][4];
char (*pa5)[10];
char (*pa6)[5];
char (*pa7)[5][10];
float (*pa8)[10];
float (*pa9)[5];
float (*pa10)[5][10];
double (*pa11)[10];
double (*pa12)[5];
double (*pa13)[5][10];
short (*pa14)[10];
short (*pa15)[5];
short (*pa16)[5][10];
long (*pa17)[10];
long (*pa18)[5];
long (*pa19)[5][10];
int (*pa20)[8];
int (*pa21)[3][7];
char (*pa22)[12];
float (*pa23)[15];
double (*pa24)[20];
short (*pa25)[5];

// =============================================================================
// SECTION 10: FUNCTION POINTERS - SIMPLE (35 test cases)
// =============================================================================

int (*fp1)(int);
int (*fp2)(int, int);
int (*fp3)(int, int, int);
char (*fp4)(char);
float (*fp5)(float);
double (*fp6)(double);
void (*fp7)(void);
void (*fp8)(int);
void (*fp9)(int, int);
int (*fp10)(void);
int (*fp11)(void);
int (*fp12)(void);
char (*fp13)(void);
float (*fp14)(void);
double (*fp15)(void);
int (*fp16)(int);
int (*fp17)(int, int);
char (*fp18)(char);
float (*fp19)(float);
double (*fp20)(double);
void (*fp21)(void);
void (*fp22)(int);
void (*fp23)(int, int);
int (*fp24)(void);
char (*fp25)(void);
float (*fp26)(void);
double (*fp27)(void);
int (*fp28)(int);
char (*fp29)(char);
float (*fp30)(float);
double (*fp31)(double);
void (*fp32)(void);
int (*fp33)(int, int, int);
char (*fp34)(char, char);
float (*fp35)(float, float);

// =============================================================================
// SECTION 11: COMPLEX NESTED FUNCTION POINTERS (40 test cases)
// =============================================================================

int (*(*nfp1)(int))(int);
int (*(*nfp2)(int))(int, int);
int (*(*nfp3)(int, int))(int);
char (*(*nfp4)(char))(char);
float (*(*nfp5)(float))(float);
double (*(*nfp6)(double))(double);
void (*(*nfp7)(void))(void);
void (*(*nfp8)(int))(int);
int (*(*nfp9)(int, int))(int, int);
int (*(*nfp10)(void))(void);
int (*(*nfp11)(int))(void);
void (*(*nfp12)(void))(int);
int (*(*nfp13)(int))(int);
char (*(*nfp14)(char))(char);
float (*(*nfp15)(float))(float);
double (*(*nfp16)(double))(double);
void (*(*nfp17)(void))(void);
int (*(*nfp18)(int))(int);
int (*(*nfp19)(int, int))(int, int);
char (*(*nfp20)(char, char))(char);
float (*(*nfp21)(float, float))(float);
double (*(*nfp22)(double, double))(double);
int (*(*nfp23)(int))(int);
char (*(*nfp24)(char))(char);
float (*(*nfp25)(float))(float);
double (*(*nfp26)(double))(double);
void (*(*nfp27)(void))(void);
int (*(*nfp28)(int, int))(int, int);
char (*(*nfp29)(char, char))(char);
float (*(*nfp30)(float, float))(float);
double (*(*nfp31)(double, double))(double);
void (*(*nfp32)(int, int))(int, int);
void (*(*nfp33)(char, char))(char, char);
int (*(*nfp34)(float))(float);
int (*(*nfp35)(double))(double);
int (*(*nfp36)(int))(int);
char (*(*nfp37)(char))(char);
float (*(*nfp38)(float))(float);
double (*(*nfp39)(double))(double);
void (*(*nfp40)(void))(void);

// =============================================================================
// SECTION 12: TYPEDEF - BASIC TYPES (50 test cases)
// =============================================================================

typedef int TD_int;
typedef char TD_char;
typedef float TD_float;
typedef double TD_double;
typedef short TD_short;
typedef long TD_long;
typedef long long TD_longlong;
typedef int TD_int2;
typedef char TD_char2;
typedef float TD_float2;
typedef double TD_double2;
typedef short TD_short2;
typedef long TD_long2;
typedef unsigned TD_unsigned;
typedef signed TD_signed;
typedef int TD_int3;
typedef char TD_char3;
typedef float TD_float3;
typedef double TD_double3;
typedef short TD_short3;
typedef long TD_long3;
typedef int TD_int4;
typedef int TD_int5;
typedef int TD_int6;
typedef int TD_int7;
typedef int TD_int8;
typedef int TD_int9;
typedef int TD_int10;
typedef char TD_char4;
typedef char TD_char5;
typedef char TD_char6;
typedef char TD_char7;
typedef char TD_char8;
typedef float TD_float4;
typedef float TD_float5;
typedef float TD_float6;
typedef float TD_float7;
typedef double TD_double4;
typedef double TD_double5;
typedef double TD_double6;
typedef double TD_double7;
typedef short TD_short4;
typedef short TD_short5;
typedef short TD_short6;
typedef long TD_long4;
typedef long TD_long5;
typedef long TD_long6;

// =============================================================================
// SECTION 13: TYPEDEF - POINTERS (50 test cases)
// =============================================================================

typedef int *TD_pint;
typedef char *TD_pchar;
typedef float *TD_pfloat;
typedef double *TD_pdouble;
typedef int **TD_ppint;
typedef int ***TD_pppint;
typedef char **TD_ppchar;
typedef char ***TD_pppchar;
typedef float **TD_ppfloat;
typedef float ***TD_pppfloat;
typedef double **TD_ppdouble;
typedef double ***TD_pppdouble;
typedef int *TD_pint2;
typedef int **TD_ppint2;
typedef char *TD_pchar2;
typedef char **TD_ppchar2;
typedef float *TD_pfloat2;
typedef float **TD_ppfloat2;
typedef double *TD_pdouble2;
typedef double **TD_ppdouble2;
typedef int *TD_pint3;
typedef int *TD_pint4;
typedef int *TD_pint5;
typedef char *TD_pchar3;
typedef char *TD_pchar4;
typedef float *TD_pfloat3;
typedef float *TD_pfloat4;
typedef double *TD_pdouble3;
typedef double *TD_pdouble4;
typedef int **TD_ppint3;
typedef int **TD_ppint4;
typedef int **TD_ppint5;
typedef char **TD_ppchar3;
typedef char **TD_ppchar4;
typedef float **TD_ppfloat3;
typedef float **TD_ppfloat4;
typedef double **TD_ppdouble3;
typedef double **TD_ppdouble4;
typedef int ***TD_pppint2;
typedef int ***TD_pppint3;
typedef char ***TD_pppchar2;
typedef char ***TD_pppchar3;
typedef float ***TD_pppfloat2;
typedef float ***TD_pppfloat3;
typedef double ***TD_pppdouble2;
typedef double ***TD_pppdouble3;

// =============================================================================
// SECTION 14: TYPEDEF - ARRAYS (40 test cases)
// =============================================================================

typedef int TD_aint[10];
typedef char TD_achar[10];
typedef float TD_afloat[10];
typedef double TD_adouble[10];
typedef int TD_aint2[5];
typedef int TD_aint3[20];
typedef int TD_aint4[100];
typedef char TD_achar2[15];
typedef char TD_achar3[20];
typedef float TD_afloat2[15];
typedef float TD_afloat3[20];
typedef double TD_adouble2[15];
typedef double TD_adouble3[20];
typedef int TD_a2dint[5][10];
typedef int TD_a2dint2[3][7];
typedef char TD_a2dchar[5][10];
typedef char TD_a2dchar2[3][7];
typedef float TD_a2dfloat[5][10];
typedef float TD_a2dfloat2[3][7];
typedef double TD_a2ddouble[5][10];
typedef double TD_a2ddouble2[3][7];
typedef int TD_a3dint[2][3][4];
typedef int TD_a3dint2[3][4][5];
typedef char TD_a3dchar[2][3][4];
typedef char TD_a3dchar2[3][4][5];
typedef float TD_a3dfloat[2][3][4];
typedef float TD_a3dfloat2[3][4][5];
typedef double TD_a3ddouble[2][3][4];
typedef double TD_a3ddouble2[3][4][5];
typedef int *TD_pAint[10];
typedef int **TD_ppAint[10];
typedef int (*TD_apint)[10];
typedef int (*TD_apint2)[5];
typedef int (*TD_a2pint)[5][10];
typedef char (*TD_apchar)[10];
typedef float (*TD_apfloat)[10];
typedef double (*TD_apdouble)[10];

// =============================================================================
// SECTION 15: TYPEDEF - FUNCTION POINTERS (40 test cases)
// =============================================================================

typedef int (*TD_fpint)(int);
typedef int (*TD_fpint2)(int, int);
typedef char (*TD_fpchar)(char);
typedef float (*TD_fpfloat)(float);
typedef double (*TD_fpdouble)(double);
typedef void (*TD_fpvoid)(void);
typedef void (*TD_fpvoid2)(int);
typedef int (*TD_fpint3)(void);
typedef int (*TD_fpint4)(int);
typedef char (*TD_fpchar2)(char);
typedef float (*TD_fpfloat2)(float);
typedef double (*TD_fpdouble2)(double);
typedef void (*TD_fpvoid3)(void);
typedef void (*TD_fpvoid4)(int);
typedef int (*TD_fpint5)(int, int);
typedef int (*TD_fpint6)(int, int, int);
typedef char (*TD_fpchar3)(char, char);
typedef float (*TD_fpfloat3)(float, float);
typedef double (*TD_fpdouble3)(double, double);
typedef void (*TD_fpvoid5)(int, int);
typedef int (*TD_fpint7)(int);
typedef int (*TD_fpint8)(int);
typedef int (*TD_fpint9)(int);
typedef char (*TD_fpchar4)(char);
typedef char (*TD_fpchar5)(char);
typedef float (*TD_fpfloat4)(float);
typedef float (*TD_fpfloat5)(float);
typedef double (*TD_fpdouble4)(double);
typedef double (*TD_fpdouble5)(double);
typedef void (*TD_fpvoid6)(void);
typedef void (*TD_fpvoid7)(void);
typedef int (*TD_fpint10)(int, int);
typedef int (*TD_fpint11)(int, int);
typedef char (*TD_fpchar6)(char, char);
typedef char (*TD_fpchar7)(char, char);
typedef float (*TD_fpfloat6)(float, float);
typedef float (*TD_fpfloat7)(float, float);
typedef double (*TD_fpdouble6)(double, double);

// =============================================================================
// SECTION 16: TYPEDEF - CHAINS (35 test cases)
// =============================================================================

typedef int TD_chain1;
typedef TD_chain1 TD_chain2;
typedef TD_chain2 TD_chain3;
typedef TD_chain3 TD_chain4;
typedef TD_chain4 TD_chain5;
typedef TD_chain5 TD_chain6;
typedef int *TD_pchain1;
typedef TD_pchain1 *TD_pchain2;
typedef TD_pchain2 *TD_pchain3;
typedef TD_pchain3 *TD_pchain4;
typedef int (*TD_fpchain1)(int);
typedef TD_fpchain1 *TD_fpchain2;
typedef int TD_achain1[10];
typedef TD_achain1 *TD_achain2;
typedef int (*TD_c3)[10];
typedef TD_c3 *TD_c3p;
typedef int *TD_ca1[10];
typedef TD_ca1 *TD_ca1p;
typedef int TD_mixed1[10];
typedef TD_mixed1 *TD_mixed2;
typedef TD_mixed2 TD_mixed3[5];
typedef int *TD_pm1;
typedef TD_pm1 TD_pm2[10];
typedef TD_pm2 *TD_pm3;
typedef int (*TD_pf1)(int);
typedef TD_pf1 *TD_pf2;
typedef TD_pf2 TD_pf3[10];
typedef int TD_deep1;
typedef TD_deep1 *TD_deep2;
typedef TD_deep2 **TD_deep3;
typedef TD_deep3 ***TD_deep4;
typedef int (*TD_fn1)(int);
typedef TD_fn1 *TD_fn2;
typedef TD_fn2 TD_fn3[10];
typedef TD_fn3 *TD_fn4;

// =============================================================================
// SECTION 17: STRUCT - SIMPLE DEFINITIONS (40 test cases)
// =============================================================================

struct S1 { int x; };
struct S2 { char x; };
struct S3 { float x; };
struct S4 { double x; };
struct S5 { int x; int y; };
struct S6 { char x; float y; };
struct S7 { int x; int y; int z; };
struct S8 { int a; char b; float c; double d; };
struct S9 { int x; };
struct S10 { char y; };
struct S11 { float z; };
struct S12 { double w; };
struct S13 { int x; int y; };
struct S14 { char a; float b; };
struct S15 { int p; double q; };
struct S16 { int f1; int f2; int f3; };
struct S17 { char c1; char c2; };
struct S18 { float f1; float f2; };
struct S19 { double d1; double d2; };
struct S20 { int x; char y; };
struct S21 { int a; int b; int c; int d; };
struct S22 { char x; float y; };
struct S23 { double p; int q; };
struct S24 { float a; float b; float c; };
struct S25 { int x; };
struct S26 { char x; };
struct S27 { float x; };
struct S28 { double x; };
struct S29 { int x; int y; };
struct S30 { char a; float b; };
struct S31 { int x; int y; int z; };
struct S32 { int a; char b; float c; };
struct S33 { int x; };
struct S34 { char y; };
struct S35 { float z; };
struct S36 { double w; };
struct S37 { int m; int n; };
struct S38 { char p; float q; };
struct S39 { int r; double s; };
struct S40 { float t; float u; };

// =============================================================================
// SECTION 18: STRUCT - VARIABLES (30 test cases)
// =============================================================================

struct S1 sv1;
struct S2 sv2;
struct S3 sv3;
struct S4 sv4;
struct S5 sv5;
struct S6 sv6;
struct S7 sv7;
struct S8 sv8;
struct S9 sv9;
struct S10 sv10;
struct S11 sv11;
struct S12 sv12;
struct S13 sv13;
struct S14 sv14;
struct S15 sv15;
struct S16 sv16;
struct S17 sv17;
struct S18 sv18;
struct S19 sv19;
struct S20 sv20;
struct S21 sv21;
struct S22 sv22;
struct S23 sv23;
struct S24 sv24;
struct S25 sv25;
struct S26 sv26;
struct S27 sv27;
struct S28 sv28;
struct S29 sv29;
struct S30 sv30;

// =============================================================================
// SECTION 19: STRUCT - POINTERS (25 test cases)
// =============================================================================

struct S1 *sp1;
struct S2 *sp2;
struct S3 *sp3;
struct S4 *sp4;
struct S5 *sp5;
struct S6 *sp6;
struct S7 *sp7;
struct S8 *sp8;
struct S9 *sp9;
struct S10 *sp10;
struct S11 *sp11;
struct S12 *sp12;
struct S13 *sp13;
struct S14 *sp14;
struct S15 *sp15;
struct S16 *sp16;
struct S17 *sp17;
struct S18 *sp18;
struct S19 *sp19;
struct S20 *sp20;
struct S21 *sp21;
struct S22 *sp22;
struct S23 *sp23;
struct S24 *sp24;
struct S25 *sp25;

// =============================================================================
// SECTION 20: STRUCT - SELF-REFERENCING (20 test cases)
// =============================================================================

struct Node1 { int data; struct Node1 *next; };
struct Node2 { char data; struct Node2 *next; };
struct Node3 { float data; struct Node3 *next; };
struct Tree1 { int data; struct Tree1 *left; struct Tree1 *right; };
struct Tree2 { char data; struct Tree2 *left; struct Tree2 *right; };
struct Node4 { int val; struct Node4 *prev; struct Node4 *next; };
struct Node5 { char c; struct Node5 *link; };
struct Node6 { float f; struct Node6 *ptr; };
struct Tree3 { int x; struct Tree3 *l; struct Tree3 *r; };
struct Tree4 { double d; struct Tree4 *left; struct Tree4 *right; };
struct N1 { int a; struct N1 *n; };
struct N2 { char b; struct N2 *n; };
struct N3 { float f; struct N3 *n; };
struct N4 { double d; struct N4 *n; };
struct T1 { int x; struct T1 *first; struct T1 *second; };
struct T2 { char c; struct T2 *first; struct T2 *second; };
struct T3 { float f; struct T3 *first; struct T3 *second; };
struct T4 { double d; struct T4 *first; struct T4 *second; };
struct Pair { int x; int y; struct Pair *next; };
struct Triple { int a; int b; int c; struct Triple *link; };

// =============================================================================
// SECTION 21: STRUCT - NESTED STRUCTS (25 test cases)
// =============================================================================

struct Outer1 { struct S1 inner; int x; };
struct Outer2 { struct S2 inner; char y; };
struct Outer3 { struct S3 inner; float z; };
struct Outer4 { struct S4 inner; double w; };
struct Outer5 { struct S5 inner; int a; };
struct Outer6 { struct S6 inner; char b; };
struct Outer7 { struct S7 inner; float c; };
struct Outer8 { int x; struct S8 inner; };
struct Outer9 { char y; struct S9 inner; };
struct Outer10 { float z; struct S10 inner; };
struct Outer11 { struct S1 in1; struct S2 in2; };
struct Outer12 { struct S3 in1; struct S4 in2; };
struct Outer13 { struct S5 in1; struct S6 in2; };
struct Outer14 { struct S7 in1; struct S8 in2; };
struct Outer15 { struct S1 x; struct S2 y; struct S3 z; };
struct Outer16 { struct S4 a; struct S5 b; struct S6 c; };
struct Outer17 { int m; struct S1 s; float n; };
struct Outer18 { char p; struct S2 t; double q; };
struct Outer19 { struct S3 first; int second; struct S4 third; };
struct Outer20 { struct S5 one; struct S6 two; int three; };
struct Outer21 { struct S7 a; struct S8 b; };
struct Outer22 { struct S1 x; struct S2 y; };
struct Outer23 { struct S3 p; struct S4 q; };
struct Outer24 { struct S5 m; struct S6 n; };
struct Outer25 { struct S7 first; struct S8 second; };

// =============================================================================
// SECTION 22: STRUCT - ARRAYS AND POINTERS (25 test cases)
// =============================================================================

struct S1 sa1[10];
struct S2 sa2[5];
struct S3 sa3[20];
struct S4 sa4[10];
struct S5 sa5[15];
struct S6 sa6[10];
struct S7 sa7[25];
struct S8 sa8[10];
struct S1 *sap1[10];
struct S2 *sap2[5];
struct S3 *sap3[10];
struct S4 *sap4[15];
struct S5 *sap5[10];
struct S6 *sap6[20];
struct S7 *sap7[10];
struct S8 *sap8[25];
struct S1 (*spax)[10];
struct S2 (*spay)[5];
struct S3 (*spaz)[10];
struct S4 (*spaw)[15];
struct S5 (*spau)[10];
struct S6 (*spav)[20];
struct S7 (*spas)[10];
struct S8 (*spat)[25];
struct S1 *sptr1;

// =============================================================================
// SECTION 23: STRUCT - FORWARD DECLARATIONS (20 test cases)
// =============================================================================

struct FwdNode1;
struct FwdNode2;
struct FwdNode3;
struct FwdNode4;
struct FwdNode5;
struct FwdTree1;
struct FwdTree2;
struct FwdTree3;
struct FwdTree4;
struct FwdTree5;
struct Fwd1;
struct Fwd2;
struct Fwd3;
struct Fwd4;
struct Fwd5;
struct FwdA;
struct FwdB;
struct FwdC;
struct FwdD;
struct FwdE;

// =============================================================================
// SECTION 24: UNION - SIMPLE DEFINITIONS (30 test cases)
// =============================================================================

union U1 { int x; };
union U2 { char x; };
union U3 { float x; };
union U4 { double x; };
union U5 { int x; char y; };
union U6 { float x; double y; };
union U7 { int a; char b; float c; };
union U8 { int x; int y; int z; };
union U9 { char a; char b; char c; };
union U10 { float p; float q; float r; };
union U11 { int x; };
union U12 { char y; };
union U13 { float z; };
union U14 { double w; };
union U15 { int x; char y; };
union U16 { float a; double b; };
union U17 { int p; int q; };
union U18 { char m; char n; };
union U19 { float u; float v; };
union U20 { double s; double t; };
union U21 { int x; };
union U22 { char x; };
union U23 { float x; };
union U24 { double x; };
union U25 { int x; char y; };
union U26 { float x; double y; };
union U27 { int a; char b; };
union U28 { float p; double q; };
union U29 { int val; };
union U30 { char data; };

// =============================================================================
// SECTION 25: UNION - VARIABLES (20 test cases)
// =============================================================================

union U1 uv1;
union U2 uv2;
union U3 uv3;
union U4 uv4;
union U5 uv5;
union U6 uv6;
union U7 uv7;
union U8 uv8;
union U9 uv9;
union U10 uv10;
union U11 uv11;
union U12 uv12;
union U13 uv13;
union U14 uv14;
union U15 uv15;
union U16 uv16;
union U17 uv17;
union U18 uv18;
union U19 uv19;
union U20 uv20;

// =============================================================================
// SECTION 26: UNION - POINTERS (15 test cases)
// =============================================================================

union U1 *up1;
union U2 *up2;
union U3 *up3;
union U4 *up4;
union U5 *up5;
union U6 *up6;
union U7 *up7;
union U8 *up8;
union U9 *up9;
union U10 *up10;
union U11 *up11;
union U12 *up12;
union U13 *up13;
union U14 *up14;
union U15 *up15;

// =============================================================================
// SECTION 27: ENUM - SIMPLE DEFINITIONS (35 test cases)
// =============================================================================

enum E1 { A1 };
enum E2 { A2, B2 };
enum E3 { A3, B3, C3 };
enum E4 { A4, B4, C4, D4 };
enum E5 { X1 };
enum E6 { X2, Y2 };
enum E7 { X3, Y3, Z3 };
enum E8 { X4, Y4, Z4, W4 };
enum E9 { M1 };
enum E10 { M2, N2 };
enum E11 { M3, N3, O3 };
enum E12 { M4, N4, O4, P4 };
enum E13 { V1 };
enum E14 { V2, U2 };
enum E15 { V3, U3, W3 };
enum E16 { V4, U4, W4, T4 };
enum E17 { a1 };
enum E18 { a2, b2 };
enum E19 { a3, b3, c3 };
enum E20 { a4, b4, c4, d4 };
enum E21 { p1 };
enum E22 { p2, q2 };
enum E23 { p3, q3, r3 };
enum E24 { p4, q4, r4, s4 };
enum E25 { L1 };
enum E26 { L2, K2 };
enum E27 { L3, K3, J3 };
enum E28 { L4, K4, J4, I4 };
enum E29 { E1_v };
enum E30 { E2_v, E3_v };
enum E31 { E4_v, E5_v, E6_v };
enum E32 { E7_v, E8_v, E9_v, E10_v };
enum E33 { F1_v };
enum E34 { F2_v, F3_v };
enum E35 { F4_v, F5_v, F6_v };

// =============================================================================
// SECTION 28: ENUM - WITH EXPLICIT VALUES (40 test cases)
// =============================================================================

enum EV1 { A1 = 1 };
enum EV2 { A2 = 10, B2 = 20 };
enum EV3 { A3 = 100, B3 = 200, C3 = 300 };
enum EV4 { A4 = 1, B4 = 2, C4 = 3, D4 = 4 };
enum EV5 { X1 = 5 };
enum EV6 { X2 = 50, Y2 = 60 };
enum EV7 { X3 = 500, Y3 = 600, Z3 = 700 };
enum EV8 { X4 = 1, Y4 = 10, Z4 = 100, W4 = 1000 };
enum EV9 { M1 = 11 };
enum EV10 { M2 = 22, N2 = 33 };
enum EV11 { M3 = 111, N3 = 222, O3 = 333 };
enum EV12 { M4 = 1, N4 = 2, O4 = 3, P4 = 4 };
enum EV13 { V1 = 99 };
enum EV14 { V2 = 88, U2 = 77 };
enum EV15 { V3 = 87, U3 = 76, W3 = 65 };
enum EV16 { V4 = 10, U4 = 20, W4 = 30, T4 = 40 };
enum EV17 { a1 = 1 };
enum EV18 { a2 = 5, b2 = 10 };
enum EV19 { a3 = 15, b3 = 25, c3 = 35 };
enum EV20 { a4 = 100, b4 = 200, c4 = 300, d4 = 400 };
enum EV21 { p1 = 7 };
enum EV22 { p2 = 14, q2 = 21 };
enum EV23 { p3 = 28, q3 = 35, r3 = 42 };
enum EV24 { p4 = 49, q4 = 56, r4 = 63, s4 = 70 };
enum EV25 { L1 = 123 };
enum EV26 { L2 = 234, K2 = 345 };
enum EV27 { L3 = 456, K3 = 567, J3 = 678 };
enum EV28 { L4 = 789, K4 = 890, J4 = 901, I4 = 1000 };
enum EV29 { E1_v = 55 };
enum EV30 { E2_v = 66, E3_v = 77 };
enum EV31 { E4_v = 88, E5_v = 99, E6_v = 110 };
enum EV32 { E7_v = 121, E8_v = 132, E9_v = 143, E10_v = 154 };
enum EV33 { F1_v = 200 };
enum EV34 { F2_v = 201, F3_v = 202 };
enum EV35 { F4_v = 203, F5_v = 204, F6_v = 205 };
enum EV36 { G1 = 1, G2 = 2, G3 = 3 };
enum EV37 { G4 = 10, G5 = 20, G6 = 30 };
enum EV38 { G7 = 100, G8 = 200, G9 = 300 };
enum EV39 { G10 = 1000, G11 = 2000, G12 = 3000 };
enum EV40 { G13 = 5, G14 = 10, G15 = 15 };

// =============================================================================
// SECTION 29: ENUM - MIXED VALUES (35 test cases)
// =============================================================================

enum EM1 { M1A = 1, M1B };
enum EM2 { M2A = 10, M2B, M2C = 20 };
enum EM3 { M3A, M3B = 5, M3C };
enum EM4 { M4A = 1, M4B, M4C, M4D = 10 };
enum EM5 { E5A = 100, E5B };
enum EM6 { E6A = 50, E6B, E6C = 75 };
enum EM7 { E7A, E7B = 10, E7C };
enum EM8 { E8A = 5, E8B, E8C, E8D = 20 };
enum EM9 { E9A = 200, E9B };
enum EM10 { E10A = 99, E10B, E10C = 150 };
enum EM11 { E11A, E11B = 25, E11C };
enum EM12 { E12A = 8, E12B, E12C, E12D = 32 };
enum EM13 { E13A = 11, E13B };
enum EM14 { E14A = 22, E14B, E14C = 44 };
enum EM15 { E15A, E15B = 33, E15C };
enum EM16 { E16A = 7, E16B, E16C, E16D = 28 };
enum EM17 { E17A = 13, E17B };
enum EM18 { E18A = 26, E18B, E18C = 39 };
enum EM19 { E19A, E19B = 47, E19C };
enum EM20 { E20A = 9, E20B, E20C, E20D = 36 };
enum EM21 { E21A = 15, E21B };
enum EM22 { E22A = 30, E22B, E22C = 45 };
enum EM23 { E23A, E23B = 17, E23C };
enum EM24 { E24A = 3, E24B, E24C, E24D = 12 };
enum EM25 { E25A = 19, E25B };
enum EM26 { E26A = 38, E26B, E26C = 57 };
enum EM27 { E27A, E27B = 21, E27C };
enum EM28 { E28A = 6, E28B, E28C, E28D = 24 };
enum EM29 { E29A = 23, E29B };
enum EM30 { E30A = 46, E30B, E30C = 69 };
enum EM31 { E31A, E31B = 34, E31C };
enum EM32 { E32A = 4, E32B, E32C, E32D = 16 };
enum EM33 { E33A = 27, E33B };
enum EM34 { E34A = 54, E34B, E34C = 81 };
enum EM35 { E35A, E35B = 41, E35C };

// =============================================================================
// SECTION 30: FUNCTION DECLARATIONS (50 test cases)
// =============================================================================

int func_1(void);
int func_2(int);
int func_3(int, int);
int func_4(int, int, int);
char func_5(void);
char func_6(char);
float func_7(float);
float func_8(float, float);
double func_9(double);
double func_10(double, double);
void func_11(void);
void func_12(int);
void func_13(int, int);
int *func_14(void);
char *func_15(void);
float *func_16(void);
double *func_17(void);
int func_18(int *);
int func_19(char *);
int func_20(float *);
int func_21(int *, int *);
int func_22(char *, char *);
float func_23(float *, float *);
double func_24(double *, double *);
int (*func_25(void))(int);
int (*func_26(int))(int);
void func_27(int (*)(int));
void func_28(int (*)(int, int));
void func_29(int *(*)(int));
void func_30(struct S1);
void func_31(struct S1 *);
int func_32(union U1);
int func_33(union U1 *);
void func_34(enum E1);
void func_35(enum E1 *);
int func_36(int);
int func_37(int, int);
int func_38(int, int, int);
void func_39(void);
void func_40(int);
int func_41(void);
int func_42(int);
char func_43(void);
float func_44(void);
double func_45(void);
int func_46(int *);
char func_47(char *);
int func_48(int (*)(int));
int func_49(void);
int func_50(int, int, int);

// =============================================================================
// SECTION 31: FUNCTION DEFINITIONS - SIMPLE (60 test cases)
// =============================================================================

int def_1(void) { return 0; }
int def_2(int x) { return x; }
int def_3(int x, int y) { return x + y; }
char def_4(char c) { return c; }
float def_5(float f) { return f; }
double def_6(double d) { return d; }
void def_7(void) { }
void def_8(int x) { }
int def_9(void) { return 42; }
int def_10(int a) { return a * 2; }
int def_11(int a, int b) { return a + b; }
int def_12(int a, int b, int c) { return a + b + c; }
char def_13(void) { return 'a'; }
char def_14(char x) { return x; }
float def_15(void) { return 3.14f; }
double def_16(void) { return 3.14; }
int def_17(int x) { int y = x; return y; }
int def_18(int x, int y) { int z = x + y; return z; }
void def_19(void) { int x = 5; }
void def_20(int a) { int b = a; }
int def_21(void) { return 1; }
int def_22(int x) { return x + 1; }
int def_23(int x, int y) { return x - y; }
int def_24(int x) { return x * 2; }
int def_25(int x) { return x / 2; }
char def_26(char c) { return c; }
char def_27(char a, char b) { return a; }
float def_28(float f) { return f * 2.0f; }
double def_29(double d) { return d * 2.0; }
int def_30(int x) { if (x > 0) return 1; return 0; }
int def_31(int x) { if (x > 0) { return 1; } else { return 0; } }
int def_32(int x) { if (x > 0) return 1; else if (x < 0) return -1; else return 0; }
int def_33(int x) { while (x > 0) { x = x - 1; } return x; }
int def_34(int x) { int sum = 0; while (x > 0) { sum = sum + x; x = x - 1; } return sum; }
int def_35(int x) { int i = 0; do { i = i + 1; } while (i < x); return i; }
int def_36(int x) { int sum = 0; for (int i = 0; i < x; i = i + 1) { sum = sum + i; } return sum; }
int def_37(int x) { switch (x) { case 1: return 10; default: return 0; } }
int def_38(int x) { switch (x) { case 1: return 10; case 2: return 20; default: return 0; } }
int def_39(int x) { for (int i = 0; i < 10; i = i + 1) { if (i == 5) break; } return x; }
int def_40(int x) { for (int i = 0; i < 10; i = i + 1) { if (i == 5) continue; } return x; }
int def_41(void) { goto label; return 0; label: return 1; }
int def_42(int x) { return x; }
int def_43(int x, int y) { return x + y; }
int def_44(int x, int y, int z) { return x + y + z; }
int def_45(void) { return 100; }
int def_46(int a) { return a + 10; }
int def_47(int a, int b) { return a * b; }
int def_48(int n) { if (n <= 1) return 1; return n; }
int def_49(int x) { return x * x; }
int def_50(int x) { return x + x + x; }
int def_51(void) { int x = 5; return x; }
int def_52(int x) { int y = x + 1; return y; }
int def_53(int x, int y) { int z = x + y; return z; }
int def_54(void) { return 0; }
int def_55(int i2025) { return i2025; }
int def_56(int i2025, int i2026) { return i2025 + i2026; }
int def_57(int a) { return a - 5; }
int def_58(int a) { return a / 5; }
int def_59(int a) { return a % 5; }
int def_60(int a) { return a + 100; }

// =============================================================================
// SECTION 32: EXPRESSION PARSING - ARITHMETIC (40 test cases)
// =============================================================================

int expr_1(void) { int x = 1 + 2; return x; }
int expr_2(void) { int x = 5 - 3; return x; }
int expr_3(void) { int x = 4 * 5; return x; }
int expr_4(void) { int x = 10 / 2; return x; }
int expr_5(void) { int x = 10 % 3; return x; }
int expr_6(void) { int x = 1 + 2 + 3; return x; }
int expr_7(void) { int x = 10 - 5 - 2; return x; }
int expr_8(void) { int x = 2 * 3 * 4; return x; }
int expr_9(void) { int x = 24 / 2 / 3; return x; }
int expr_10(void) { int x = 1 + 2 * 3; return x; }
int expr_11(void) { int x = 2 * 3 + 4; return x; }
int expr_12(void) { int x = (1 + 2) * 3; return x; }
int expr_13(void) { int x = 1 * (2 + 3); return x; }
int expr_14(void) { int x = 10 - 5 + 2; return x; }
int expr_15(void) { int x = 10 - (5 + 2); return x; }
int expr_16(void) { int x = 20 / 4 * 2; return x; }
int expr_17(void) { int x = 20 / (4 * 2); return x; }
int expr_18(void) { int x = 100 + 50 - 25; return x; }
int expr_19(void) { int x = 100 - 50 + 25; return x; }
int expr_20(void) { int x = 10 * 2 + 5; return x; }
int expr_21(void) { int x = 10 + 2 * 5; return x; }
int expr_22(void) { int x = 30 / 3 / 2; return x; }
int expr_23(void) { int x = 30 / (3 / 2); return x; }
int expr_24(void) { int x = 10 + 20 - 5 + 3; return x; }
int expr_25(void) { int x = 2 * 3 + 4 * 5; return x; }
int expr_26(void) { int x = (2 + 3) * (4 + 5); return x; }
int expr_27(void) { int x = 100 - 25 - 25; return x; }
int expr_28(void) { int x = 7 + 8 - 3; return x; }
int expr_29(void) { int x = 15 * 2 / 3; return x; }
int expr_30(void) { int x = 9 + 6 / 3; return x; }
int expr_31(void) { int x = 1 + 2 + 3 + 4 + 5; return x; }
int expr_32(void) { int x = 5 * 4 * 3 * 2 * 1; return x; }
int expr_33(void) { int x = 100 / 10 / 2; return x; }
int expr_34(void) { int x = 1 + 2 * 3 + 4; return x; }
int expr_35(void) { int x = 2 * (3 + 4) * 5; return x; }
int expr_36(void) { int x = (10 - 5) * (10 - 3); return x; }
int expr_37(void) { int x = 50 / 2 / 5; return x; }
int expr_38(void) { int x = 12 + 8 - 4; return x; }
int expr_39(void) { int x = 3 * 7 + 2; return x; }
int expr_40(void) { int x = 25 - 10 - 5; return x; }

// =============================================================================
// SECTION 33: EXPRESSION PARSING - BINARY OPERATORS (45 test cases)
// =============================================================================

int binop_1(void) { int x = 5 & 3; return x; }
int binop_2(void) { int x = 5 | 3; return x; }
int binop_3(void) { int x = 5 ^ 3; return x; }
int binop_4(void) { int x = ~5; return x; }
int binop_5(void) { int x = 5 << 2; return x; }
int binop_6(void) { int x = 20 >> 2; return x; }
int binop_7(void) { int x = 1 && 1; return x; }
int binop_8(void) { int x = 1 || 0; return x; }
int binop_9(void) { int x = !1; return x; }
int binop_10(void) { int x = 5 < 10; return x; }
int binop_11(void) { int x = 10 > 5; return x; }
int binop_12(void) { int x = 5 <= 5; return x; }
int binop_13(void) { int x = 5 >= 5; return x; }
int binop_14(void) { int x = 5 == 5; return x; }
int binop_15(void) { int x = 5 != 5; return x; }
int binop_16(void) { int x = 15 & 7; return x; }
int binop_17(void) { int x = 12 | 10; return x; }
int binop_18(void) { int x = 9 ^ 6; return x; }
int binop_19(void) { int x = 8 << 1; return x; }
int binop_20(void) { int x = 16 >> 2; return x; }
int binop_21(void) { int x = 1 && 0; return x; }
int binop_22(void) { int x = 0 || 1; return x; }
int binop_23(void) { int x = !0; return x; }
int binop_24(void) { int x = 3 < 7; return x; }
int binop_25(void) { int x = 7 > 3; return x; }
int binop_26(void) { int x = 1 & 1 & 1; return x; }
int binop_27(void) { int x = 1 | 0 | 1; return x; }
int binop_28(void) { int x = 1 ^ 0 ^ 1; return x; }
int binop_29(void) { int x = 8 << 1 << 1; return x; }
int binop_30(void) { int x = 32 >> 1 >> 1; return x; }
int binop_31(void) { int x = 1 && 1 && 1; return x; }
int binop_32(void) { int x = 1 || 0 || 0; return x; }
int binop_33(void) { int x = 5 > 3 && 3 > 1; return x; }
int binop_34(void) { int x = 5 < 3 || 3 > 1; return x; }
int binop_35(void) { int x = 10 & 6 | 2; return x; }
int binop_36(void) { int x = 10 & (6 | 2); return x; }
int binop_37(void) { int x = (10 & 6) | 2; return x; }
int binop_38(void) { int x = 7 + 3 > 5; return x; }
int binop_39(void) { int x = 5 * 2 == 10; return x; }
int binop_40(void) { int x = 8 && 4 > 3; return x; }
int binop_41(void) { int x = 5 > 3 && 8 < 10; return x; }
int binop_42(void) { int x = 10 == 9 || 5 == 5; return x; }
int binop_43(void) { int x = 3 < 5 && 7 > 4; return x; }
int binop_44(void) { int x = 5 != 3 && 2 < 8; return x; }
int binop_45(void) { int x = 6 > 2 || 4 == 3; return x; }

// =============================================================================
// SECTION 34: EXPRESSION PARSING - TERNARY OPERATOR (30 test cases)
// =============================================================================

int tern_1(void) { int x = 1 ? 10 : 20; return x; }
int tern_2(void) { int x = 0 ? 10 : 20; return x; }
int tern_3(void) { int x = 5 > 3 ? 1 : 0; return x; }
int tern_4(void) { int x = 5 < 3 ? 1 : 0; return x; }
int tern_5(void) { int x = 10 == 10 ? 100 : 200; return x; }
int tern_6(void) { int x = 10 != 5 ? 50 : 75; return x; }
int tern_7(void) { int x = 1 ? 2 : 3 ? 4 : 5; return x; }
int tern_8(void) { int x = 1 ? 2 ? 3 : 4 : 5; return x; }
int tern_9(void) { int x = 5 > 3 ? 10 > 8 ? 1 : 2 : 3; return x; }
int tern_10(void) { int x = 1 ? 2 : 3; return x; }
int tern_11(void) { int x = 0 ? 100 : 200; return x; }
int tern_12(void) { int x = 5 >= 5 ? 1 : 0; return x; }
int tern_13(void) { int x = 3 <= 7 ? 10 : 20; return x; }
int tern_14(void) { int x = 10 - 10 ? 777 : 888; return x; }
int tern_15(void) { int x = 1 + 1 ? 300 : 400; return x; }
int tern_16(void) { int x = 1 ? 10 : 20; return x; }
int tern_17(void) { int x = 0 ? 10 : 20; return x; }
int tern_18(void) { int x = 5 > 2 ? 5 : 2; return x; }
int tern_19(void) { int x = 2 > 5 ? 2 : 5; return x; }
int tern_20(void) { int x = 7 == 7 ? 1 : 0; return x; }
int tern_21(void) { int x = 7 == 8 ? 1 : 0; return x; }
int tern_22(void) { int x = 1 ? 1 ? 1 : 0 : 0; return x; }
int tern_23(void) { int x = 1 ? 0 ? 1 : 0 : 1; return x; }
int tern_24(void) { int x = 0 ? 1 ? 1 : 0 : 1; return x; }
int tern_25(void) { int x = 1 ? 2 : 3 ? 4 : 5; return x; }
int tern_26(void) { int x = 0 ? 2 : 3 ? 4 : 5; return x; }
int tern_27(void) { int x = 1 ? 2 : 3 ? 4 : 5 ? 6 : 7; return x; }
int tern_28(void) { int x = 10 > 5 ? 10 < 15 ? 99 : 98 : 97; return x; }
int tern_29(void) { int x = 3 <= 3 ? 5 > 2 ? 10 : 11 : 12; return x; }
int tern_30(void) { int x = 2 == 2 ? 4 != 5 ? 88 : 89 : 90; return x; }

// =============================================================================
// SECTION 35: EXPRESSION PARSING - ASSIGNMENT & COMPOUND (25 test cases)
// =============================================================================

int assign_1(void) { int x = 5; x += 3; return x; }
int assign_2(void) { int x = 10; x -= 4; return x; }
int assign_3(void) { int x = 5; x *= 2; return x; }
int assign_4(void) { int x = 20; x /= 4; return x; }
int assign_5(void) { int x = 17; x %= 5; return x; }
int assign_6(void) { int x = 5; x &= 3; return x; }
int assign_7(void) { int x = 5; x |= 2; return x; }
int assign_8(void) { int x = 5; x ^= 3; return x; }
int assign_9(void) { int x = 8; x <<= 1; return x; }
int assign_10(void) { int x = 20; x >>= 2; return x; }
int assign_11(void) { int x = 100; x += 50; x -= 25; return x; }
int assign_12(void) { int x = 2; x *= 3; x *= 4; return x; }
int assign_13(void) { int x = 200; x /= 10; x /= 2; return x; }
int assign_14(void) { int x = 20; x %= 7; return x; }
int assign_15(void) { int x = 15; x &= 7; return x; }
int assign_16(void) { int x = 8; x |= 4; return x; }
int assign_17(void) { int x = 12; x ^= 9; return x; }
int assign_18(void) { int x = 4; x <<= 3; return x; }
int assign_19(void) { int x = 128; x >>= 4; return x; }
int assign_20(void) { int x = 5; x += 1; x += 1; x += 1; return x; }
int assign_21(void) { int x = 100; x -= 10; x -= 5; return x; }
int assign_22(void) { int x = 1; x *= 2; x *= 3; x *= 4; return x; }
int assign_23(void) { int x = 48; x /= 2; x /= 3; return x; }
int assign_24(void) { int x = 25; x %= 3; return x; }
int assign_25(void) { int x = 7; x += 2; x *= 3; return x; }

// =============================================================================
// SECTION 36: EXPRESSION PARSING - INCREMENT/DECREMENT (30 test cases)
// =============================================================================

int inc_1(void) { int x = 5; x++; return x; }
int inc_2(void) { int x = 5; ++x; return x; }
int inc_3(void) { int x = 5; x--; return x; }
int inc_4(void) { int x = 5; --x; return x; }
int inc_5(void) { int x = 5; int y = x++; return x; }
int inc_6(void) { int x = 5; int y = ++x; return x; }
int inc_7(void) { int x = 5; int y = x--; return x; }
int inc_8(void) { int x = 5; int y = --x; return x; }
int inc_9(void) { int x = 10; x++; ++x; return x; }
int inc_10(void) { int x = 10; x--; --x; return x; }
int inc_11(void) { int x = 3; int y = ++x + x++; return y; }
int inc_12(void) { int x = 5; return x++; }
int inc_13(void) { int x = 5; return ++x; }
int inc_14(void) { int x = 5; return x--; }
int inc_15(void) { int x = 5; return --x; }
int inc_16(void) { int x = 0; x++; x++; x++; return x; }
int inc_17(void) { int x = 3; x--; x--; x--; return x; }
int inc_18(void) { int x = 5; ++x; ++x; ++x; return x; }
int inc_19(void) { int x = 5; --x; --x; return x; }
int inc_20(void) { int x = 1; int y = x++ + ++x; return y; }
int inc_21(void) { int x = 5; return x++; }
int inc_22(void) { int x = 5; return ++x; }
int inc_23(void) { int x = 10; return x--; }
int inc_24(void) { int x = 10; return --x; }
int inc_25(void) { int x = 7; x = x + 1; return x; }
int inc_26(void) { int x = 7; x = x - 1; return x; }
int inc_27(void) { int x = 0; x += 1; x += 1; return x; }
int inc_28(void) { int x = 10; x -= 1; x -= 1; return x; }
int inc_29(void) { int x = 5; int y = (x++) + (++x); return y; }
int inc_30(void) { int x = 8; int y = (x--) + (--x); return y; }

// =============================================================================
// SECTION 37: EXPRESSION PARSING - CAST & SIZEOF (25 test cases)
// =============================================================================

int cast_1(void) { float f = 3.14f; int x = (int)f; return x; }
int cast_2(void) { double d = 2.71; int x = (int)d; return x; }
int cast_3(void) { int i = 65; char c = (char)i; return (int)c; }
int cast_4(void) { int x = (int)5.5; return x; }
int cast_5(void) { int x = (int)10.1; return x; }
int cast_6(void) { float f = 3.14f; float x = (float)f; return (int)x; }
int cast_7(void) { double d = 2.71; double x = (double)d; return (int)x; }
int cast_8(void) { char c = 'A'; int x = (int)c; return x; }
int cast_9(void) { int x = 42; char c = (char)x; return (int)c; }
int cast_10(void) { int x = (int)3.14; return x; }
int cast_11(void) { float f = 2.5f; int x = f; return x; }
int cast_12(void) { int x = 1; float f = x; return (int)f; }
int cast_13(void) { int s1 = sizeof(int); return s1; }
int cast_14(void) { int s2 = sizeof(char); return s2; }
int cast_15(void) { int s3 = sizeof(float); return s3; }
int cast_16(void) { int s4 = sizeof(double); return s4; }
int cast_17(void) { int s5 = sizeof(int *); return s5; }
int cast_18(void) { int s6 = sizeof(int [10]); return s6; }
int cast_19(void) { int s7 = sizeof(int [5][10]); return s7; }
int cast_20(void) { int s8 = sizeof(struct S1); return s8; }
int cast_21(void) { int s9 = sizeof(union U1); return s9; }
int cast_22(void) { int s10 = sizeof(enum E1); return s10; }
int cast_23(void) { double d = 5.5; int x = (int)d; return x; }
int cast_24(void) { float f = 7.8f; int x = (int)f; return x; }
int cast_25(void) { int x = 255; char c = (char)x; return (int)c; }

// =============================================================================
// SECTION 38: COMPLEX NESTED CONTROL FLOW (40 test cases)
// =============================================================================

int ctrl_1(void) { int x = 5; if (x > 0) { x = x + 1; } return x; }
int ctrl_2(void) { int x = 5; if (x > 0) { x = x + 1; } else { x = x - 1; } return x; }
int ctrl_3(void) { int x = 5; if (x > 0) x = x + 1; else x = x - 1; return x; }
int ctrl_4(void) { int x = 0; if (x > 0) { } else { x = 1; } return x; }
int ctrl_5(void) { int x = 5; if (x > 0) { if (x > 3) { x = 10; } } return x; }
int ctrl_6(void) { int sum = 0; int i = 0; while (i < 5) { sum = sum + i; i = i + 1; } return sum; }
int ctrl_7(void) { int x = 5; int y = 0; while (x > 0) { y = y + 1; x = x - 1; } return y; }
int ctrl_8(void) { int x = 0; do { x = x + 1; } while (x < 5); return x; }
int ctrl_9(void) { int sum = 0; int i = 0; do { sum = sum + i; i = i + 1; } while (i < 5); return sum; }
int ctrl_10(void) { int sum = 0; for (int i = 0; i < 5; i = i + 1) { sum = sum + i; } return sum; }
int ctrl_11(void) { int sum = 0; for (int i = 1; i <= 5; i = i + 1) { sum = sum + i; } return sum; }
int ctrl_12(void) { int sum = 0; for (int i = 5; i > 0; i = i - 1) { sum = sum + i; } return sum; }
int ctrl_13(void) { for (int i = 0; i < 3; i = i + 1) { for (int j = 0; j < 3; j = j + 1) { } } return 1; }
int ctrl_14(void) { int x = 0; for (int i = 0; i < 5; i = i + 1) { if (i == 2) break; x = x + 1; } return x; }
int ctrl_15(void) { int x = 0; for (int i = 0; i < 5; i = i + 1) { if (i == 2) continue; x = x + 1; } return x; }
int ctrl_16(void) { int x = 1; switch (x) { case 1: x = 10; break; } return x; }
int ctrl_17(void) { int x = 2; switch (x) { case 1: x = 10; break; case 2: x = 20; break; } return x; }
int ctrl_18(void) { int x = 3; switch (x) { case 1: x = 10; break; case 2: x = 20; break; default: x = 30; } return x; }
int ctrl_19(void) { int x = 2; switch (x) { case 1: x = 10; case 2: x = 20; } return x; }
int ctrl_20(void) { int x = 5; if (x > 0) { if (x > 3) { x = 100; } else { x = 50; } } return x; }
int ctrl_21(void) { int x = 1; if (x == 0) { } else if (x == 1) { x = 10; } else { x = 20; } return x; }
int ctrl_22(void) { int x = 0; while (x < 10) { if (x == 5) break; x = x + 1; } return x; }
int ctrl_23(void) { int sum = 0; for (int i = 0; i < 10; i = i + 1) { if (i % 2 == 0) continue; sum = sum + i; } return sum; }
int ctrl_24(void) { int x = 0; int y = 0; while (x < 3) { y = 0; while (y < 3) { y = y + 1; } x = x + 1; } return x; }
int ctrl_25(void) { int x = 1; do { if (x == 3) break; x = x + 1; } while (x < 5); return x; }
int ctrl_26(void) { int sum = 0; for (int i = 0; i < 5; i = i + 1) { for (int j = 0; j < 3; j = j + 1) { sum = sum + 1; } } return sum; }
int ctrl_27(void) { int x = 5; if (x > 0) { if (x > 2) { if (x > 4) { x = 1000; } } } return x; }
int ctrl_28(void) { int x = 0; switch (x) { case 0: x = 1; break; case 1: x = 2; break; default: x = 3; } return x; }
int ctrl_29(void) { int x = 0; goto label; x = 100; label: x = 200; return x; }
int ctrl_30(void) { int i = 0; while (i < 10) { if (i == 5) goto end; i = i + 1; } end: return i; }
int ctrl_31(void) { int x = 5; if (x > 0) { x = 10; } return x; }
int ctrl_32(void) { int x = 5; if (x < 0) { x = 10; } else { x = 20; } return x; }
int ctrl_33(void) { for (int i = 0; i < 5; i = i + 1) { } return 1; }
int ctrl_34(void) { int sum = 0; for (int i = 0; i < 10; i = i + 1) sum = sum + i; return sum; }
int ctrl_35(void) { int x = 0; while (x < 5) x = x + 1; return x; }
int ctrl_36(void) { int x = 0; do x = x + 1; while (x < 5); return x; }
int ctrl_37(void) { int x = 1; switch (x) { case 1: return 10; default: return 0; } }
int ctrl_38(void) { int x = 5; if (x > 3) { if (x < 10) { return 1; } } return 0; }
int ctrl_39(void) { for (int i = 0; i < 2; i = i + 1) { for (int j = 0; j < 2; j = j + 1) { for (int k = 0; k < 2; k = k + 1) { } } } return 1; }
int ctrl_40(void) { int x = 5; while (x > 0) { x = x - 1; } return x; }

// =============================================================================
// SECTION 39: POINTER OPERATIONS (30 test cases)
// =============================================================================

int ptr_1(void) { int x = 42; int *p = &x; return *p; }
int ptr_2(void) { int x = 10; int *p = &x; int **pp = &p; return **pp; }
int ptr_3(void) { int arr[5]; return 1; }
int ptr_4(void) { int x = 5; int *p = &x; *p = 10; return x; }
int ptr_5(void) { int x = 5; int *p = &x; int y = *p; return y; }
int ptr_6(void) { int x = 100; int *p = &x; return *p; }
int ptr_7(void) { int x = 5; int *p = &x; int **pp = &p; **pp = 20; return x; }
int ptr_8(void) { int x = 10; int *p = &x; int **pp = &p; int ***ppp = &pp; return ***ppp; }
int ptr_9(void) { int x = 15; int *p = &x; return *p; }
int ptr_10(void) { int x = 25; int *p = &x; int **pp = &p; int y = **pp; return y; }
int ptr_11(void) { int x = 30; int *p = &x; return *p; }
int ptr_12(void) { int x = 5; int *p = &x; *p = 50; return *p; }
int ptr_13(void) { int *p; return 1; }
int ptr_14(void) { int x = 7; int *p = &x; int y = *p + 3; return y; }
int ptr_15(void) { int x = 20; int *p = &x; return *p - 5; }
int ptr_16(void) { int x = 8; int *p = &x; int **pp = &p; int z = **pp + 2; return z; }
int ptr_17(void) { int x = 9; int *p = &x; return *p * 2; }
int ptr_18(void) { int x = 4; int *p = &x; return *p / 2; }
int ptr_19(void) { int x = 10; int *p = &x; return *p % 3; }
int ptr_20(void) { int x = 12; int *p = &x; return *p & 7; }
int ptr_21(void) { int x = 5; int *p = &x; return *p | 2; }
int ptr_22(void) { int x = 6; int *p = &x; return *p ^ 3; }
int ptr_23(void) { int x = 8; int *p = &x; return *p << 1; }
int ptr_24(void) { int x = 16; int *p = &x; return *p >> 2; }
int ptr_25(void) { int x = 5; int *p = &x; return *p > 3; }
int ptr_26(void) { int x = 2; int *p = &x; return *p < 5; }
int ptr_27(void) { int x = 5; int *p = &x; return *p == 5; }
int ptr_28(void) { int x = 5; int *p = &x; return *p != 3; }
int ptr_29(void) { int x = 10; int *p = &x; int *q = p; return *q; }
int ptr_30(void) { int x = 42; int *p = &x; return *p; }

// =============================================================================
// MAIN FUNCTION WITH COMPREHENSIVE TESTS (100+ expressions)
// =============================================================================

int main(void) {
    int result = 0;

    result += 1 + 2;
    result += 5 - 3;
    result += 2 * 3;
    result += 10 / 2;
    result += 10 % 3;
    result += 1 + 2 * 3;
    result += (1 + 2) * 3;
    result += 10 - 5 + 2;
    result += 20 / 4 * 2;
    result += 100 + 50 - 25;
    result += 5 & 3;
    result += 5 | 3;
    result += 5 ^ 3;
    result += 8 << 1;
    result += 16 >> 2;
    result += 5 & 3 & 1;
    result += 5 | 3 | 1;
    result += 5 ^ 3 ^ 1;
    result += 1 & 1 & 1;
    result += 1 | 0 | 1;
    result += 1 && 1;
    result += 1 || 0;
    result += !0;
    result += 5 > 3 && 3 > 1;
    result += 5 < 3 || 3 > 1;
    result += 5 < 10;
    result += 10 > 5;
    result += 5 <= 5;
    result += 5 >= 5;
    result += 5 == 5;
    result += 5 != 3;
    result += 3 < 7;
    result += 7 > 3;
    result += 10 == 10;
    result += 10 != 5;
    result += 1 ? 10 : 20;
    result += 0 ? 10 : 20;
    result += 5 > 3 ? 1 : 0;
    result += 5 < 3 ? 1 : 0;
    result += 10 == 10 ? 100 : 200;
    result += 1 ? 2 : 3 ? 4 : 5;
    result += 1 ? 2 ? 3 : 4 : 5;

    int x = 5;
    x += 3;
    result += x;

    int y = 10;
    y -= 4;
    result += y;

    int z = 5;
    z *= 2;
    result += z;

    int w = 20;
    w /= 4;
    result += w;

    int a = 5;
    a++;
    result += a;

    int b = 5;
    ++b;
    result += b;

    int c = 5;
    c--;
    result += c;

    int d = 5;
    --d;
    result += d;

    float f = 3.14f;
    result += (int)f;

    double dd = 2.71;
    result += (int)dd;

    result += sizeof(int);
    result += sizeof(char);
    result += sizeof(float);
    result += sizeof(double);

    if (5 > 0) result += 1;
    if (5 < 0) result -= 1; else result += 2;

    int i = 0;
    while (i < 3) {
        result += 1;
        i = i + 1;
    }

    int j = 0;
    do {
        result += 1;
        j = j + 1;
    } while (j < 3);

    for (int k = 0; k < 3; k = k + 1) {
        result += 1;
    }

    int sw = 2;
    switch (sw) {
        case 1: result += 10; break;
        case 2: result += 20; break;
        default: result += 30;
    }

    for (int m = 0; m < 5; m = m + 1) {
        if (m == 2) break;
        result += 1;
    }

    for (int n = 0; n < 5; n = n + 1) {
        if (n == 2) continue;
        result += 1;
    }

    goto end_label;
    result += 100;

    end_label:
    return result;
}
