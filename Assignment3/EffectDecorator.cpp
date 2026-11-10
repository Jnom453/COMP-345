#include "EffectDecorator.h"
#include "Critter.h"

EffectDecorator::EffectDecorator(std::unique_ptr<Tower> tower)
    : TowerDecorator(std::move(tower))
{
}

void EffectDecorator::attack(Critter& target)
{
    wrapped->attack(target);
    applyEffect(target);
}

void EffectDecorator::attackWithNearby(Critter& target, const std::vector<Critter*>& nearby)
{
    wrapped->attackWithNearby(target, nearby);
    applyEffect(target);
}

void EffectDecorator::attackAll(std::vector<Critter*>& targets)
{
    wrapped->attackAll(targets);
    for (std::size_t i = 0; i < targets.size(); ++i)
    {
        if (targets[i] != NULL && targets[i]->isAlive())
        {
            applyEffect(*targets[i]);
        }
    }
}

bool EffectDecorator::hasEffect(const std::string& effectName) const
{
    return effectName == getEffectName() || wrapped->hasEffect(effectName);
}

std::string EffectDecorator::getEffects() const
{
    std::string inside = wrapped->getEffects();
    if (inside.empty())
    {
        return getEffectName();
    }
    return inside + ", " + getEffectName();
}

std::string EffectDecorator::getDescription() const
{
    return wrapped->getDescription() + " + " + getEffectName();
}
