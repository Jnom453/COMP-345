#include "Tests.h"

#include "Map.h"
#include "DirectDamageTower.h"
#include "AreaDamageTower.h"
#include "SlowTower.h"
#include "CritterGroupGenerator.h"
#include "Player.h"

#include <iostream>
#include <string>

using namespace std;

void check(bool condition, string message)
{
    if (condition)
    {
        cout << "[PASS] " << message << endl;
    }
    else
    {
        cout << "[FAIL] " << message << endl;
    }
}

void runTests()
{
    cout << "\n========== TESTS ==========\n";

    // --------------------------
    // MAP TEST
    // --------------------------

    Map map(5, 5);

    map.setCell(0, 0, PATH);
    map.setCell(0, 1, PATH);
    map.setCell(0, 2, PATH);
    map.setCell(1, 2, PATH);
    map.setCell(2, 2, PATH);
    map.setCell(3, 2, PATH);
    map.setCell(4, 2, PATH);

    map.setEntry(0, 0);
    map.setExit(4, 2);

    check(
        map.isValid(),
        "Valid map is accepted."
    );

    check(
        map.getCellType(0, 0) == ENTRY,
        "Entry cell is correctly stored."
    );

    check(
        map.getCellType(4, 2) == EXIT,
        "Exit cell is correctly stored."
    );

    check(
        map.getCellType(4, 4) == SCENERY,
        "Scenery cells allow towers."
    );

    // --------------------------
    // INVALID MAP TEST
    // --------------------------

    Map invalidMap(5, 5);

    invalidMap.setCell(0, 0, PATH);
    invalidMap.setCell(4, 4, PATH);

    invalidMap.setEntry(0, 0);
    invalidMap.setExit(4, 4);

    check(
        !invalidMap.isValid(),
        "Disconnected map is rejected."
    );

    // --------------------------
    // TOWER TESTS
    // --------------------------

    Player player(1000);

    DirectDamageTower directTower;

    check(
        directTower.getLevel() == 1,
        "Tower starts at level 1."
    );

    check(
        player.spendCoins(
            directTower.getBuyingCost()
        ),
        "Player can buy tower."
    );

    int oldLevel = directTower.getLevel();

    check(
        directTower.upgrade(player),
        "Tower can be upgraded."
    );

    check(
        directTower.getLevel() == oldLevel + 1,
        "Tower level increases after upgrade."
    );

    // --------------------------
    // CRITTER GROUP TESTS
    // --------------------------

    CritterGroupGenerator generator;

    CritterGroup wave1 =
        generator.generateWave(1);

    CritterGroup wave2 =
        generator.generateWave(2);

    check(
        wave2.getSize() > wave1.getSize(),
        "Later waves contain more critters."
    );

    // --------------------------
    // DIRECT ATTACK
    // --------------------------

    Critter& critter =
        wave1.getCritter(0);

    int oldHP =
        critter.getHitPoints();

    directTower.attack(critter);

    check(
        critter.getHitPoints() < oldHP,
        "Tower attack reduces critter HP."
    );

    // --------------------------
    // AREA DAMAGE
    // --------------------------

    AreaDamageTower areaTower;

    Critter& secondCritter =
        wave1.getCritter(1);

    int secondOldHP =
        secondCritter.getHitPoints();

    areaTower.attack(secondCritter);

    check(
        secondCritter.getHitPoints() < secondOldHP,
        "Area damage tower damages critter."
    );

    // --------------------------
    // SLOW TOWER
    // --------------------------

    SlowTower slowTower;

    Critter& slowTarget =
        wave1.getCritter(2);

    double oldSpeed =
        slowTarget.getSpeed();

    slowTower.attack(slowTarget);

    check(
        slowTarget.getSpeed() < oldSpeed,
        "Slow tower reduces critter speed."
    );

    // --------------------------
    // EXIT / COIN TEST
    // --------------------------

    int oldCoins =
        player.getCoins();

    Critter escapingCritter(
        1,
        50,
        5,
        1.0,
        10
    );

    escapingCritter.reachExit();

    player.loseCoins(
        escapingCritter.getStrength()
    );

    check(
        player.getCoins() ==
        oldCoins - escapingCritter.getStrength(),
        "Escaping critter steals coins."
    );

    cout << "===========================\n";
}