#include "DirectDamageTower.h"
#include "Critter.h"

DirectDamageTower::DirectDamageTower()
    : Tower(
        "Direct Damage Tower",
        100,
        50,
        3.0,
        20,
        1.0
    )
{
}

void DirectDamageTower::attack(Critter& target)
{
    target.takeDamage(power);
}

std::string DirectDamageTower::getDescription() const
{
    return "Deals direct damage to one critter.";
}