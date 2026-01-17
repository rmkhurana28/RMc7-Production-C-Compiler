//rm129
// Unit Test Case - Simple Pointer Declarators
// Purpose: Test basic pointer declarations with varying levels of indirection
// Coverage: Single star (*x), double star (**x), triple star (***x)
// Expected: Parser correctly identifies pointer levels and associates with variable names

int main() {
    int *x;
    int **y;
    int ***z;
    char *a;
    char **b;
    void *c;
    float *d;
    double **e;
    return 0;
}
