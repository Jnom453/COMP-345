#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AreaDamageTower.h"
#include "BurningDecorator.h"
#include "Critter.h"
#include "CritterFactory.h"
#include "DirectDamageTower.h"
#include "FreezingDecorator.h"
#include "Game.h"
#include "LevelUpDecorator.h"
#include "Player.h"
#include "SlowTower.h"
#include "SplashDecorator.h"
#include "TargetingStrategy.h"
#include "WaveFactory.h"

/**
 * @file Tests_A3.cpp
 * @brief Console test cases for the three design patterns of Assignment 3.
 *
 * Run it with ./a3_tests. Each CHECK prints PASS or FAIL; the program returns
 * 0 only if every check passed. The tests are grouped by pattern:
 * Strategy (targeting), Factory (critters and waves), Decorator (towers) and
 * a few tests of the Game that uses them together.
 */

namespace
{
int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name)
{
    if (condition)
    {
        ++passed;
        std::cout << "  PASS  " << name << "\n";
    }
    else
    {
        ++failed;
        std::cout << "  FAIL  " << name << "\n";
    }
}

// A critter that is already "position" along its path (position 0 to 100).
// The critter's speed is used as the distance of its first move.
Critter critterAt(double position, int hitPoints)
{
    Critter c(1, hitPoints, 5, position, 10);
    c.move();
    return c;
}

TargetCandidate candidate(Critter& c, double distance)
{
    TargetCandidate t = { &c, distance };
    return t;
}

// ---------------------------------------------------------------- Strategy
void testStrategies()
{
    std::cout << "\nStrategy pattern (targeting)\n";

    Critter a = critterAt(10.0, 80);  // distance 3.0
    Critter b = critterAt(60.0, 40);  // distance 2.0
    Critter c = critterAt(30.0, 120); // distance 1.0 ... nearest to the tower
    std::vector<TargetCandidate> list;
    list.push_back(candidate(a, 3.0));
    list.push_back(candidate(b, 2.0));
    list.push_back(candidate(c, 1.0));

    check(NearestToTowerStrategy().chooseTarget(list) == 2, "nearest to tower picks the smallest distance");
    check(NearestToExitStrategy().chooseTarget(list) == 1, "nearest to exit picks the furthest along the path");
    check(StrongestStrategy().chooseTarget(list) == 2, "strongest picks the most hit points");
    check(WeakestStrategy().chooseTarget(list) == 1, "weakest picks the fewest hit points");

    std::vector<TargetCandidate> empty;
    check(NearestToExitStrategy().chooseTarget(empty) == -1, "no candidates gives -1");

    Critter dead = critterAt(90.0, 10);
    dead.takeDamage(1000);
    std::vector<TargetCandidate> withDead;
    withDead.push_back(candidate(dead, 0.5));
    withDead.push_back(candidate(a, 3.0));
    check(NearestToTowerStrategy().chooseTarget(withDead) == 1, "dead critters are never chosen");

    Critter t1 = critterAt(20.0, 50);
    Critter t2 = critterAt(20.0, 50);
    std::vector<TargetCandidate> ties;
    ties.push_back(candidate(t1, 2.0));
    ties.push_back(candidate(t2, 2.0));
    check(StrongestStrategy().chooseTarget(ties) == 0, "a tie goes to the first candidate");

    check(createStrategy(STRONGEST)->getKind() == STRONGEST, "createStrategy builds the requested kind");
    check(createStrategy(99)->getKind() == NEAREST_TO_EXIT, "createStrategy falls back to nearest to exit");

    DirectDamageTower tower;
    check(tower.chooseTarget(list) == 1, "a new tower targets the critter nearest to the exit");
    tower.setStrategy(createStrategy(WEAKEST));
    check(tower.chooseTarget(list) == 1, "the strategy can be replaced (weakest)");
    tower.setStrategy(createStrategy(STRONGEST));
    check(tower.chooseTarget(list) == 2, "the strategy can be replaced again at any time (strongest)");
    tower.setStrategy(std::shared_ptr<TargetingStrategy>());
    check(tower.getStrategy()->getKind() == STRONGEST, "setting a null strategy is ignored");

    std::unique_ptr<Tower> decorated(new LevelUpDecorator(std::unique_ptr<Tower>(new DirectDamageTower())));
    decorated->setStrategy(createStrategy(NEAREST_TO_TOWER));
    check(decorated->chooseTarget(list) == 2, "a decorated tower uses the strategy set on it");
}

