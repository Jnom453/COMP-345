#include "Critter.h"
#include <iostream>

using namespace std;

Critter::Critter(
    int level,
    int hitPoints,
    int strength,
    double speed,
    int reward,
    string kind
)
{
    this->level = level;
    this->hitPoints = hitPoints;
    this->strength = strength;
    this->speed = speed;
    this->reward = reward;

    this->kind = kind;
    position = 0.0;
    alive = true;
    burnDamage = 0;
    burnTime = 0.0;
    burnTick = 0.0;
    freezeTime = 0.0;
}

int Critter::getReward() const
{
    return reward;
}

int Critter::getHitPoints() const
{
    return hitPoints;
}

int Critter::getStrength() const
{
    return strength;
}

double Critter::getSpeed() const
{
    return speed;
}

int Critter::getLevel() const
{
    return level;
}

string Critter::getKind() const
{
    return kind;
}

double Critter::getPosition() const
{
    return position;
}

bool Critter::isAlive() const
{
    return alive;
}

void Critter::takeDamage(int damage)
{
    if (!alive)
    {
        return;
    }

    hitPoints = hitPoints - damage;

    if (hitPoints <= 0)
    {
        hitPoints = 0;
        alive = false;
    }

    notifyObservers();
}

void Critter::slow(double multiplier)
{
    if (multiplier > 0 && multiplier < 1)
    {
        speed = speed * multiplier;
    }
    
    notifyObservers();
}

// A burning critter takes damage once per second. The fire and the freeze
// both count down in updateEffects(), which the game calls every frame.
void Critter::ignite(int damagePerSecond, double seconds)
{
    if (!alive || damagePerSecond <= 0 || seconds <= 0.0)
    {
        return;
    }
    burnDamage = damagePerSecond;
    burnTime = seconds;
    burnTick = 0.0;
}

void Critter::freeze(double seconds)
{
    if (!alive || seconds <= 0.0)
    {
        return;
    }
    freezeTime = seconds;
}

bool Critter::isBurning() const
{
    return alive && burnTime > 0.0;
}

bool Critter::isFrozen() const
{
    return alive && freezeTime > 0.0;
}

void Critter::updateEffects(double seconds)
{
    if (!alive)
    {
        return;
    }

    if (freezeTime > 0.0)
    {
        freezeTime -= seconds;
        if (freezeTime < 0.0)
        {
            freezeTime = 0.0;
        }
    }

    if (burnTime > 0.0)
    {
        burnTime -= seconds;
        burnTick += seconds;
        while (burnTick >= 1.0 - 1e-9 && alive)
        {
            burnTick -= 1.0;
            takeDamage(burnDamage);
        }
        if (burnTime <= 0.0)
        {
            burnTime = 0.0;
            burnDamage = 0;
        }
    }
}

void Critter::move()
{
    if (alive)
    {
        double step = speed;
        if (freezeTime > 0.0)
        {
            step = speed * 0.5;
        }
        position = position + step;
    }

    notifyObservers();
}

void Critter::reachExit()
{
    position = 100.0;

    notifyObservers();
}

void Critter::display() const
{
    cout << "Critter Level: " << level
         << " | HP: " << hitPoints
         << " | Strength: " << strength
         << " | Speed: " << speed
         << " | Reward: " << reward
         << endl;
}