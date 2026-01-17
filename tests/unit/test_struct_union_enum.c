//rm129
// Unit Test Case - Struct/Union/Enum Declarations
// Purpose: Test type registry (TR) based declarations
// Coverage: struct, union, enum keywords with tags
// Expected: Parser recognizes TR entries and validates against type registry
// Note: Requires structs to be registered in TR hashmap first

struct Point {
    int x;
    int y;
};

union Data {
    int i;
    float f;
};

enum Color {
    RED,
    GREEN,
    BLUE
};

int main() {
    struct Point p;
    union Data d;
    enum Color c;
    struct Point *ptr;
    return 0;
}
