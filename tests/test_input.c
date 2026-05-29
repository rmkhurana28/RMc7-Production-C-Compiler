/*
 * Final Verification Test - bool and _Bool Support
 * All valid patterns from C99
 */

/* Global declarations */
bool global1;
_Bool global2;
const bool const_global1;
const _Bool const_global2;
volatile bool vol_global1;
volatile _Bool vol_global2;
static bool static_global1;
static _Bool static_global2;

int main(void)
{
    /* Basic bool and _Bool */
    bool b1;
    _Bool b2;

    /* With initializers */
    bool b3 = 1;
    _Bool b4 = 0;

    /* Pointers */
    bool *p1;
    _Bool *p2;
    bool **pp1;
    _Bool **pp2;

    /* Arrays */
    bool arr1[5];
    _Bool arr2[10];

    /* With qualifiers */
    const bool cb1;
    const _Bool cb2;
    volatile bool vb1;
    volatile _Bool vb2;

    return 0;
}
