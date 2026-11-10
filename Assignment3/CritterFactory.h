#ifndef CRITTER_FACTORY_H
#define CRITTER_FACTORY_H

#include <string>
#include "Critter.h"

/**
 * @file CritterFactory.h
 * @brief Factory pattern: creates critters of one kind.
 */

/**
 * @brief Abstract factory that creates critters of one kind for a given wave.
 *
 * This is the Factory (Factory Method) design pattern. Code that needs a
 * critter (the WaveFactory) only knows this interface and asks it to
 * createCritter(); each concrete factory decides the kind and the statistics
 * of the critter it builds. A new kind of critter is added by writing a new
 * factory, without changing the code that uses the factories.
 *
 * The statistics grow with the wave number, so a later wave is harder.
 */
class CritterFactory
{
public:
    virtual ~CritterFactory() {}

    /**
     * @brief Creates one critter for a wave.
     * @param waveNumber Number of the wave, starting at 1 (values below 1 count as 1).
     * @return A new critter of this factory's kind.
     */
    virtual Critter createCritter(int waveNumber) const = 0;

    /** @brief Returns the kind of critter this factory builds. */
    virtual std::string getKind() const = 0;
};

/**
 * @brief Builds the standard critter. Same statistics as CritterGroupGenerator.
 *
 * For wave n: hit points 50 + 15n, strength 5 + n, speed 1.0 + 0.1n, reward 10n.
 */
class NormalCritterFactory : public CritterFactory
{
public:
    Critter createCritter(int waveNumber) const;
    std::string getKind() const;
};

/**
 * @brief Builds a fast critter: few hit points but quick.
 *
 * For wave n: hit points 30 + 10n, strength 4 + n, speed 1.6 + 0.1n, reward 12n.
 */
class FastCritterFactory : public CritterFactory
{
public:
    Critter createCritter(int waveNumber) const;
    std::string getKind() const;
};

/**
 * @brief Builds a tank critter: slow, with many hit points.
 *
 * For wave n: hit points 120 + 30n, strength 8 + n, speed 0.7 + 0.05n, reward 18n.
 */
class TankCritterFactory : public CritterFactory
{
public:
    Critter createCritter(int waveNumber) const;
    std::string getKind() const;
};

/**
 * @brief Builds a boss critter: very strong, steals a lot, pays a lot.
 *
 * For wave n: hit points 300 + 60n, strength 15 + 2n, speed 0.9 + 0.05n, reward 40n.
 */
class BossCritterFactory : public CritterFactory
{
public:
    Critter createCritter(int waveNumber) const;
    std::string getKind() const;
};

#endif
