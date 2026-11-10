#include "LevelUpDecorator.h"
#include "Critter.h"

// The price of this level is read before the level changes: at that moment
// the wrapped tower still answers with its old level, so getUpgradeCost()
// gives buying cost x old level, which is the price of the new level.
LevelUpDecorator::LevelUpDecorator(std::unique_ptr<Tower> tower)
    : TowerDecorator(std::move(tower)),
      pricePaid(wrapped->getUpgradeCost())
{
}

int LevelUpDecorator::getLevel() const
{
    return wrapped->getLevel() + 1;
}

int LevelUpDecorator::getRefundValue() const
{
    return wrapped->getRefundValue() + pricePaid / 2;
}

double LevelUpDecorator::getRange() const
{
    return wrapped->getRange() + 1.0;
}

int LevelUpDecorator::getPower() const
{
    return wrapped->getPower() + POWER_BONUS;
}

std::string LevelUpDecorator::getDescription() const
{
    return wrapped->getDescription();
}

void LevelUpDecorator::attack(Critter& target)
{
    wrapped->attack(target);
    target.takeDamage(POWER_BONUS);
}

void LevelUpDecorator::attackWithNearby(Critter& target, const std::vector<Critter*>& nearby)
{
    wrapped->attackWithNearby(target, nearby);
    target.takeDamage(POWER_BONUS);
}

void LevelUpDecorator::attackAll(std::vector<Critter*>& targets)
{
    wrapped->attackAll(targets);
    for (std::size_t i = 0; i < targets.size(); i++)
    {
        if (targets[i] != NULL && targets[i]->isAlive())
        {
            targets[i]->takeDamage(POWER_BONUS);
        }
    }
}
