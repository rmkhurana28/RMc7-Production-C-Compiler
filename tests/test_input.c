

// int tr4 = a ? b : c ? d : e ? f : g;
// currently ternary is acting like LR , but need to make it RL


// ============================================
// EXTREME PRECEDENCE CHAINS
// ============================================
int p1 = a + b * c - d / e % f + g * h - i / j % k;
int p2 = a << b + c * d - e >> f + g * h;
int p3 = a | b ^ c & d | e ^ f & g | h ^ i & j;
int p4 = a && b || c && d || e && f || g && h;
int p5 = a == b != c == d != e < f > g <= h >= i;
int p6 = a | b & c ^ d | e & f ^ g | h & i ^ j;
int p7 = a << b << c << d >> e >> f >> g;
int p8 = a + b - c + d - e + f - g + h - i + j;
int p9 = a * b / c % d * e / f % g * h / i % j;
int p10 = a || b && c | d ^ e & f == g < h << i + j * k;

// ============================================
// ASSIGNMENT CHAIN STRESS
// ============================================
int as1 = a = b = c = d = e = f = g = h = 100;
int as2 = a += b -= c *= d /= e %= f = 10;
int as3 = a &= b |= c ^= d <<= e >>= f = 5;
int as4 = (a = 1) + (b = 2) + (c = 3) + (d = 4);
int as5 = (a += b) * (c -= d) / (e *= f) + (g /= h);
int as6 = a = (b = (c = (d = (e = 1))));
int as7 = (a = b + c) * (d = e - f) + (g = h * i);
int as8 = a = b = c += d += e *= f *= g = 1;

// ============================================
// UNARY MADNESS
// ============================================
int u1 = - - - - - - - - a;
int u2 = + + + + + + + + b;
int u3 = ! ! ! ! ! ! ! ! c;
int u4 = ~ ~ ~ ~ ~ ~ ~ ~ d;
int u5 = - + ~ ! - + ~ ! a;
int u6 = ~ ! ~ ! ~ ! ~ ! b;
int u7 = - - a * + + b - ~ ~ c + ! ! d;
int u8 = ++a + ++b + ++c + ++d + ++e;
int u9 = a++ + b++ + c++ + d++ + e++;
int u10 = --a - --b - --c - --d - --e;
int u11 = a-- - b-- - c-- - d-- - e--;
int u12 = ++a * b-- + --c / d++ - ++e % f--;
int u13 = -++a + +--b - ~c++ * !d--;
int u14 = a+++b+++c+++d+++e;
int u15 = a---b---c---d---e;

// ============================================
// FUNCTION CALL CHAOS
// ============================================
int fc1 = add(a, b) + add(c, d) + add(e, f) + add(g, h);
int fc2 = add(a * b, c * d) + multiply(e / f, g / h);
int fc3 = add(add(add(a, b), add(c, d)), add(add(e, f), add(g, h)));
int fc4 = add(a + b + c, d + e + f) * add(g - h - i, j - k - l);
int fc5 = -add(a, b) + ~add(c, d) - !add(e, f);
int fc6 = add(a, b)++ + ++add(c, d) - add(e, f)-- * --add(g, h);
int fc7 = add((a, b, c), (d, e, f)) + multiply((g, h), (i, j));
int fc8 = add(a = 1, b = 2) + add(c += 3, d -= 4);
int fc9 = add(++a, b++) * add(--c, d--) / add(~e, !f);
int fc10 = compute(add(a, b), multiply(c, d), add(e, f), multiply(g, h));
int fc11 = (add(a, b) + add(c, d)) * (multiply(e, f) - multiply(g, h));
int fc12 = add(add(add(add(a, b), c), d), e) + add(a, add(b, add(c, add(d, e))));

// ============================================
// SIZEOF STRESS
// ============================================
int sz1 = sizeof(int) + sizeof(char) + sizeof(float) + sizeof(double);
int sz2 = sizeof(a) + sizeof(b) + sizeof(c) + sizeof(d);
int sz3 = sizeof(int) * sizeof(char) - sizeof(float) / sizeof(double);
int sz4 = -sizeof(int) + ~sizeof(char) - !sizeof(float);
int sz5 = sizeof(a + b) + sizeof(c * d) + sizeof(e - f);
int sz6 = sizeof(a = b) + sizeof(c += d) + sizeof(e *= f);
int sz7 = sizeof(++a) + sizeof(b++) + sizeof(--c) + sizeof(d--);
int sz8 = sizeof(-a) + sizeof(+b) + sizeof(~c) + sizeof(!d);
int sz9 = add(sizeof(int), sizeof(char)) * sizeof(float);
int sz10 = sizeof(add(a, b)) + sizeof(multiply(c, d));
int sz11 = sizeof(int) << 2 >> 1 & 255 | 256;
int sz12 = (sizeof(int) + sizeof(char)) * (sizeof(float) - sizeof(double));

// ============================================
// TYPE CAST MADNESS
// ============================================
int tc1 = (int)a + (int)b + (int)c + (int)d;
int tc2 = (int)(float)(double)(char)(long)a;
int tc3 = (int)a * (float)b + (double)c - (char)d;
int tc4 = -(int)a + ~(float)b - !(double)c;
int tc5 = (int)-a + (float)+b + (double)~c + (char)!d;
int tc6 = (int)++a + (float)b++ + (double)--c + (char)d--;
int tc7 = (int)(a + b) * (float)(c - d) / (double)(e * f);
int tc8 = (int)add(a, b) + (float)multiply(c, d);
int tc9 = (int)sizeof(float) + (float)sizeof(int);
int tc10 = (int)(a = b) + (float)(c += d) + (double)(e *= f);
int tc11 = (int)(a, b, c) + (float)(d, e, f);
int tc12 = add((int)a, (float)b) * multiply((double)c, (char)d);

