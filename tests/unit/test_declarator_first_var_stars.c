//rm129
// Unit Test Case - First Variable Stars (Session 19 Fix)
// Purpose: Test that stars in base type are correctly applied to first variable
// Coverage: int *x, int **y (base type has *), first variable should get those stars
// Expected: Stars deferred until end of first variable parsing, then applied
// Critical: This tests the Session 19 bug fix for first variable star precedence

int main() {
    int *x;
    int **y;
    int ***z;
    char *a;
    void *b;
    unsigned int *c;
    long long **d;
    const int *e;
    volatile char **f;
    return 0;
}
