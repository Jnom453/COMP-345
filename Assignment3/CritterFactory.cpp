#include "CritterFactory.h"

// Each concrete factory only chooses a kind and a formula for the statistics.
// The level of a critter is the number of its wave.

static int validWave(int waveNumber)
{
    return waveNumber < 1 ? 1 : waveNumber;
}

Critter NormalCritterFactory::createCritter(int waveNumber) const
{
    int n = validWave(waveNumber);
    return Critter(n, 50 + 15 * n, 5 + n, 1.0 + 0.1 * n, 10 * n, getKind());
}
std::string NormalCritterFactory::getKind() const { return "Normal"; }

Critter FastCritterFactory::createCritter(int waveNumber) const
{
    int n = validWave(waveNumber);
    return Critter(n, 30 + 10 * n, 4 + n, 1.6 + 0.1 * n, 12 * n, getKind());
}
std::string FastCritterFactory::getKind() const { return "Fast"; }

Critter TankCritterFactory::createCritter(int waveNumber) const
{
    int n = validWave(waveNumber);
    return Critter(n, 120 + 30 * n, 8 + n, 0.7 + 0.05 * n, 18 * n, getKind());
}
std::string TankCritterFactory::getKind() const { return "Tank"; }

Critter BossCritterFactory::createCritter(int waveNumber) const
{
    int n = validWave(waveNumber);
    return Critter(n, 300 + 60 * n, 15 + 2 * n, 0.9 + 0.05 * n, 40 * n, getKind());
}
std::string BossCritterFactory::getKind() const { return "Boss"; }
