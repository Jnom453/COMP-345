#ifndef BURNING_DECORATOR_H
#define BURNING_DECORATOR_H

#include "EffectDecorator.h"

/**
 * @file BurningDecorator.h
 * @brief Decorator that sets the hit critter on fire.
 */

/**
 * @brief Burning effect: the damage of a hit is re-applied for a few seconds.
 *
 * Game rule (from the assignment): "burning: damage to the target critter is
 * re-applied for a few seconds after a hit". Here a hit sets the critter on
 * fire for 3 seconds, and the fire deals a quarter of the tower's power (at
 * least 1) every second. Being hit again restarts the fire; it does not add up.
 */
class BurningDecorator : public EffectDecorator
{
public:
    /** @brief Coins the player pays to add this effect. */
    static const int COST = 90;
    /** @brief Seconds the fire lasts. */
    static const double DURATION;

    /**
     * @brief Wraps a tower.
     * @param tower The tower to decorate. The decorator takes ownership.
     */
    explicit BurningDecorator(std::unique_ptr<Tower> tower);

    /** @brief Returns the wrapped refund plus half of COST. */
    int getRefundValue() const;
    /** @brief Returns "Burning". */
    std::string getEffectName() const;

protected:
    /** @brief Sets the critter on fire. */
    void applyEffect(Critter& target);
};

#endif
