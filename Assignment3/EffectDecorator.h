#ifndef EFFECT_DECORATOR_H
#define EFFECT_DECORATOR_H

#include "TowerDecorator.h"

/**
 * @file EffectDecorator.h
 * @brief Base class of the decorators that add a special effect to a tower's hits.
 */

/**
 * @brief A decorator that applies an extra effect to every critter its tower hits.
 *
 * The tower attacks normally (through the wrapped tower), and then
 * applyEffect() is called for the critter that was hit. BurningDecorator and
 * FreezingDecorator only have to define the effect; this class takes care of
 * the three ways a tower can attack (one target, a target with neighbours,
 * or all critters in range) and of the bookkeeping of the effect name.
 */
class EffectDecorator : public TowerDecorator
{
public:
    /**
     * @brief Wraps a tower.
     * @param tower The tower to decorate. The decorator takes ownership.
     */
    explicit EffectDecorator(std::unique_ptr<Tower> tower);

    /** @brief Attacks the target, then applies the effect to it. */
    void attack(Critter& target);
    /** @brief Attacks the target and its neighbours, then applies the effect to the target. */
    void attackWithNearby(Critter& target, const std::vector<Critter*>& nearby);
    /** @brief Attacks all critters, then applies the effect to each living one. */
    void attackAll(std::vector<Critter*>& targets);

    /** @brief Returns true for this decorator's own effect name, or an effect inside. */
    bool hasEffect(const std::string& effectName) const;
    /** @brief Returns the effects inside followed by this decorator's own effect name. */
    std::string getEffects() const;
    /** @brief Returns the wrapped description followed by " + " and the effect name. */
    std::string getDescription() const;

    /** @brief Returns the name of this effect, for example "Burning". */
    virtual std::string getEffectName() const = 0;

protected:
    /**
     * @brief Applies the effect to a critter that was just hit.
     * @param target The critter that was hit.
     */
    virtual void applyEffect(Critter& target) = 0;
};

#endif
