#ifndef CRITTER_H
#define CRITTER_H

#include "Observable.h"

class Critter : public Observable
{
private:
    int reward;
    int hitPoints;
    int strength;
    double speed;
    int level;

    double position;

    bool alive;

public:
    Critter(
        int level,
        int hitPoints,
        int strength,
        double speed,
        int reward
    );

    int getReward() const;
    int getHitPoints() const;
    int getStrength() const;
    double getSpeed() const;
    int getLevel() const;

    double getPosition() const;

    bool isAlive() const;

    void takeDamage(int damage);

    void slow(double multiplier);

    void move();

    void reachExit();

    void display() const;
};

#endif