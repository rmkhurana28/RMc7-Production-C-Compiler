//rm129
// Unit Test Case - Void Type Declarations
// Purpose: Test special rules for void type (requires pointer for variables)
// Coverage: void *, void **, but NOT plain void variable
// Expected: void * accepted, plain void variable should be rejected (or function only)

int main() {
    void *x;
    void **y;
    const void *z;
    volatile void *w;
    return 0;
}
