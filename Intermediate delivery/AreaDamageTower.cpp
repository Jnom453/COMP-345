#include "AreaDamageTower.h"
#include "Critter.h"

AreaDamageTower::AreaDamageTower()
    : Tower(
        "Area Damage Tower",
        150,
        75,
        2.5,
        12,
        0.7
    )
{
}

void AreaDamageTower::attack(Critter& target)
{
    target.takeDamage(power);
}

void AreaDamageTower::attackMultiple(
    std::vector<Critter*>& targets
)
{
    for (std::size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i] != NULL)
        {
            if (targets[i]->isAlive())
            {
                targets[i]->takeDamage(power);
            }
        }
    }
}

std::string AreaDamageTower::getDescription() const
{
    return "Deals damage to multiple critters.";
}