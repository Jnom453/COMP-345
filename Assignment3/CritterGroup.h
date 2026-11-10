#ifndef CRITTER_GROUP_H
#define CRITTER_GROUP_H

#include <vector>
#include "Critter.h"
#include "Observable.h"

/**
 * @file CritterGroup.h
 * @brief A wave of critters.
 */

/**
 * @brief The group of critters that makes up one wave.
 *
 * Critters are stored by value, so addCritter() stores a copy of the critter
 * it receives.
 *
 * CritterGroup is an Observable: it notifies its observers whenever a critter
 * is added or removed (see CritterGroupObserver).
 */
class CritterGroup : public Observable
{
private:
    /** @brief The critters of the wave, in order of appearance. */
    std::vector<Critter> critters;

public:
    /**
     * @brief Adds a copy of a critter at the end of the group and notifies the observers.
     * @param critter The critter to add.
     */
    void addCritter(const Critter& critter);

    /**
     * @brief Removes a critter from the group and notifies the observers.
     * @param index Position of the critter, starting at 0.
     * @return true if a critter was removed, false if the index is invalid.
     */
    bool removeCritter(int index);

    /** @brief Returns the number of critters in the group. */
    int getSize() const;

    /**
     * @brief Returns the critter at a position so it can be modified.
     * @param index Position of the critter, starting at 0.
     * @throws std::out_of_range if the index is invalid.
     */
    Critter& getCritter(int index);

    /**
     * @brief Returns the critter at a position for reading only.
     * @param index Position of the critter, starting at 0.
     * @throws std::out_of_range if the index is invalid.
     */
    const Critter& getCritter(int index) const;

    /** @brief Prints every critter of the group. */
    void display() const;
};

#endif
