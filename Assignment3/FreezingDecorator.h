#ifndef FREEZING_DECORATOR_H
#define FREEZING_DECORATOR_H

#include "EffectDecorator.h"

/**
 * @file FreezingDecorator.h
 * @brief Decorator that freezes the hit critter.
 */

/**
 * @brief Freezing effect: a hit also slows the critter down for a short time.
 *
 * Game rule (from the assignment): "freezing: damage to the target critter
 * also slows down the critter". Here a hit freezes the critter for 2 seconds,
 * during which it moves at half speed. Being hit again restarts the 2
 * seconds, but it never becomes slower than half speed.
 */
class FreezingDecorator : public EffectDecorator
{
public:
    /** @brief Coins the player pays to add this effect. */
    static const int COST = 70;
    /** @brief Seconds the freeze lasts. */
    static const double DURATION;

    /**
     * @brief Wraps a tower.
     * @param tower The tower to decorate. The decorator takes ownership.
     */
    explicit FreezingDecorator(std::unique_ptr<Tower> tower);

    /** @brief Returns the wrapped refund plus half of COST. */
    int getRefundValue() const;
    /** @brief Returns "Freezing". */
    std::string getEffectName() const;

protected:
    /** @brief Freezes the critter. */
    void applyEffect(Critter& target);
};

#endif
