#include "SlowTower.h"
#include "Critter.h"

SlowTower::SlowTower()
    : Tower(
        "Slow Tower",
        125,
        60,
        4.0,
        8,
        0.5
    )
{
}

void SlowTower::attack(Critter& target)
{
    target.takeDamage(power);
    target.slow(0.5);
}

std::string SlowTower::getDescription() const
{
    return "Deals damage and slows the target.";
}