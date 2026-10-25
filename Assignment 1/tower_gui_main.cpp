
#include "DirectDamageTower.h"
#include "TowerObserver.h"
#include "Player.h"
#include <iostream>

int main()
{
    // Create a tower and a player with enough coins
    DirectDamageTower tower;
    Player player(1000);

    // Create and register the observer
    TowerObserver towerObserver(tower);
    tower.attach(&towerObserver);

    std::cout << "Upgrading tower...\n";

    // Trigger a tower notification
    bool upgraded = tower.upgrade(player);

    if (upgraded)
        std::cout << "Tower upgraded successfully!\n";
    else
        std::cout << "Tower upgrade failed.\n";

    // Open the graphical view
    towerObserver.run();

    // Detach before the observer is destroyed
    tower.detach(&towerObserver);

    return 0;
}