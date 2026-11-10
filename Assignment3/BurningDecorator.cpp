#include "BurningDecorator.h"
#include "Critter.h"

const double BurningDecorator::DURATION = 3.0;

BurningDecorator::BurningDecorator(std::unique_ptr<Tower> tower)
    : EffectDecorator(std::move(tower))
{
}

int BurningDecorator::getRefundValue() const
{
    return wrapped->getRefundValue() + COST / 2;
}

std::string BurningDecorator::getEffectName() const
{
    return "Burning";
}

void BurningDecorator::applyEffect(Critter& target)
{
    int damagePerSecond = getPower() / 4;
    if (damagePerSecond < 1)
    {
        damagePerSecond = 1;
    }
    target.ignite(damagePerSecond, DURATION);
}
