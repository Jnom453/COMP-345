#ifndef TOWER_H
#define TOWER_H

#include <memory>
#include <string>
#include <vector>
#include "Observable.h"
#include "TargetingStrategy.h"

class Critter;
class Player;

/**
 * @file Tower.h
 * @brief Base class of all towers.
 */

/**
 * @brief Base class of every tower in the game.
 *
 * A tower has a buying cost, a refund value, a range, a power (damage per
 * hit) and a rate of fire. Subclasses (DirectDamageTower, AreaDamageTower,
 * SlowTower) differ in how they attack, by overriding attack().
 *
 * Two design patterns are built around this class:
 * - Strategy: the tower holds a TargetingStrategy that chooses which critter
 *   in range to shoot (see setStrategy() and chooseTarget()).
 * - Decorator: TowerDecorator and its subclasses wrap a Tower to change its
 *   level, statistics or attack effects. For this reason all the getters and
 *   the attack functions are virtual: a decorator answers in place of the
 *   tower it wraps.
 *
 * Game rules:
 * - A tower starts at level 1. The maximum level is 3.
 * - Upgrading costs buyingCost x current level coins. Each upgrade adds
 *   5 power and 1.0 range. The rate of fire does not change.
 * - An upgrade is refused, and nothing changes, if the tower is already at
 *   the maximum level or the player cannot afford it.
 *
 * Tower is an Observable: it notifies its observers after a successful
 * upgrade (see TowerObserver).
 */
class Tower : public Observable
{
protected:
    /** @brief Display name of the tower type. */
    std::string name;

    /** @brief Current level, starting at 1. */
    int level;
    /** @brief Highest level the tower can reach. */
    int maxLevel;

    /** @brief Coins needed to buy the tower. */
    int buyingCost;
    /** @brief Coins the player would get back by selling the tower. */
    int refundValue;

    /** @brief Distance within which the tower can shoot critters, in cells. */
    double range;
    /** @brief Damage inflicted on a critter each time the tower hits. */
    int power;
    /** @brief Shots per second. */
    double rateOfFire;

    /** @brief How the tower chooses among several critters in range. */
    std::shared_ptr<TargetingStrategy> strategy;

public:
    /**
     * @brief Creates a level 1 tower.
     *
     * The targeting strategy starts as NearestToExitStrategy.
     *
     * @param name        Name of the tower type.
     * @param buyingCost  Coins needed to buy it.
     * @param refundValue Coins returned when it is sold.
     * @param range       Shooting range.
     * @param power       Damage per hit.
     * @param rateOfFire  Shots per second.
     */
    Tower(
        std::string name,
        int buyingCost,
        int refundValue,
        double range,
        int power,
        double rateOfFire
    );

    virtual ~Tower();

    /** @brief Returns the tower type name. */
    virtual std::string getName() const;
    /** @brief Returns the current level. */
    virtual int getLevel() const;
    /** @brief Returns the highest level the tower can reach. */
    virtual int getMaxLevel() const;
    /** @brief Returns the buying cost in coins. */
    virtual int getBuyingCost() const;
    /** @brief Returns the refund value in coins. */
    virtual int getRefundValue() const;
    /** @brief Returns the shooting range. */
    virtual double getRange() const;
    /** @brief Returns the damage per hit. */
    virtual int getPower() const;
    /** @brief Returns the shots per second. */
    virtual double getRateOfFire() const;

    /**
     * @brief Returns the coins the next level costs: buying cost x current level.
     *
     * It is computed with the virtual getters, so a decorated tower gives the
     * price of its own next level.
     */
    virtual int getUpgradeCost() const;

    /**
     * @brief Upgrades the tower by one level and notifies the observers.
     *
     * Game rules: costs buyingCost x current level coins, then adds 5 power
     * and 1.0 range. Fails, with no change and no notification, if the tower
     * is at its maximum level or the player cannot pay.
     *
     * This is the upgrade used by the Assignment 2 demonstration. In the
     * game, upgrades are done by wrapping the tower in a LevelUpDecorator.
     *
     * @param player The player who pays for the upgrade.
     * @return true if the tower was upgraded.
     */
    virtual bool upgrade(Player& player);

    /**
     * @brief Attacks one critter. The default tower deals its power as damage.
     *
     * Overridden by the tower types to give them their own behavior.
     *
     * @param target The critter being shot.
     */
    virtual void attack(Critter& target);

    /**
     * @brief Attacks a critter, knowing which other critters are next to it.
     *
     * The default is to attack the target only. Decorators such as
     * SplashDecorator override it to also hit the neighbours.
     *
     * @param target The critter being shot.
     * @param nearby Other critters within getSplashRadius() of the target.
     */
    virtual void attackWithNearby(Critter& target, const std::vector<Critter*>& nearby);

    /**
     * @brief Tells whether the tower hits every critter in range at once.
     *
     * false for most towers; AreaDamageTower returns true.
     */
    virtual bool attacksAll() const;

    /**
     * @brief Attacks all the given critters. Used when attacksAll() is true.
     *
     * The default attacks each living critter in turn.
     *
     * @param targets The critters in range.
     */
    virtual void attackAll(std::vector<Critter*>& targets);

    /**
     * @brief Returns the distance around the target in which neighbours are
     *        also hit (0 means no splash).
     */
    virtual double getSplashRadius() const;

    /**
     * @brief Tells whether the tower has a given special effect.
     * @param effectName For example "Splash", "Burning" or "Freezing".
     */
    virtual bool hasEffect(const std::string& effectName) const;

    /** @brief Returns the special effects as text, for example "Splash, Burning". */
    virtual std::string getEffects() const;

    /** @brief Returns the targeting strategy the tower uses. */
    virtual std::shared_ptr<TargetingStrategy> getStrategy() const;

    /**
     * @brief Replaces the targeting strategy. Can be done at any time.
     * @param newStrategy The new strategy. A null pointer is ignored.
     */
    virtual void setStrategy(std::shared_ptr<TargetingStrategy> newStrategy);

    /**
     * @brief Chooses a target among the critters in range, using the strategy.
     * @param candidates The critters in range.
     * @return The index of the chosen candidate, or -1 if there is none.
     */
    virtual int chooseTarget(const std::vector<TargetCandidate>& candidates) const;

    /** @brief Returns a short text describing what the tower does. */
    virtual std::string getDescription() const;
};

#endif
