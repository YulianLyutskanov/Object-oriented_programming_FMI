#include "ModifiableIntegersFunction.h"
#include <iostream>

int16_t f(int16_t n)
{
    return n + n % 3;
}

int16_t g(int16_t n)
{
    return n + 7 + n % 5;
}

int16_t t(int16_t n)
{
    return 5 * n + 1;
}

int16_t s(int16_t n)
{
    return -n * n;
}
int main()
{
    ModifiableIntegersFunction m(t);
    ModifiableIntegersFunction m1(s);

    (m1 + m).drawGraphicInInterval(-10, -10);
}

// to do
// make method isConstant maybe
