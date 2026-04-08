// =============================================================================
// FINAL PHASE 2 — COMPLEX DECLARATOR + TYPEDEF TESTS (RMc7 VALID)
// =============================================================================

// ---------- Function parameters (named, complex allowed) ----------
int p1(int (*fp)(int));
int p2(int (*fp[5])(int));
int p3(int (*(*fp)(int))[5]);
int p4(int (*(*(*fp)(int))[5])(int));

// ---------- Typedef inside parameter ----------
typedef int *TP2;

int p5(TP2 a);
int p6(TP2 *a);
int p7(TP2 (*a[5]));
int p8(TP2 (*(*a)[5]));

// ---------- Function returning array pointer via typedef ----------
typedef int (*ARR10)[10];

ARR10 r1(int a);
ARR10 *r2(int a);
ARR10 (*r3[5])(int);
ARR10 (*(*r4)[10])(int);

// ---------- Mixed typedef chains with arrays ----------
typedef int *A1;
typedef A1 A2[5];
typedef A2 *A3;

A3 x1;
A3 *x2;
A3 x3[10];

// ---------- Typedef hiding function pointer ----------
typedef int (*Hidden)(int);
typedef Hidden HiddenArr[5];
typedef HiddenArr *HiddenPtr;

HiddenPtr y1;
HiddenPtr *y2;
HiddenPtr y3[3];

// ---------- Multi-level indirect function madness ----------
int (*(*(*m1)(int))(int))(int);
int (*(*(*(*m2)(int))[5])(int))(int);
int (*(*(*(*(*m3)(int))[5])(int))[10])(int);

// ---------- Array of function returning pointer to array ----------
int (*(*af1[5])(int))[10];
int (*(*(*af2[5])(int))[10])[20];
int (*(*(*(*af3[5])(int))[10])[20])[30];

// ---------- Function parameter returning function pointer (named param) ----------
int (*pf1(int (*fp)(int)))(int);
int (*(*pf2(int (*fp)(int)))(int))(int);