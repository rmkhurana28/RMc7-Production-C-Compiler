// ====== COMPREHENSIVE TESTING (EXCLUDING VOID TYPEDEF & TYPEDEF REUSE) ======

// GROUP 1: PRIMITIVE TYPEDEFS
typedef int MyInt;
typedef unsigned int UInt;
typedef long Long;
typedef unsigned long ULong;
typedef long long LongLong;
typedef unsigned long long ULongLong;
typedef short Short;
typedef unsigned short UShort;
typedef char Char;
typedef float Float;
typedef double Double;

// GROUP 2: POINTER TYPEDEFS
typedef int* IntPtr;
typedef int** IntPtrPtr;
typedef int*** IntPtrPtrPtr;
typedef int **** IntPtrPtrPtrPtr;
typedef char* CharPtr;
typedef void* VoidPtr;

// GROUP 3: ARRAY TYPEDEFS
typedef int IntArray[10];
typedef int IntMatrix[5][5];
typedef int Int3D[3][4][5];
typedef int Int4D[2][3][4][5];

// GROUP 4: FUNCTION POINTER TYPEDEFS
typedef int (*BinaryOp)(int, int);
typedef void (*Callback)(void);
typedef int (*GetValue)(void);
typedef void (*SetValue)(int);
typedef int (*VoidFunc)(void);
typedef int* (*GetPtr)(void);
typedef int (*MultiParam)(int, int, int, int, int);

// GROUP 5: COMPLEX POINTER/ARRAY MIX
typedef int* IntPtrArray[10];
typedef int (*IntArrayPtr)[10];
typedef int (*IntArrayPtr2D)[5][5];
typedef int *(*IntPtrArrayPtr)[10];

// GROUP 6: FUNCTION POINTER ARRAYS
typedef int (*FuncPtrArray[10])(void);
typedef int (*FuncPtr2D[5][5])(int);

// GROUP 7: NESTED FUNCTION POINTERS
typedef int (*(*NestedFuncPtr)(void))(int);
typedef int (*(*NestedFuncPtr2)(int))(int, int);

// GROUP 8: QUALIFIERS
typedef const int ConstInt;
typedef volatile int VolInt;
typedef const volatile int CVInt;
typedef const int* ConstIntPtr;
typedef int* const IntConstPtr;
typedef volatile int* VolIntPtr;
typedef int* volatile IntVolPtr;
typedef const int** ConstIntPtrPtr;
typedef int* const* IntConstPtrPtr;
typedef const volatile int* CVIntPtr;
typedef const int ConstIntArray[10];
typedef volatile int VolIntArray[5];
typedef const int* ConstIntPtrArray[10];

// GROUP 9: RESTRICT QUALIFIER
typedef int* restrict RestrictPtr;
typedef int** restrict RestrictPtrPtr;

// GROUP 10: EXTREME NESTING
typedef int (*(*Level2)(void))(int);
typedef int (*(*(*Level3)(void))(int))(int);
typedef int (*(*(*Level4Func)(int))(int, int))(void);

// GROUP 11: VARIADIC FUNCTION POINTERS
typedef int (*VarFunc)(int, ...);
typedef void (*VarNoReturn)(const char*, ...);

// GROUP 12: SIMPLE ANONYMOUS STRUCTS
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    float r;
    float g;
    float b;
} Color;

typedef struct {
    int width;
    int height;
} Size;

// GROUP 13: ANONYMOUS STRUCTS WITH MULTIPLE NAMES
typedef struct {
    int a;
    int b;
} Pair, TwoParts;

typedef struct {
    double x;
    double y;
    double z;
} Vector3D, V3D, ThreeD;

// GROUP 14: ANONYMOUS STRUCTS WITH POINTER DECLARATORS
typedef struct {
    int data;
} Node, *NodePtr;

typedef struct {
    float value;
} Cell, *CellPtr, **CellPtrPtr;

// GROUP 15: ANONYMOUS STRUCTS WITH ARRAY DECLARATORS
typedef struct {
    int id;
} Item, ItemArray[20];

typedef struct {
    char name[50];
} Entry, EntryArray[100];

// GROUP 16: MIXED DECLARATORS
typedef struct {
    int val;
} Value, *ValPtr, ValArr[10];

typedef struct {
    int x;
} Coord, *CoordPtr, CoordArr[5], **CoordPtrPtr;

// GROUP 17: NAMED STRUCTS
typedef struct Point2D {
    int x;
    int y;
} Point2D;

typedef struct Color3 {
    int r;
    int g;
    int b;
} Color3;

typedef struct Node {
    int data;
    int next;
} Node;

// GROUP 18: NAMED STRUCTS WITH MULTIPLE NAMES
typedef struct LinkedList {
    int value;
    int link;
} LinkedList, LL, List;

typedef struct TreeNode {
    int val;
    int left;
    int right;
} TreeNode, TNode;

// GROUP 19: NAMED STRUCTS WITH POINTER ALIASES
typedef struct DataNode {
    int info;
} DataNode, *DataNodePtr;

// GROUP 20: SELF-REFERENCING STRUCTS
typedef struct ListNode {
    int data;
    struct ListNode *next;
} ListNode;

typedef struct TreeNodeSelf {
    int value;
    struct TreeNodeSelf *left;
    struct TreeNodeSelf *right;
} TreeNodeSelf;

typedef struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

// GROUP 21: STRUCT WITH FUNCTION POINTER MEMBERS
typedef struct {
    int (*callback)(int);
} HandlerSimple;

typedef struct {
    int (*init)(void);
    int (*process)(int);
    void (*cleanup)(void);
} Handler;

typedef struct {
    int (*op1)(int, int);
    int (*op2)(int, int);
    void (*display)(void);
} Operations;

// GROUP 22: STRUCT WITH QUALIFIED MEMBERS
typedef struct {
    const int id;
    int value;
} ConstMembers;

typedef struct {
    volatile int port;
    int data;
} VolMembers;

typedef struct {
    const volatile int reg;
} CVMembers;

// GROUP 23: STRUCT WITH POINTER MEMBERS
typedef struct {
    int *ptr;
    int **pptr;
    int ***ppptr;
} PointerMembers;

// GROUP 24: STRUCT WITH ARRAY MEMBERS
typedef struct {
    int arr[10];
    int matrix[5][5];
    int tensor[3][4][5];
} ArrayMembers;

// GROUP 25: STRUCT WITH MIXED MEMBERS
typedef struct {
    int data;
    int *ptr;
    int arr[10];
    int (*func)(int);
} MixedMembers;

// GROUP 26: COMPLEX STRUCT COMBINATIONS
typedef struct {
    int arr[10];
    int *ptr;
    int **pptr;
    int (*callback)(int, int);
} ComplexLayout;

typedef struct {
    const int* const_ptr;
    volatile int* vol_ptr;
    int* const* const_ptr_ptr;
} QualifiedPtrs;

// GROUP 27: MULTIPLE DECLARATORS ON COMPLEX TYPES
typedef struct {
    int x;
} S, *SP, SA[5], (*SF)(void);

typedef struct Named {
    int y;
} N, *NP, NA[10];
