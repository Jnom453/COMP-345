
#include "Critter.h"
#include "CritterObserver.h"
#include <iostream>

int main()
{
    // Create a critter: level, hit points, strength, speed, reward
    Critter critter(1, 100, 10, 2.0, 25);

    // Create and attach the observer
    CritterObserver critterObserver(critter);
    critter.attach(&critterObserver);

    // Change the critter after attaching the observer
    std::cout << "Applying damage to critter...\n";
    critter.takeDamage(30);

    std::cout << "Critter HP after damage: "
              << critter.getHitPoints() << "\n";

    // Open the graphical view
    critterObserver.run();

    // Detach before the observer is destroyed
    critter.detach(&critterObserver);

    return 0;
}