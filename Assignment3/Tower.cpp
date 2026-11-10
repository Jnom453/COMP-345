#include "Tower.h"
#include "Critter.h"
#include "Player.h"
#include "TargetingStrategy.h"

Tower::Tower(
    std::string name,
    int buyingCost,
    int refundValue,
    double range,
    int power,
    double rateOfFire
)
{
    this->name = name;

    level = 1;
    maxLevel = 3;

    this->buyingCost = buyingCost;
    this->refundValue = refundValue;

    this->range = range;
    this->power = power;
    this->rateOfFire = rateOfFire;

    // Every tower starts by shooting the critter closest to the exit.
    strategy = createStrategy(NEAREST_TO_EXIT);
}

Tower::~Tower()
{
}

std::string Tower::getName() const
{
    return name;
}

int Tower::getLevel() const
{
    return level;
}

int Tower::getMaxLevel() const
{
    return maxLevel;
}

int Tower::getBuyingCost() const
{
    return buyingCost;
}

int Tower::getRefundValue() const
{
    return refundValue;
}

double Tower::getRange() const
{
    return range;
}

int Tower::getPower() const
{
    return power;
}

double Tower::getRateOfFire() const
{
    return rateOfFire;
}

bool Tower::upgrade(Player& player)
{
    if (level >= maxLevel)
    {
        return false;
    }

    int upgradeCost = buyingCost * level;

    if (!player.spendCoins(upgradeCost))
    {
        return false;
    }

    level++;

    power = power + 5;
    range = range + 1.0;

    notifyObservers();

    return true;
}

void Tower::attack(Critter& target)
{
    target.takeDamage(power);
}

// The price of the next level uses the virtual getters, so a tower wrapped in
// decorators gives the price of its own next level.
int Tower::getUpgradeCost() const
{
    return getBuyingCost() * getLevel();
}

// The default tower has no neighbours effect: it only attacks the target.
void Tower::attackWithNearby(Critter& target, const std::vector<Critter*>& nearby)
{
    (void)nearby;
    attack(target);
}

bool Tower::attacksAll() const
{
    return false;
}

void Tower::attackAll(std::vector<Critter*>& targets)
{
    for (std::size_t i = 0; i < targets.size(); ++i)
    {
        if (targets[i] != NULL && targets[i]->isAlive())
        {
            attack(*targets[i]);
        }
    }
}

double Tower::getSplashRadius() const
{
    return 0.0;
}

bool Tower::hasEffect(const std::string& effectName) const
{
    (void)effectName;
    return false;
}

std::string Tower::getEffects() const
{
    return "";
}

std::shared_ptr<TargetingStrategy> Tower::getStrategy() const
{
    return strategy;
}

void Tower::setStrategy(std::shared_ptr<TargetingStrategy> newStrategy)
{
    if (newStrategy)
    {
        strategy = newStrategy;
    }
}

int Tower::chooseTarget(const std::vector<TargetCandidate>& candidates) const
{
    return strategy->chooseTarget(candidates);
}

std::string Tower::getDescription() const
{
    return "Basic tower";
}