// ============================================
// COMMA OPERATOR INSANITY
// ============================================
int cm1 = (a, b, c, d, e, f, g, h, i, j);
int cm2 = (a = 1, b = 2, c = 3, d = 4, a + b + c + d);
int cm3 = (++a, ++b, ++c, a + b + c);
int cm4 = (a++, b++, c++, d++, e++, a + b + c + d + e);
int cm5 = ((a, b), (c, d), (e, f), (g, h));
int cm6 = (a = 1, b = a + 1, c = b + 1, d = c + 1, a + b + c + d);
int cm7 = add((a, b), (c, d)) + multiply((e, f), (g, h));
int cm8 = (a, b) + (c, d) * (e, f) - (g, h) / (i, j);
int cm9 = ((a = 1, a + 1), (b = 2, b + 2), (c = 3, c + 3));
int cm10 = (add(a, b), multiply(c, d), compute(e, f, g, h));

// ============================================
// BITWISE OPERATOR SYMPHONY
// ============================================
int bw1 = a & b & c & d & e & f & g & h;
int bw2 = a | b | c | d | e | f | g | h;
int bw3 = a ^ b ^ c ^ d ^ e ^ f ^ g ^ h;
int bw4 = ~a & ~b | ~c ^ ~d & ~e | ~f ^ ~g & ~h;
int bw5 = (a & b) | (c & d) ^ (e & f) | (g & h);
int bw6 = a << 1 | b << 2 | c << 3 | d << 4;
int bw7 = a >> 1 & b >> 2 & c >> 3 & d >> 4;
int bw8 = (a | b) & (c | d) ^ (e | f) & (g | h);
int bw9 = a & 255 | b & 65280 ^ c & 16711680;
int bw10 = ~(a & b) | ~(c | d) ^ ~(e ^ f);
int bw11 = a << b + c >> d - e & f * g | h / i;
int bw12 = ((a & b) | (c ^ d)) & ((e | f) ^ (g & h));

// ============================================
// LOGICAL OPERATOR MAZE
// ============================================
int lg1 = a && b && c && d && e && f && g && h;
int lg2 = a || b || c || d || e || f || g || h;
int lg3 = a && b || c && d || e && f || g && h;
int lg4 = !a && !b || !c && !d || !e && !f;
int lg5 = (a && b) || (c && d) || (e && f) || (g && h);
int lg6 = (a || b) && (c || d) && (e || f) && (g || h);
int lg7 = !!a && ~~b || !!c && ~~d;
int lg8 = a > 0 && b < 10 || c >= 5 && d <= 15;
int lg9 = a == b && c != d || e < f && g > h;
int lg10 = (a + b > c - d) && (e * f < g / h) || (i % j == k);
int lg11 = !a || !b && !c || !d && !e || !f;
int lg12 = a && b && c || d || e && f && g || h;

// ============================================
// RELATIONAL + EQUALITY CHAINS
// ============================================
int rl1 = a < b < c < d < e;
int rl2 = a > b > c > d > e;
int rl3 = a <= b <= c <= d <= e;
int rl4 = a >= b >= c >= d >= e;
int rl5 = a == b == c == d == e;
int rl6 = a != b != c != d != e;
int rl7 = a < b == c > d != e <= f >= g;
int rl8 = (a < b) == (c > d) != (e <= f);
int rl9 = a + b < c + d == e - f > g - h;
int rl10 = a * b <= c / d >= e % f != g + h;
int rl11 = (a < b) + (c > d) - (e == f) * (g != h);
int rl12 = !!(a < b) && !!(c > d) || !!(e == f);

// ============================================
// SHIFT OPERATOR STRESS
// ============================================
int sh1 = a << 1 << 2 << 3 << 4 << 5;
int sh2 = a >> 1 >> 2 >> 3 >> 4 >> 5;
int sh3 = a << 1 >> 2 << 3 >> 4 << 5;
int sh4 = a << b + c >> d - e << f * g;
int sh5 = (a + b) << (c + d) >> (e + f);
int sh6 = a << 1 + b << 2 + c << 3 + d;
int sh7 = a >> b - c >> d - e >> f - g;
int sh8 = (a << 1) | (b << 2) | (c << 3) | (d << 4);
int sh9 = (a >> 1) & (b >> 2) & (c >> 3) & (d >> 4);
int sh10 = a * b << c / d >> e % f + g - h;
int sh11 = ~a << 2 | ~b >> 3 ^ ~c << 4;
int sh12 = sizeof(int) << 3 >> 1 + 2;

// ============================================
// MONSTER EXPRESSIONS (NO TERNARY)
// ============================================
int mon1 = a + b * c - d / e % f << g >> h & i | j ^ k && l || m;
int mon2 = (a = b += c) * (d -= e *= f) + (g /= h %= i) - (j &= k |= l);
int mon3 = add(a * b + c, multiply(d - e, f)) + compute(g, h, i, j) - add(k, l);
int mon4 = ~!-+a + (int)sizeof(b) * add(c, d) / (e + f) % (g, h, i);
int mon5 = a++ * ++b - c-- / --d + +e - -f & ~g | !h ^ i << j >> k;
int mon6 = ((a + b) * (c - d)) << ((e / f) % (g + h)) >> ((i & j) | (k ^ l));
int mon7 = add((a, b, c), (d, e, f)) * multiply((int)g, (float)h) + sizeof(int);
int mon8 = (a = 1, b = 2, c = a + b) * (d = 3, e = 4, d + e) + (f++, g--, f + g);
int mon9 = !!~~--++a + (int)(float)(double)b - sizeof(c + d) * add(e, f);
int mon10 = a && b || c && d && e || f || g && h || i && j || k && l || m && n;