#ifndef WAVE_FACTORY_H
#define WAVE_FACTORY_H

#include <string>
#include "CritterFactory.h"
#include "CritterGroup.h"

/**
 * @file WaveFactory.h
 * @brief Builds the waves of critters, choosing the kinds from the progress of the game.
 */

/**
 * @brief Creates each wave of critters using the critter factories.
 *
 * The WaveFactory owns one factory per kind of critter and decides, for each
 * place in the wave, which factory to ask. The choice depends on the number
 * of waves already destroyed, so the further the player gets, the more kinds
 * of critters appear and the harder the next wave is.
 *
 * Game rule: waves get harder as more waves are destroyed. For the next wave
 * number n = (waves destroyed) + 1:
 * - the wave has 3 + n critters;
 * - from wave 3, every third critter is a Fast critter;
 * - from wave 5, every fourth critter is a Tank critter;
 * - from wave 8, the last critter of the wave is a Boss;
 * - all other critters are Normal.
 * (A place that qualifies for several kinds takes the first one in the order
 * Boss, Tank, Fast.)
 */
class WaveFactory
{
private:
    NormalCritterFactory normalFactory;
    FastCritterFactory fastFactory;
    TankCritterFactory tankFactory;
    BossCritterFactory bossFactory;

public:
    /**
     * @brief Chooses the factory for one place in a wave.
     *
     * @param waveNumber Number of the wave (waves destroyed + 1).
     * @param index      Place of the critter in the wave, starting at 0.
     * @param count      Number of critters in the wave.
     * @return The factory that builds the critter for that place.
     */
    const CritterFactory& chooseFactory(int waveNumber, int index, int count) const;

    /**
     * @brief Returns the number of critters in a wave (3 + waveNumber).
     * @param waveNumber Number of the wave, starting at 1.
     */
    int critterCount(int waveNumber) const;

    /**
     * @brief Creates a whole wave.
     * @param wavesDestroyed Number of waves destroyed so far (0 before the first wave).
     * @return The critters of the next wave, in the order they appear.
     */
    CritterGroup createWave(int wavesDestroyed) const;
};

#endif
