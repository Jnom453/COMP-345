#include "CritterGroup.h"
#include <cassert>
#include <iostream>

int main()
{
    CritterGroup group;

    Critter first(1, 100, 10, 2.0, 25);
    Critter second(2, 150, 15, 1.5, 35);

    assert(group.getSize() == 0);

    group.addCritter(first);
    group.addCritter(second);

    assert(group.getSize() == 2);
    std::cout << "Adding critters: PASS\n";

    bool removed = group.removeCritter(0);

    assert(removed);
    assert(group.getSize() == 1);
    assert(group.getCritter(0).getLevel() == 2);
    std::cout << "Removing a critter: PASS\n";

    assert(!group.removeCritter(5));
    assert(group.getSize() == 1);
    std::cout << "Invalid removal: PASS\n";

    return 0;
}