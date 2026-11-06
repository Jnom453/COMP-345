#include "Tower.h"
#include "Critter.h"
#include "Player.h"

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

std::string Tower::getDescription() const
{
    return "Basic tower";
}