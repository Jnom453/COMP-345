#ifndef LEVEL_UP_DECORATOR_H
#define LEVEL_UP_DECORATOR_H

#include "TowerDecorator.h"

/**
 * @file LevelUpDecorator.h
 * @brief Decorator that makes a tower go up one level.
 */

/**
 * @brief Raises the wrapped tower by one level.
 *
 * Game rules (the same as Tower::upgrade()): each level adds 5 power and 1.0
 * range. The price of the next level is the buying cost times the current
 * level, and the price that was paid is partly returned when the tower is
 * sold: the refund value goes up by half of the price of this upgrade.
 *
 * The decorator does not take the coins itself. Game::upgradeTower() asks the
 * tower for getUpgradeCost(), takes the coins from the player and then wraps
 * the tower in a LevelUpDecorator.
 */
class LevelUpDecorator : public TowerDecorator
{
private:
    /** @brief Coins that were paid for this level. */
    int pricePaid;

public:
    /**
     * @brief Wraps a tower and raises it one level.
     * @param tower The tower to upgrade. The decorator takes ownership.
     */
    explicit LevelUpDecorator(std::unique_ptr<Tower> tower);

    /** @brief Returns the wrapped level plus one. */
    int getLevel() const;
    /** @brief Returns the wrapped refund plus half of the price paid for this level. */
    int getRefundValue() const;
    /** @brief Returns the wrapped range plus 1.0. */
    double getRange() const;
    /** @brief Returns the wrapped power plus 5. */
    int getPower() const;
    /** @brief Returns the wrapped description. */
    std::string getDescription() const;

    /**
     * @brief Attacks with the wrapped tower, then adds the bonus power.
     *
     * The concrete towers deal damage with their own power value, so the
     * extra power of the new level is dealt here as additional damage.
     */
    void attack(Critter& target);
    /** @brief Same as attack(), but the wrapped tower may also hit neighbours. */
    void attackWithNearby(Critter& target, const std::vector<Critter*>& nearby);
    /** @brief Attacks every critter in the list, adding the bonus power to each. */
    void attackAll(std::vector<Critter*>& targets);

    /** @brief Power added by one level. */
    static const int POWER_BONUS = 5;
};

#endif
