#include "SplashDecorator.h"
#include "Critter.h"

const double SplashDecorator::SPLASH_RADIUS = 1.5;

SplashDecorator::SplashDecorator(std::unique_ptr<Tower> tower)
    : TowerDecorator(std::move(tower))
{
}

// The wrapped tower attacks the target as usual. Then every other living
// critter in "nearby" (the game already filtered the ones within the splash
// radius of the target) takes half of the tower's power.
void SplashDecorator::attackWithNearby(Critter& target, const std::vector<Critter*>& nearby)
{
    wrapped->attackWithNearby(target, nearby);

    int splashDamage = getPower() / 2;
    if (splashDamage < 1)
    {
        splashDamage = 1;
    }
    for (std::size_t i = 0; i < nearby.size(); ++i)
    {
        if (nearby[i] != NULL && nearby[i] != &target && nearby[i]->isAlive())
        {
            nearby[i]->takeDamage(splashDamage);
        }
    }
}

double SplashDecorator::getSplashRadius() const
{
    // Take the larger radius if a splash decorator is already inside.
    double inside = wrapped->getSplashRadius();
    return inside > SPLASH_RADIUS ? inside : SPLASH_RADIUS;
}

int SplashDecorator::getRefundValue() const
{
    return wrapped->getRefundValue() + COST / 2;
}

bool SplashDecorator::hasEffect(const std::string& effectName) const
{
    return effectName == "Splash" || wrapped->hasEffect(effectName);
}

std::string SplashDecorator::getEffects() const
{
    std::string inside = wrapped->getEffects();
    if (inside.empty())
    {
        return "Splash";
    }
    return inside + ", Splash";
}

std::string SplashDecorator::getDescription() const
{
    return wrapped->getDescription() + " + Splash";
}
