#ifndef TOWER_H
#define TOWER_H

#include <string>
#include "Observable.h"

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

public:
    /**
     * @brief Creates a level 1 tower.
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
    std::string getName() const;
    /** @brief Returns the current level. */
    int getLevel() const;
    /** @brief Returns the buying cost in coins. */
    int getBuyingCost() const;
    /** @brief Returns the refund value in coins. */
    int getRefundValue() const;
    /** @brief Returns the shooting range. */
    double getRange() const;
    /** @brief Returns the damage per hit. */
    int getPower() const;
    /** @brief Returns the shots per second. */
    double getRateOfFire() const;

    /**
     * @brief Upgrades the tower by one level and notifies the observers.
     *
     * Game rules: costs buyingCost x current level coins, then adds 5 power
     * and 1.0 range. Fails, with no change and no notification, if the tower
     * is at its maximum level or the player cannot pay.
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

    /** @brief Returns a short text describing what the tower does. */
    virtual std::string getDescription() const;
};

#endif
