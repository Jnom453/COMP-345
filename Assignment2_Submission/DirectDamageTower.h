#ifndef DIRECT_DAMAGE_TOWER_H
#define DIRECT_DAMAGE_TOWER_H

#include "Tower.h"

class DirectDamageTower : public Tower
{
public:
    DirectDamageTower();

    void attack(Critter& target);

    std::string getDescription() const;
};

#endif