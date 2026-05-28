// rm129

// Basic variable declarations with optional initialization.
// Tests simple type variables (int, char, float, double, etc) with and without initializers.
// Coverage: fundamental variable declaration patterns.

// ===== INT TYPE (10 cases) =====
int ut_01_var1;
int ut_01_var2 = 0;
int ut_01_var3 = 1;
int ut_01_var4 = 100;
int ut_01_var5 = 9999;
int ut_01_var6 = -1;
int ut_01_var7 = -100;
int ut_01_var8 = 0xFF;
int ut_01_var9 = 0777;
int ut_01_var10 = 2147483647;

// ===== CHAR TYPE (10 cases) =====
char ut_01_var11;
char ut_01_var12 = '\0';
char ut_01_var13 = 'a';
char ut_01_var14 = 'Z';
char ut_01_var15 = '0';
char ut_01_var16 = '\n';
char ut_01_var17 = '\t';
char ut_01_var18 = 65;
char ut_01_var19 = -1;
char ut_01_var20 = 127;

// ===== FLOAT TYPE (9 cases) =====
float ut_01_var21;
float ut_01_var22 = 0.0;
float ut_01_var23 = 0.5;
float ut_01_var24 = 3.14;
float ut_01_var25 = 9.99;
float ut_01_var26 = -1.5;
float ut_01_var27 = 1e-3;
float ut_01_var28 = 2e5;
float ut_01_var29 = 1e10;

// ===== DOUBLE TYPE (9 cases) =====
double ut_01_var30;
double ut_01_var31 = 0.0;
double ut_01_var32 = 0.5;
double ut_01_var33 = 2.718;
double ut_01_var34 = 3.14159;
double ut_01_var35 = -2.71828;
double ut_01_var36 = 1e-5;
double ut_01_var37 = 2.5e10;
double ut_01_var38 = 1.23e-20;

// ===== SHORT TYPE (11 cases) =====
short ut_01_var39;
short ut_01_var40 = 0;
short ut_01_var41 = 1;
short ut_01_var42 = 100;
short ut_01_var43 = 1000;
short ut_01_var44 = 32767;
short ut_01_var45 = -1;
short ut_01_var46 = -100;
short ut_01_var47 = -32768;
short ut_01_var48 = 0x7FFF;
short ut_01_var49 = 0777;

// ===== LONG TYPE (12 cases) =====
long ut_01_var50;
long ut_01_var51 = 0;
long ut_01_var52 = 1;
long ut_01_var53 = 100000;
long ut_01_var54 = 2147483647;
long ut_01_var55 = -1;
long ut_01_var56 = -100000;
long ut_01_var57 = -2147483648;
long ut_01_var58 = 0xFF;
long ut_01_var59 = 0x7FFFFFFF;
long ut_01_var60 = 12345678;
long ut_01_var61 = 999999999;

// ===== SIGNED TYPE (11 cases) =====
signed ut_01_var62;
signed ut_01_var63 = 0;
signed ut_01_var64 = 1;
signed ut_01_var65 = 100;
signed ut_01_var66 = 999999;
signed ut_01_var67 = -1;
signed ut_01_var68 = -100000;
signed ut_01_var69 = 0xFF;
signed ut_01_var70 = 0777;
signed ut_01_var71 = 2147483647;
signed ut_01_var72 = -2147483648;

// ===== UNSIGNED TYPE (8 cases) =====
unsigned ut_01_var73;
unsigned ut_01_var74 = 0;
unsigned ut_01_var75 = 1;
unsigned ut_01_var76 = 100;
unsigned ut_01_var77 = 9999;
unsigned ut_01_var78 = 4294967295;
unsigned ut_01_var79 = 0xFFFFFFFF;
unsigned ut_01_var80 = 2147483647;
