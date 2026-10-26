
#include "MapObserver.h"
#include "TowerObserver.h"
#include "CritterObserver.h"
#include "CritterGroupObserver.h"

#include "DirectDamageTower.h"
#include "Player.h"
#include "Critter.h"
#include "CritterGroup.h"

#include <iostream>

int main()
{
    std::cout << "Assignment 2: Observer Pattern Demonstration\n";

    // Part 1: Map Observer
    std::cout << "\nPart 1: Map Observer\n";

    Map map(12, 10);

    for (int x = 0; x < 12; ++x)
        map.setCell(x, 5, PATH);

    map.setEntry(5, 0);
    map.setExit(5, 11);

    MapObserver mapObserver(map);
    map.attach(&mapObserver);

    map.setCell(2, 4, PATH);
    mapObserver.run();

    map.detach(&mapObserver);

    // Part 2: Tower Observer
    std::cout << "\nPart 2: Tower Observer\n";

    DirectDamageTower tower;
    Player player(1000);

    TowerObserver towerObserver(tower);
    tower.attach(&towerObserver);

    tower.upgrade(player);
    towerObserver.run();

    tower.detach(&towerObserver);

    // Part 3: Critter Observer
    std::cout << "\nPart 3: Critter Observer\n";

    Critter critter(1, 100, 10, 2.0, 25);

    CritterObserver critterObserver(critter);
    critter.attach(&critterObserver);

    critter.takeDamage(30);
    critterObserver.run();

    critter.detach(&critterObserver);

    // Critter group: demonstrate adding and removing
    std::cout << "\nCritter Group Observer\n";

    CritterGroup group;
    Critter first(1, 100, 10, 2.0, 25);
    Critter second(2, 80, 15, 1.5, 30);
    Critter third(3, 60, 20, 1.0, 35);

    group.addCritter(first);
    group.addCritter(second);

    CritterGroupObserver groupObserver(group);
    group.attach(&groupObserver);

    group.addCritter(third);
    group.removeCritter(0);

    groupObserver.run();
    group.detach(&groupObserver);

    std::cout << "\nAll demonstrations completed.\n";
    return 0;
}