// ----------------------------------------------------------------- Factory
void testFactories()
{
    std::cout << "\nFactory pattern (critters and waves)\n";

    NormalCritterFactory normal;
    FastCritterFactory fast;
    TankCritterFactory tank;
    BossCritterFactory boss;

    Critter n = normal.createCritter(2);
    check(n.getKind() == "Normal" && n.getHitPoints() == 80 && n.getReward() == 20, "normal critter of wave 2");
    Critter f = fast.createCritter(2);
    check(f.getKind() == "Fast" && f.getHitPoints() == 50 && std::fabs(f.getSpeed() - 1.8) < 1e-9, "fast critter of wave 2");
    Critter t = tank.createCritter(2);
    check(t.getKind() == "Tank" && t.getHitPoints() == 180 && t.getStrength() == 10, "tank critter of wave 2");
    Critter b = boss.createCritter(2);
    check(b.getKind() == "Boss" && b.getHitPoints() == 420 && b.getReward() == 80, "boss critter of wave 2");
    check(normal.createCritter(0).getLevel() == 1, "a wave number below 1 counts as wave 1");
    check(normal.createCritter(6).getHitPoints() > normal.createCritter(1).getHitPoints(), "later critters have more hit points");

    WaveFactory factory;
    CritterGroup w1 = factory.createWave(0);
    bool allNormal = true;
    for (int i = 0; i < w1.getSize(); ++i)
        if (w1.getCritter(i).getKind() != "Normal")
            allNormal = false;
    check(w1.getSize() == 4 && allNormal, "first wave: 4 normal critters");

    CritterGroup w3 = factory.createWave(2);
    int fastCount = 0, otherCount = 0;
    for (int i = 0; i < w3.getSize(); ++i)
    {
        if (w3.getCritter(i).getKind() == "Fast") ++fastCount;
        else if (w3.getCritter(i).getKind() != "Normal") ++otherCount;
    }
    check(w3.getSize() == 6 && fastCount == 2 && otherCount == 0, "wave 3 (2 destroyed): fast critters appear, no tanks or bosses");

    CritterGroup w5 = factory.createWave(4);
    bool hasTank = false;
    for (int i = 0; i < w5.getSize(); ++i)
        if (w5.getCritter(i).getKind() == "Tank") hasTank = true;
    check(hasTank, "wave 5 (4 destroyed) contains a tank");

    CritterGroup w8 = factory.createWave(7);
    check(w8.getCritter(w8.getSize() - 1).getKind() == "Boss", "wave 8 (7 destroyed) ends with a boss");

    CritterGroup w7 = factory.createWave(6);
    check(w7.getCritter(w7.getSize() - 1).getKind() != "Boss", "wave 7 has no boss yet");
    check(factory.createWave(-3).getSize() == 4, "a negative number of destroyed waves counts as zero");
    check(factory.createWave(9).getSize() > factory.createWave(0).getSize(), "later waves have more critters");
}

