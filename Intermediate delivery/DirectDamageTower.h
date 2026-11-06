#ifndef DIRECT_DAMAGE_TOWER_H
#define DIRECT_DAMAGE_TOWER_H

#include "Tower.h"

/**
 * @file DirectDamageTower.h
 * @brief Tower that damages a single critter.
 */

/**
 * @brief The basic tower: each shot damages one critter.
 *
 * Starting stats (level 1): cost 100, refund 50, range 3.0, power 20,
 * rate of fire 1.0.
 */
class DirectDamageTower : public Tower
{
public:
    /** @brief Creates a level 1 direct damage tower with its starting stats. */
    DirectDamageTower();

    /**
     * @brief Inflicts the tower's power as damage on one critter.
     * @param target The critter being shot.
     */
    void attack(Critter& target);

    /** @brief Returns "Deals direct damage to one critter." */
    std::string getDescription() const;
};

#endif
