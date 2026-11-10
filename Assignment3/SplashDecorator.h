#ifndef SPLASH_DECORATOR_H
#define SPLASH_DECORATOR_H

#include "TowerDecorator.h"

/**
 * @file SplashDecorator.h
 * @brief Decorator that damages the critters next to the target.
 */

/**
 * @brief Splash effect: a hit also damages the critters near the target.
 *
 * Game rule (from the assignment): "splash: damage also applies to critters
 * near the target critter". Here every other critter within SPLASH_RADIUS
 * cells of the target takes half of the tower's power (at least 1).
 */
class SplashDecorator : public TowerDecorator
{
public:
    /** @brief Coins the player pays to add this effect. */
    static const int COST = 80;
    /** @brief Distance around the target, in cells, in which critters are hit. */
    static const double SPLASH_RADIUS;

    /**
     * @brief Wraps a tower.
     * @param tower The tower to decorate. The decorator takes ownership.
     */
    explicit SplashDecorator(std::unique_ptr<Tower> tower);

    /** @brief Attacks the target, then damages the neighbours with half the power. */
    void attackWithNearby(Critter& target, const std::vector<Critter*>& nearby);

    /** @brief Returns SPLASH_RADIUS. */
    double getSplashRadius() const;
    /** @brief Returns the wrapped refund plus half of COST. */
    int getRefundValue() const;
    /** @brief Returns true for "Splash", or an effect inside. */
    bool hasEffect(const std::string& effectName) const;
    /** @brief Returns the effects inside followed by "Splash". */
    std::string getEffects() const;
    /** @brief Returns the wrapped description followed by " + Splash". */
    std::string getDescription() const;
};

#endif
