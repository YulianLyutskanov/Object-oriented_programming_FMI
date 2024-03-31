#include "Multiset.h"
#include <iostream>

int main()
{
    MultiSet ms(10);
    MultiSet gs(14);
    ms.add(4);
    ms.add(4);
    ms.add(4);
    ms.add(4);
    ms.add(4);
    ms.add(3);
    ms.add(7);
    ms.add(7);
    gs.add(1);
    gs.add(1);
    gs.add(2);
    gs.add(3);
    gs.add(4);
    gs.add(4);
    gs.add(11);
    gs.add(12);
    ms.print();
    gs.print();
    MultiSet res = UnionOfMultiSets(ms, gs);
    MultiSet res2 = IntersectionOfMultiSets(ms, gs);
    res.print();
    res2.print();
}
