#include "NumberSeries.h"
#include <iostream>

int powerSeries(int n)
{
    return n + 5;
}

int nextFibonacci(int n)
{
    int a = 0, b = 1;
    while (true)
    {
        int temp = a + b;
        if (temp > n)
            return temp;
        a = b;
        b = temp;
    }
}

int main()
{

    NumberSeries ns(1, nextFibonacci);
    ns.getNumberByIndex(50);
    ns.print();
}
