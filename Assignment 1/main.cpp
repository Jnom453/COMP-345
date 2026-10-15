#include <iostream>
#include <vector>

#include "Map.h"
#include "DirectDamageTower.h"
#include "AreaDamageTower.h"
#include "SlowTower.h"
#include "CritterGroupGenerator.h"
#include "Player.h"
#include "Tests.h"

using namespace std;

int main()
{
    cout << "====================================\n";
    cout << "       TOWER DEFENSE PROJECT\n";
    cout << "====================================\n";

    // =====================================
    // PART 1: MAP
    // =====================================

    cout << "\n--- PART 1: MAP ---\n";

    Map map(8, 8);

    // Create horizontal part of path.
    for (int column = 0; column < 6; column++)
    {
        map.setCell(0, column, PATH);
    }

    // Create vertical part of path.
    for (int row = 0; row < 6; row++)
    {
        map.setCell(row, 5, PATH);
    }

    // Set entry and exit.
    map.setEntry(0, 0);
    map.setExit(5, 5);

    map.display();

    if (map.isValid())
    {
        cout << "Map is valid.\n";
    }
    else
    {
        cout << "Map is invalid.\n";
    }

    // =====================================
    // PART 2: TOWERS
    // =====================================

    cout << "\n--- PART 2: TOWERS ---\n";

    Player player(1000);

    DirectDamageTower directTower;
    AreaDamageTower areaTower;
    SlowTower slowTower;

    cout << directTower.getName()
         << ": "
         << directTower.getDescription()
         << endl;

    cout << areaTower.getName()
         << ": "
         << areaTower.getDescription()
         << endl;

    cout << slowTower.getName()
         << ": "
         << slowTower.getDescription()
         << endl;

    cout << "Player coins: "
         << player.getCoins()
         << endl;

    // Buy the direct damage tower.
    if (player.spendCoins(
        directTower.getBuyingCost()))
    {
        cout << "Direct damage tower purchased.\n";
    }

    cout << "Player coins after purchase: "
         << player.getCoins()
         << endl;

    // Upgrade the tower.
    if (directTower.upgrade(player))
    {
        cout << "Direct damage tower upgraded to level "
             << directTower.getLevel()
             << ".\n";
    }

    // =====================================
    // PART 3: CRITTER GROUP
    // =====================================

    cout << "\n--- PART 3: CRITTER GROUP ---\n";

    CritterGroupGenerator generator;

    CritterGroup wave =
        generator.generateWave(2);

    wave.display();

    // =====================================
    // TOWER ATTACK
    // =====================================

    cout << "\n--- TOWER ATTACK ---\n";

    Critter& target =
        wave.getCritter(0);

    cout << "Target HP before attack: "
         << target.getHitPoints()
         << endl;

    directTower.attack(target);

    cout << "Target HP after attack: "
         << target.getHitPoints()
         << endl;

    // =====================================
    // AREA DAMAGE
    // =====================================

    cout << "\n--- AREA DAMAGE ---\n";

    vector<Critter*> targets;

    targets.push_back(
        &wave.getCritter(1)
    );

    targets.push_back(
        &wave.getCritter(2)
    );

    areaTower.attackMultiple(targets);

    cout << "Two critters were attacked "
         << "by the area damage tower.\n";

    // =====================================
    // SLOW EFFECT
    // =====================================

    cout << "\n--- SLOW EFFECT ---\n";

    Critter& slowTarget =
        wave.getCritter(3);

    cout << "Speed before slow: "
         << slowTarget.getSpeed()
         << endl;

    slowTower.attack(slowTarget);

    cout << "Speed after slow: "
         << slowTarget.getSpeed()
         << endl;

    // =====================================
    // AUTOMATED TESTS
    // =====================================

    runTests();

    cout << "\nProgram finished.\n";

    return 0;
}