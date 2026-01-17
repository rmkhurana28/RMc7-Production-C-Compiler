//rm129
// Unit Test Case - Complex Pointer Declarators with Parentheses
// Purpose: Test pointer-to-pointer with precedence using parentheses
// Coverage: *(*x), *((*x)), **(*x), *((**x))
// Expected: Parser handles recursion correctly and applies stars in proper order
// Note: Tests Session 19 fix for deferred star application

int main() {
    int *(*x);
    int *((*y));
    int **(*z);
    int *((**w));
    char *(*a);
    void *(*b);
    return 0;
}