// --------------------------------------------------------------- Decorator
void testDecorators()
{
    std::cout << "\nDecorator pattern (towers)\n";

    std::unique_ptr<Tower> base(new DirectDamageTower());
    check(base->getLevel() == 1 && base->getPower() == 20 && base->getUpgradeCost() == 100, "undecorated tower: level 1, power 20, next level costs 100");

    std::unique_ptr<Tower> lv2(new LevelUpDecorator(std::move(base)));
    check(lv2->getLevel() == 2, "level up decorator: level 2");
    check(lv2->getPower() == 25 && std::fabs(lv2->getRange() - 4.0) < 1e-9, "level up decorator: +5 power and +1 range");
    check(lv2->getUpgradeCost() == 200, "level up decorator: next level costs 200");
    check(lv2->getRefundValue() == 100, "level up decorator: refund grows from 50 to 100");
    check(lv2->getName() == "Direct Damage Tower", "a decorator keeps the name of the tower");

    std::unique_ptr<Tower> lv3(new LevelUpDecorator(std::move(lv2)));
    check(lv3->getLevel() == 3 && lv3->getPower() == 30, "two level ups stack: level 3, power 30");
    check(lv3->getLevel() >= lv3->getMaxLevel(), "level 3 is the maximum level");

    // Splash
    {
        std::unique_ptr<Tower> splash(new SplashDecorator(std::unique_ptr<Tower>(new DirectDamageTower())));
        Critter target(1, 100, 5, 1.0, 10);
        Critter neighbour(1, 100, 5, 1.0, 10);
        std::vector<Critter*> nearby;
        nearby.push_back(&neighbour);
        splash->attackWithNearby(target, nearby);
        check(target.getHitPoints() == 80, "splash: the target takes the full damage (20)");
        check(neighbour.getHitPoints() == 90, "splash: a neighbour takes half the damage (10)");
        check(splash->hasEffect("Splash") && !splash->hasEffect("Burning"), "splash: hasEffect reports the effect");
        check(splash->getSplashRadius() > 0.0, "splash: the splash radius is positive");

        Critter solo(1, 100, 5, 1.0, 10);
        splash->attackWithNearby(solo, std::vector<Critter*>(1, &solo));
        check(solo.getHitPoints() == 80, "splash: the target is not hit twice when it is in the nearby list");
    }

    // Burning
    {
        std::unique_ptr<Tower> burning(new BurningDecorator(std::unique_ptr<Tower>(new DirectDamageTower())));
        Critter target(1, 100, 5, 1.0, 10);
        burning->attack(target);
        check(target.getHitPoints() == 80 && target.isBurning(), "burning: the hit damages and sets the critter on fire");
        for (int i = 0; i < 30; ++i)
            target.updateEffects(0.1);
        check(target.getHitPoints() == 65, "burning: 3 seconds of fire deal 3 x 5 more damage");
        check(!target.isBurning(), "burning: the fire stops after 3 seconds");
    }

    // Freezing
    {
        std::unique_ptr<Tower> freezing(new FreezingDecorator(std::unique_ptr<Tower>(new DirectDamageTower())));
        Critter target(1, 100, 5, 2.0, 10);
        freezing->attack(target);
        check(target.isFrozen(), "freezing: the hit freezes the critter");
        target.move();
        check(std::fabs(target.getPosition() - 1.0) < 1e-9, "freezing: a frozen critter moves at half speed");
        target.updateEffects(2.5);
        target.move();
        check(!target.isFrozen() && std::fabs(target.getPosition() - 3.0) < 1e-9, "freezing: after 2 seconds the critter moves at full speed again");
    }

    // Stacking
    {
        std::unique_ptr<Tower> t(new DirectDamageTower());
        t.reset(new LevelUpDecorator(std::move(t)));
        t.reset(new SplashDecorator(std::move(t)));
        t.reset(new BurningDecorator(std::move(t)));
        check(t->getLevel() == 2 && t->getPower() == 25, "stacked decorators keep the level and power of the level up");
        check(t->getEffects() == "Splash, Burning", "stacked decorators list their effects in order");
        check(t->hasEffect("Splash") && t->hasEffect("Burning") && !t->hasEffect("Freezing"), "stacked decorators: hasEffect finds effects inside");
        check(t->getRefundValue() == 100 + SplashDecorator::COST / 2 + BurningDecorator::COST / 2, "stacked decorators: refund adds half of each price");
        check(t->getDescription() == "Deals direct damage to one critter. + Splash + Burning", "stacked decorators: description lists the effects");

        Critter target(1, 100, 5, 1.0, 10);
        Critter neighbour(1, 100, 5, 1.0, 10);
        std::vector<Critter*> nearby(1, &neighbour);
        t->attackWithNearby(target, nearby);
        check(target.getHitPoints() == 75 && neighbour.getHitPoints() == 88 && target.isBurning() && !neighbour.isBurning(),
              "stacked decorators: damage, splash and burning all happen on one attack");
    }

    // Decorating an area tower and a slow tower
    {
        std::unique_ptr<Tower> area(new BurningDecorator(std::unique_ptr<Tower>(new AreaDamageTower())));
        Critter a(1, 100, 5, 1.0, 10), b(1, 100, 5, 1.0, 10);
        std::vector<Critter*> all;
        all.push_back(&a);
        all.push_back(&b);
        check(area->attacksAll(), "a decorated area tower still hits everything in range");
        area->attackAll(all);
        check(a.getHitPoints() == 88 && b.getHitPoints() == 88 && a.isBurning() && b.isBurning(), "burning is applied to every critter hit by an area tower");

        std::unique_ptr<Tower> slow(new FreezingDecorator(std::unique_ptr<Tower>(new SlowTower())));
        Critter c(1, 100, 5, 1.0, 10);
        slow->attack(c);
        check(c.getHitPoints() == 92 && c.isFrozen() && std::fabs(c.getSpeed() - 0.5) < 1e-9, "a decorated slow tower keeps its own slowing and adds freezing");
    }
}

