#ifndef CRITTER_GROUP_H
#define CRITTER_GROUP_H

#include <vector>
#include "Critter.h"

class CritterGroup
{
private:
    std::vector<Critter> critters;

public:
    void addCritter(const Critter& critter);
    bool removeCritter(int index);

    int getSize() const;

    Critter& getCritter(int index);
    const Critter& getCritter(int index) const;

    void display() const;
};

#endif