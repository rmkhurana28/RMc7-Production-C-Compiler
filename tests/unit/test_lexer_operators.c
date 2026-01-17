//rm129
// Unit Test Case - Lexer Operators
// Purpose: Test lexer's ability to recognize all C operators
// Coverage: Arithmetic (+, -, *, /, %), comparison (==, !=, <, >), logical (&&, ||)
// Expected: Each operator correctly tokenized with proper type classification

int test() {
    a = b + c;
    d = e - f;
    g = h * i;
    j = k / l;
    m = n % o;
    p == q;
    r != s;
    t < u;
    v > w;
    x <= y;
    z >= aa;
    bb && cc;
    dd || ee;
    ff++;
    gg--;
    return 0;
}
