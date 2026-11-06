#ifndef SLOW_TOWER_H
#define SLOW_TOWER_H

#include "Tower.h"

/**
 * @file SlowTower.h
 * @brief Tower that damages and slows a critter.
 */

/**
 * @brief Special effect tower: each shot damages a critter and halves its speed.
 *
 * Starting stats (level 1): cost 125, refund 60, range 4.0, power 8,
 * rate of fire 0.5. It does little damage but keeps critters inside the
 * range of the other towers for longer.
 */
class SlowTower : public Tower
{
public:
    /** @brief Creates a level 1 slow tower with its starting stats. */
    SlowTower();

    /**
     * @brief Damages the critter, then multiplies its speed by 0.5.
     * @param target The critter being shot.
     */
    void attack(Critter& target);

    /** @brief Returns "Deals damage and slows the target." */
    std::string getDescription() const;
};

#endif
