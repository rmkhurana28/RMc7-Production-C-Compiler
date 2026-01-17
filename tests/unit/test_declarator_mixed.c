//rm129
// Unit Test Case - Mixed Declarators
// Purpose: Test combination of pointers, arrays, and parentheses
// Coverage: Complex patterns mixing all declarator types
// Expected: Correct precedence handling with proper star and bracket application

int main() {
    int *x[];
    int (*y)[];
    int **z[];
    int *(*w)[];
    int (**a)[];
    int ***b;
    int *(**c);
    return 0;
}
