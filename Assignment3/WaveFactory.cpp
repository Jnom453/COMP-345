#include "WaveFactory.h"

int WaveFactory::critterCount(int waveNumber) const
{
    if (waveNumber < 1)
    {
        waveNumber = 1;
    }
    return 3 + waveNumber;
}

// The rules of the wave composition are all here: a place in the wave goes to
// the first factory whose condition holds (Boss, then Tank, then Fast).
const CritterFactory& WaveFactory::chooseFactory(int waveNumber, int index, int count) const
{
    if (waveNumber >= 8 && index == count - 1)
    {
        return bossFactory;
    }
    if (waveNumber >= 5 && index % 4 == 3)
    {
        return tankFactory;
    }
    if (waveNumber >= 3 && index % 3 == 2)
    {
        return fastFactory;
    }
    return normalFactory;
}

CritterGroup WaveFactory::createWave(int wavesDestroyed) const
{
    if (wavesDestroyed < 0)
    {
        wavesDestroyed = 0;
    }
    int waveNumber = wavesDestroyed + 1;
    int count = critterCount(waveNumber);

    CritterGroup group;
    for (int i = 0; i < count; ++i)
    {
        const CritterFactory& factory = chooseFactory(waveNumber, i, count);
        group.addCritter(factory.createCritter(waveNumber));
    }
    return group;
}
