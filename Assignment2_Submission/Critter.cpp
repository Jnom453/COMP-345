#include "Critter.h"
#include <iostream>

using namespace std;

Critter::Critter(
    int level,
    int hitPoints,
    int strength,
    double speed,
    int reward
)
{
    this->level = level;
    this->hitPoints = hitPoints;
    this->strength = strength;
    this->speed = speed;
    this->reward = reward;

    position = 0.0;
    alive = true;
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

void Critter::move()
{
    if (alive)
    {
        position = position + speed;
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