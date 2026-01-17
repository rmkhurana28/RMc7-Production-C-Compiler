//rm129
// Unit Test Case - Type Qualifiers
// Purpose: Test type qualifiers (const, volatile, restrict) in declarations
// Coverage: Qualifiers before/after pointers, multiple qualifiers
// Expected: Parser accepts valid qualifier positions and combinations

int main() {
    const int x;
    volatile int y;
    const volatile int z;
    int * const w;
    const int * a;
    volatile int * b;
    const int * const c;
    volatile int * volatile d;
    int * restrict e;
    return 0;
}
