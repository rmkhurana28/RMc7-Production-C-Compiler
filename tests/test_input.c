// ====== COMPREHENSIVE COMPLEX TYPEDEF TESTS ======
// Testing deep nesting, complex declarators, qualifiers, etc.
// Now that implicit 'int' is fixed, testing full complexity

// ====== LEVEL 1: MULTI-LEVEL POINTERS ======
typedef int*** TriplePtr;
typedef int**** QuadruplePtr;
typedef int***** FivePtr;

// ====== LEVEL 2: POINTERS WITH QUALIFIERS ======
typedef const int* ConstIntPtr;
typedef int* const ConstPtr;
typedef volatile int* VolPtr;
typedef const volatile int* CVPtr;

// ====== LEVEL 3: ARRAYS ======
typedef int Arr1D[10];
typedef int Arr2D[3][4];
typedef int Arr3D[2][3][4];
typedef int Arr4D[2][3][4][5];

// ====== LEVEL 4: POINTERS TO ARRAYS ======
typedef int (*PtrToArr1D)[10];
typedef int (*PtrToArr2D)[3][4];
typedef int (*PtrToArr3D)[2][3][4];

// ====== LEVEL 5: ARRAYS OF POINTERS ======
typedef int* ArrOfPtr1D[5];
typedef int* ArrOfPtr2D[3][4];
typedef int** ArrOfPtrToPtr[5];

// ====== LEVEL 6: FUNCTION POINTERS - BASIC ======
typedef int (*FuncPtr1)(void);
typedef int (*FuncPtr2)(int);
typedef int (*FuncPtr3)(int, int);
typedef int (*FuncPtr4)(int, int, int);
typedef void (*VoidFunc)(void);
typedef void (*VoidFunc1)(int);

// ====== LEVEL 7: FUNCTION POINTERS - WITH POINTERS ======
typedef int* (*FuncRetPtr)(void);
typedef int** (*FuncRetPtrPtr)(void);
typedef int* (*FuncPtr1Arg)(int*);
typedef int** (*FuncPtr2Arg)(int*, int**);

// ====== LEVEL 8: FUNCTION POINTERS - WITH ARRAYS ======
typedef int (*FuncArrParam)[10];
typedef int* (*FuncArrPtrParam)[5];
typedef int (*FuncArr2DParam)[3][4];

// ====== LEVEL 9: POINTERS TO FUNCTION POINTERS ======
typedef int (**PtrToFuncPtr)(void);
typedef int (**PtrToFuncPtr2)(int, int);
typedef void (**PtrToVoidFunc)(void);

// ====== LEVEL 10: ARRAYS OF FUNCTION POINTERS ======
typedef int (*ArrOfFunc[5])(void);
typedef int (*ArrOfFunc2[3])(int, int);
typedef int (*ArrOfFunc3[2][3])(void);
typedef void (*ArrOfVoidFunc[5])(int);

// ====== LEVEL 11: COMPLEX NESTED - FUNCTION POINTERS RETURNING POINTERS ======
typedef int* (*FuncPtrReturnPtr)(int);
typedef int** (*FuncPtrReturnPtrPtr)(void);
typedef int* (*FuncPtrReturnPtr2)(int, int);
typedef void* (*GenericFunc)(void);

// ====== LEVEL 12: RETURN ARRAY-LIKE PATTERNS ======
typedef int (*FuncReturnArrPtr)[10];
typedef int (*FuncReturnArr2DPtr)[3][4];

// ====== LEVEL 13: MIXED POINTER AND ARRAY NESTING ======
typedef int (*(*PtrToPtrToFunc))(void);
typedef int (*(*PtrToFuncArr)[5])(void);
typedef int* (*(*PtrToPtrFunc))(void);
typedef int (*(*PtrToFuncPtrArr)[3])(int, int);

// ====== LEVEL 14: DEEPLY NESTED POINTERS ======
typedef int* (*(**DeepPtr1)(void))(int);
typedef int* (*(**DeepPtr2)[10])(void);
typedef int (*(**DeepPtr3)(int))[10];

// ====== LEVEL 15: QUALIFIERS IN COMPLEX POSITIONS ======
typedef const int* (*FuncRetConstPtr)(void);
typedef int* const (*FuncRetConstPtrToInt)(void);
typedef volatile int* (*FuncRetVolPtr)(int);
typedef int* volatile (*FuncRetVolPtrVar)(void);

// ====== LEVEL 16: COMPLEX WITH MULTIPLE QUALIFIERS ======
typedef const volatile int** (*FuncRetCVPtrPtr)(void);
typedef const int* volatile (*FuncRetConstPtrVol)(void);
typedef volatile int* const (*FuncRetVolPtrConst)(void);

// ====== LEVEL 17: ARRAYS OF CONST/VOLATILE POINTERS ======
typedef const int* ArrConstPtr[5];
typedef int* const ArrConstPtr2[3];

// ====== LEVEL 18: POINTERS TO ARRAYS OF FUNCTIONS ======
typedef int (*(*PtrToArrOfFunc)[5])(void);
typedef void (*(*PtrToArrOfFunc2)[3])(int);
typedef int* (*(*PtrToArrOfFuncRetPtr)[4])(void);

// ====== LEVEL 19: ADVANCED COMBINATIONS ======
typedef int* (*(*(*TripleNest)(void))(int))(int);
typedef int (*(*(*PtrToFuncReturningPtrToFunc)(int))(void))(int);

// ====== LEVEL 20: EXTREME NESTING (6+ LEVELS) ======
typedef int (*(*(*(*Level4FuncPtr)(void))(int))(int))(int);
typedef int (*(*(*(*(*Level5FuncPtr)(void))(int))(int))(int))(int);
typedef int (*(*(*(*(*(*Level6FuncPtr)(void))(int))(int))(int))(int))(int);

// ====== LEVEL 21: UNSIGNED/SIGNED SIZE MODIFIERS WITH COMPLEX DECLARATORS ======
typedef unsigned long* ULPtr;
typedef unsigned long long** ULLPtrPtr;
typedef long long int* LLIntPtr;
typedef unsigned short* USPtr;
typedef signed int (*SignedFuncPtr)(void);
typedef unsigned int (*UnsignedFuncPtr)(void);

// ====== LEVEL 22: VOLATILE WITH COMPLEX PATTERNS ======
typedef volatile int* VolIntPtr[5];
typedef volatile int* (*VolFuncPtr)(void);
typedef int* volatile (*FuncRetVolPtr2)(void);

// ====== LEVEL 23: RESTRICT QUALIFIER (IF SUPPORTED) ======
typedef int* restrict RestrictPtr;
typedef int** restrict RestrictPtrPtr;

// ====== LEVEL 24: MIXED SIZE MODIFIERS WITH QUALIFIERS ======
typedef const unsigned long CLULong;
typedef volatile unsigned short VUSShort;
typedef const unsigned long long int CULLInt;

// ====== EDGE CASES ======
typedef int (*EmptyParamFunc)(void);
typedef int (*SingleParamFunc)(int);
typedef int (*MultiParamFunc)(int, int, int, int, int);
typedef void (*EvenMoreParams)(int, int, int, int, int, int, int);
