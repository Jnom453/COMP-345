#include "CritterGroupGenerator.h"

CritterGroupGenerator::CritterGroupGenerator()
{
}

CritterGroup CritterGroupGenerator::generateWave(
    int waveNumber
) const
{
    CritterGroup group;

    if (waveNumber < 1)
    {
        waveNumber = 1;
    }

    int numberOfCritters = 3 + waveNumber;

    for (int i = 0; i < numberOfCritters; i++)
    {
        int level = waveNumber;

        int hitPoints = 50 + (waveNumber * 15);

        int strength = 5 + waveNumber;

        double speed = 1.0 + (waveNumber * 0.1);

        int reward = 10 * waveNumber;

        Critter critter(
            level,
            hitPoints,
            strength,
            speed,
            reward
        );

        group.addCritter(critter);
    }

    return group;
}