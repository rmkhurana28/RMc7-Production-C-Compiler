struct S {
    int value;
};

typedef int (*FuncPtr)(int);

int square(int x) {
    return x * x;
}

int main(void) {

    FuncPtr table[3];

    table[0] = square;
    table[1] = square;
    table[2] = square;

    int result = table[1](5);

    return result;
}