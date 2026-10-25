#include "CritterGroup.h"
#include <iostream>

using namespace std;

void CritterGroup::addCritter(const Critter& critter)
{
    critters.push_back(critter);
}

int CritterGroup::getSize() const
{
    return critters.size();
}

Critter& CritterGroup::getCritter(int index)
{
    return critters.at(index);
}

const Critter& CritterGroup::getCritter(int index) const
{
    return critters.at(index);
}

void CritterGroup::display() const
{
    cout << "\nCritter Group\n";

    for (std::size_t int i = 0; i < critters.size(); i++)
    {
        critters[i].display();
    }
}