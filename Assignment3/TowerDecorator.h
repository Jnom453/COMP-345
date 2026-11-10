#ifndef TOWER_DECORATOR_H
#define TOWER_DECORATOR_H

#include <memory>
#include "Tower.h"

/**
 * @file TowerDecorator.h
 * @brief Decorator pattern: base class of the decorators that change a Tower.
 */

/**
 * @brief A Tower that wraps another Tower and forwards everything to it.
 *
 * This is the Decorator design pattern. A decorator is itself a Tower, so the
 * game and the other decorators can use it exactly like a plain tower. It
 * owns the tower it wraps ("component") and by default passes every call on
 * to it. The concrete decorators (LevelUpDecorator, SplashDecorator,
 * BurningDecorator, FreezingDecorator) override only the few functions they
 * change, so decorators can be stacked in any order, for example
 * Burning(Splash(LevelUp(DirectDamageTower))).
 *
 * Game rule: upgrading a tower during play changes its behavior; this is
 * done by wrapping the tower in a new decorator instead of editing the tower.
 */
class TowerDecorator : public Tower
{
protected:
    /** @brief The tower being decorated. Owned by the decorator. */
    std::unique_ptr<Tower> wrapped;

public:
    /**
     * @brief Wraps a tower.
     * @param tower The tower to decorate. The decorator takes ownership.
     */
    explicit TowerDecorator(std::unique_ptr<Tower> tower);

    virtual ~TowerDecorator();

    /** @brief Returns the name of the wrapped tower. */
    virtual std::string getName() const;
    /** @brief Returns the level of the wrapped tower. */
    virtual int getLevel() const;
    /** @brief Returns the maximum level of the wrapped tower. */
    virtual int getMaxLevel() const;
    /** @brief Returns the buying cost of the wrapped tower. */
    virtual int getBuyingCost() const;
    /** @brief Returns the refund value of the wrapped tower. */
    virtual int getRefundValue() const;
    /** @brief Returns the range of the wrapped tower. */
    virtual double getRange() const;
    /** @brief Returns the power of the wrapped tower. */
    virtual int getPower() const;
    /** @brief Returns the rate of fire of the wrapped tower. */
    virtual double getRateOfFire() const;

    /** @brief Upgrades the wrapped tower (see Tower::upgrade()). */
    virtual bool upgrade(Player& player);

    /** @brief Attacks with the wrapped tower. */
    virtual void attack(Critter& target);
    /** @brief Attacks with the wrapped tower, passing the neighbours on. */
    virtual void attackWithNearby(Critter& target, const std::vector<Critter*>& nearby);
    /** @brief Returns whether the wrapped tower hits every critter in range. */
    virtual bool attacksAll() const;
    /** @brief Attacks all the critters with the wrapped tower. */
    virtual void attackAll(std::vector<Critter*>& targets);

    /** @brief Returns the splash radius of the wrapped tower. */
    virtual double getSplashRadius() const;
    /** @brief Returns whether the wrapped tower (or a decorator inside) has an effect. */
    virtual bool hasEffect(const std::string& effectName) const;
    /** @brief Returns the effects of the wrapped tower. */
    virtual std::string getEffects() const;

    /** @brief Returns the strategy of the wrapped tower. */
    virtual std::shared_ptr<TargetingStrategy> getStrategy() const;
    /** @brief Changes the strategy of the wrapped tower. */
    virtual void setStrategy(std::shared_ptr<TargetingStrategy> newStrategy);
    /** @brief Chooses a target with the strategy of the wrapped tower. */
    virtual int chooseTarget(const std::vector<TargetCandidate>& candidates) const;

    /** @brief Returns the description of the wrapped tower. */
    virtual std::string getDescription() const;
};

#endif
