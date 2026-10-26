#include "CritterGroup.h"
#include <iostream>

using namespace std;

void CritterGroup::addCritter(const Critter& critter)
{
    critters.push_back(critter);
}

bool CritterGroup::removeCritter(int index)
{
    if (index < 0 ||
        index >= static_cast<int>(critters.size()))
    {
        return false;
    }

    critters.erase(critters.begin() + index);
    return true;
}

int CritterGroup::getSize() const
{
    return static_cast<int>(critters.size());
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

    for (std::size_t i = 0; i < critters.size(); ++i)
    {
        critters[i].display();
    }
}