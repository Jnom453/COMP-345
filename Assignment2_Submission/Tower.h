#ifndef TOWER_H
#define TOWER_H

#include <string>
#include "Observable.h"

class Critter;
class Player;

class Tower : public Observable
{
protected:
    std::string name;

    int level;
    int maxLevel;

    int buyingCost;
    int refundValue;

    double range;
    int power;
    double rateOfFire;

public:
    Tower(
        std::string name,
        int buyingCost,
        int refundValue,
        double range,
        int power,
        double rateOfFire
    );

    virtual ~Tower();

    std::string getName() const;
    int getLevel() const;
    int getBuyingCost() const;
    int getRefundValue() const;
    double getRange() const;
    int getPower() const;
    double getRateOfFire() const;

    virtual bool upgrade(Player& player);

    virtual void attack(Critter& target);

    virtual std::string getDescription() const;
};

#endif