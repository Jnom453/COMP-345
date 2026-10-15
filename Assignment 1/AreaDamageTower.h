#ifndef AREA_DAMAGE_TOWER_H
#define AREA_DAMAGE_TOWER_H

#include "Tower.h"
#include <vector>

class AreaDamageTower : public Tower
{
public:
    AreaDamageTower();

    void attack(Critter& target);

    void attackMultiple(std::vector<Critter*>& targets);

    std::string getDescription() const;
};

#endif