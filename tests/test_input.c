// ====== FAILING TEST CASES (2 Categories) ======

// ====== CATEGORY 1: VOID TYPEDEF (Semantically Invalid) ======
// These SHOULD fail - void cannot instantiate variables
typedef void Void;

// ====== CATEGORY 2: TYPE REGISTRY EXPANSION (Not Implemented Yet) ======
// These fail because type registry expansion is not yet implemented
// Pattern: Using previously-defined typedefs as base types

// Test 2.1: Reuse typedef as base type for pointer
typedef int MyInt;
typedef MyInt* MyIntPtr;

// Test 2.2: Reuse typedef as base type for array
typedef int BaseInt;
typedef BaseInt BaseIntArray[10];

// Test 2.3: Reuse typedef as base type for function pointer
typedef int MyIntType;
typedef MyIntType (*GetMyInt)(void);

// Test 2.4: Chain typedefs (typedef reuse)
typedef int T1;
typedef T1 T2;

// Test 2.5: Struct typedef reuse
typedef struct {
    int x;
} StructPoint;
typedef StructPoint* StructPointPtr;

// Test 2.6: Named struct typedef reuse
typedef struct Node {
    int data;
} Node;
typedef Node* NodePtr;

// Test 2.7: Struct typedef in array
typedef struct {
    int val;
} Element;
typedef Element ElementArray[10];

// Test 2.8: Struct typedef in function pointer
typedef struct {
    int field;
} Record;
typedef Record* (*GetRecord)(void);

// Test 2.9: Multiple levels of typedef chaining
typedef int BaseType;
typedef BaseType Level1;
typedef Level1 Level2;

// Test 2.10: Pointer typedef reuse
typedef int* IntPtr;
typedef IntPtr* IntPtrPtr;

// Test 2.11: Array typedef reuse
typedef int IntArr[10];
typedef IntArr IntArrOfArr[10];

// Test 2.12: Function pointer typedef reuse
typedef int (*Func)(int);
typedef Func* FuncPtr;

// Test 2.13: Complex reuse - struct member access
typedef struct {
    int x;
} Point;
typedef Point Points[100];

// Test 2.14: Reuse in complex declarators
typedef float FloatType;
typedef FloatType* (*ComplexFunc)(void);

// Test 2.15: Struct with previously-defined typedef member type (might work)
typedef struct {
    int x;
} SimpleStruct;

typedef struct {
    SimpleStruct nested;
    int y;
} ComplexStructWithTypedef;
