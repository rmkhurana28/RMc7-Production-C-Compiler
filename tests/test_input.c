int *t1a, **t1b, ***t1c;

int *t2a[10], **t2b(int x), ***(*t2c)(int y), ****t2d;

int *t3a(int p1), **t3b(int **(*p2)(int)), ***t3c;

int **(*t4a)(int), ***t4b(int *(*p)(int **)), *(*t4c)[5];

int *(*(*t5a)[10])(int), **t5b(int ***(*p)(int ****)), *(*(*t5c(int *p))[5])(int);

int *(*(*t6)(int *(*p1)(int **p2)))[10];

int **f7a(int), (*f7b)(int), ***f7c(int), ****f7d;