#include "TowerDecorator.h"

// The base decorator does nothing by itself: every function simply calls the
// same function on the wrapped tower. The concrete decorators override the
// functions they want to change and leave the rest to these forwards.

TowerDecorator::TowerDecorator(std::unique_ptr<Tower> tower)
    : Tower("", 0, 0, 0.0, 0, 0.0),
      wrapped(std::move(tower))
{
}

TowerDecorator::~TowerDecorator()
{
}

std::string TowerDecorator::getName() const { return wrapped->getName(); }
int TowerDecorator::getLevel() const { return wrapped->getLevel(); }
int TowerDecorator::getMaxLevel() const { return wrapped->getMaxLevel(); }
int TowerDecorator::getBuyingCost() const { return wrapped->getBuyingCost(); }
int TowerDecorator::getRefundValue() const { return wrapped->getRefundValue(); }
double TowerDecorator::getRange() const { return wrapped->getRange(); }
int TowerDecorator::getPower() const { return wrapped->getPower(); }
double TowerDecorator::getRateOfFire() const { return wrapped->getRateOfFire(); }

bool TowerDecorator::upgrade(Player& player)
{
    return wrapped->upgrade(player);
}

void TowerDecorator::attack(Critter& target)
{
    wrapped->attack(target);
}

void TowerDecorator::attackWithNearby(Critter& target, const std::vector<Critter*>& nearby)
{
    wrapped->attackWithNearby(target, nearby);
}

bool TowerDecorator::attacksAll() const { return wrapped->attacksAll(); }

void TowerDecorator::attackAll(std::vector<Critter*>& targets)
{
    wrapped->attackAll(targets);
}

double TowerDecorator::getSplashRadius() const { return wrapped->getSplashRadius(); }

bool TowerDecorator::hasEffect(const std::string& effectName) const
{
    return wrapped->hasEffect(effectName);
}

std::string TowerDecorator::getEffects() const { return wrapped->getEffects(); }

std::shared_ptr<TargetingStrategy> TowerDecorator::getStrategy() const
{
    return wrapped->getStrategy();
}

void TowerDecorator::setStrategy(std::shared_ptr<TargetingStrategy> newStrategy)
{
    wrapped->setStrategy(newStrategy);
}

int TowerDecorator::chooseTarget(const std::vector<TargetCandidate>& candidates) const
{
    return wrapped->chooseTarget(candidates);
}

std::string TowerDecorator::getDescription() const
{
    return wrapped->getDescription();
}
