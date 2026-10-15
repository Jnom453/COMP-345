#ifndef SLOW_TOWER_H
#define SLOW_TOWER_H

#include "Tower.h"

class SlowTower : public Tower
{
public:
    SlowTower();

    void attack(Critter& target);

    std::string getDescription() const;
};

#endif