#include "FreezingDecorator.h"
#include "Critter.h"

const double FreezingDecorator::DURATION = 2.0;

FreezingDecorator::FreezingDecorator(std::unique_ptr<Tower> tower)
    : EffectDecorator(std::move(tower))
{
}

int FreezingDecorator::getRefundValue() const
{
    return wrapped->getRefundValue() + COST / 2;
}

std::string FreezingDecorator::getEffectName() const
{
    return "Freezing";
}

void FreezingDecorator::applyEffect(Critter& target)
{
    target.freeze(DURATION);
}
