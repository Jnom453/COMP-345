#ifndef CRITTER_GROUP_GENERATOR_H
#define CRITTER_GROUP_GENERATOR_H

#include "CritterGroup.h"

/**
 * @file CritterGroupGenerator.h
 * @brief Creates the critters of each wave.
 */

/**
 * @brief Builds a wave of critters that gets harder with the wave number.
 *
 * Game rule: each wave is stronger than the one before it. For wave n:
 * - number of critters: 3 + n
 * - level: n
 * - hit points: 50 + 15n
 * - strength: 5 + n
 * - speed: 1.0 + 0.1n
 * - reward: 10n coins
 */
class CritterGroupGenerator
{
public:
    CritterGroupGenerator();

    /**
     * @brief Creates the critters of one wave.
     * @param waveNumber Wave number, starting at 1. Values below 1 are treated as 1.
     * @return A CritterGroup holding the new critters.
     */
    CritterGroup generateWave(int waveNumber) const;
};

#endif