// -------------------------------------------------------------------- Game
void testGame()
{
    std::cout << "\nGame (the patterns working together)\n";

    Game game;
    check(game.getMap().isValid(), "the game map is valid");
    check(game.placeTower(2, 5, Game::DIRECT_DAMAGE), "a tower can be bought and placed on scenery");
    check(!game.placeTower(1, 3, Game::DIRECT_DAMAGE), "a tower cannot be placed on the path");

    int coins = game.getCoins();
    check(game.upgradeTower(2, 5), "upgrade: level 2");
    check(game.getCoins() == coins - 100, "upgrade: level 2 cost 100 coins");
    check(game.getPlacedTower(0).tower->getLevel() == 2, "upgrade: the tower is now level 2");
    check(game.getPlacedTower(0).tower->getUpgradeCost() == 200, "upgrade: level 3 would cost 200 coins");
    check(!game.upgradeTower(2, 5), "upgrade: refused when the player has too few coins (150 < 200)");
    game.sellTower(2, 5);
    Game rich;
    rich.placeTower(2, 5, Game::DIRECT_DAMAGE);
    rich.upgradeTower(2, 5);
    rich.sellTower(2, 5); // refund gives coins back
    check(rich.getCoins() > 150, "selling returns coins to the player");

    Game g2;
    g2.placeTower(2, 5, Game::DIRECT_DAMAGE);
    int before = g2.getCoins();
    check(g2.addEffect(2, 5, Game::BURNING), "effect: burning can be added");
    check(g2.getCoins() == before - BurningDecorator::COST, "effect: burning costs its price");
    check(!g2.addEffect(2, 5, Game::BURNING), "effect: the same effect cannot be added twice");
    check(g2.getPlacedTower(0).tower->hasEffect("Burning"), "effect: the placed tower has the effect");
    g2.placeTower(3, 5, Game::AREA_DAMAGE);
    check(!g2.addEffect(3, 5, Game::SPLASH), "effect: splash is refused on an area tower");

    Game g3;
    g3.placeTower(2, 5, Game::DIRECT_DAMAGE);
    check(g3.getPlacedTower(0).tower->getStrategy()->getKind() == NEAREST_TO_EXIT, "strategy: a new tower starts with nearest to exit");
    std::vector<int> kinds;
    for (int i = 0; i < 4; ++i)
    {
        g3.cycleStrategy(2, 5);
        kinds.push_back(g3.getPlacedTower(0).tower->getStrategy()->getKind());
    }
    check(kinds[0] == STRONGEST && kinds[1] == WEAKEST && kinds[2] == NEAREST_TO_TOWER && kinds[3] == NEAREST_TO_EXIT,
          "strategy: cycling goes strongest, weakest, nearest to tower, nearest to exit");
    check(!g3.cycleStrategy(9, 9), "strategy: cycling a cell without a tower fails");

    Game g4;
    g4.placeTower(2, 5, Game::DIRECT_DAMAGE);
    int coins4 = g4.getCoins();
    g4.sellTower(2, 5);
    int plainRefund = g4.getCoins() - coins4;
    Game g5;
    g5.placeTower(2, 5, Game::DIRECT_DAMAGE);
    g5.upgradeTower(2, 5);
    int coins5 = g5.getCoins();
    g5.sellTower(2, 5);
    check(g5.getCoins() - coins5 > plainRefund, "sell: an upgraded tower is refunded more than a new one");

    // A tower kills critters of the first wave and the player earns coins.
    Game g6;
    g6.placeTower(2, 3, Game::DIRECT_DAMAGE);
    g6.placeTower(2, 7, Game::DIRECT_DAMAGE);
    int startCoins = g6.getCoins();
    g6.startWave();
    for (int i = 0; i < 60 * 60 && g6.getState() == Game::WAVE_RUNNING; ++i)
        g6.update(1.0 / 60.0);
    check(g6.getState() == Game::PLANNING && g6.getCoins() > startCoins, "a wave can be destroyed by towers, and kills pay coins");
    check(g6.getWave() == 2, "after a wave is destroyed the next wave is wave 2");
}
} // namespace

int main()
{
    std::cout << "Assignment 3 test cases\n";
    testStrategies();
    testFactories();
    testDecorators();
    testGame();
    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
