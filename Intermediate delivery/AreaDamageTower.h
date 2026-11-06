#ifndef AREA_DAMAGE_TOWER_H
#define AREA_DAMAGE_TOWER_H

#include "Tower.h"
#include <vector>

/**
 * @file AreaDamageTower.h
 * @brief Tower that damages several critters at once.
 */

/**
 * @brief Area of effect tower: a shot damages every critter in a group.
 *
 * Starting stats (level 1): cost 150, refund 75, range 2.5, power 12,
 * rate of fire 0.7. It costs more and hits harder in total than the direct
 * damage tower because it can hit several critters per shot.
 */
class AreaDamageTower : public Tower
{
public:
    /** @brief Creates a level 1 area damage tower with its starting stats. */
    AreaDamageTower();

    /**
     * @brief Inflicts the tower's power as damage on one critter.
     * @param target The critter being shot.
     */
    void attack(Critter& target);

    /**
     * @brief Damages every living critter in the list.
     *
     * Null pointers and critters that are already dead are skipped.
     *
     * @param targets The critters caught in the area of effect.
     */
    void attackMultiple(std::vector<Critter*>& targets);

    /** @brief Returns "Deals damage to multiple critters." */
    std::string getDescription() const;
};

#endif
