#include "CritterGroupObserver.h"
#include <iostream>

int main()
{
    CritterGroup group;

    Critter first(1, 100, 10, 2.0, 25);
    Critter second(2, 80, 15, 1.5, 30);
    Critter third(3, 60, 20, 1.0, 35);

    group.addCritter(first);
    group.addCritter(second);

    CritterGroupObserver observer(group);
    group.attach(&observer);

    std::cout << "Initial group size: " << group.getSize() << '\n';

    group.addCritter(third);
    std::cout << "After adding a critter: "
              << group.getSize() << '\n';

    group.removeCritter(0);
    std::cout << "After removing a critter: "
              << group.getSize() << '\n';

    observer.run();

    group.detach(&observer);
    return 0;
}