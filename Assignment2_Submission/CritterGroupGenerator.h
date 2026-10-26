#ifndef CRITTER_GROUP_GENERATOR_H
#define CRITTER_GROUP_GENERATOR_H

#include "CritterGroup.h"

class CritterGroupGenerator
{
public:
    CritterGroupGenerator();

    CritterGroup generateWave(int waveNumber) const;
};

#endif