#ifndef CRITTER_H
#define CRITTER_H

#include <string>
#include "Observable.h"

/**
 * @file Critter.h
 * @brief An enemy that walks the path from the entry to the exit.
 */

/**
 * @brief An enemy unit of the game.
 *
 * Game rules:
 * - A critter has a level, hit points, strength, speed and reward.
 * - It loses hit points when a tower hits it, and dies when they reach 0.
 *   A critter that is already dead takes no more damage.
 * - Killing a critter earns the player its reward in coins.
 * - A critter that reaches the exit steals coins from the player, equal to
 *   its strength.
 * - A slow effect multiplies its speed by a factor between 0 and 1.
 * - A critter has a kind (for example "Normal", "Fast", "Tank" or "Boss"),
 *   set by the CritterFactory that created it.
 * - Tower effects can also burn it (damage over a few seconds, see ignite())
 *   or freeze it (half speed for a few seconds, see freeze()).
 *
 * The position is a progress value along the path: 0 at the entry, and
 * 100 once the critter has reached the exit.
 *
 * Critter is an Observable: it notifies its observers when it takes damage,
 * is slowed, moves, or reaches the exit (see CritterObserver).
 */
class Critter : public Observable
{
private:
    /** @brief Coins the player earns by killing the critter. */
    int reward;
    /** @brief Remaining hit points. Never below 0. */
    int hitPoints;
    /** @brief Coins stolen from the player if the critter reaches the exit. */
    int strength;
    /** @brief Distance travelled per call to move(). */
    double speed;
    /** @brief Difficulty level, equal to the wave the critter belongs to. */
    int level;

    /** @brief Progress along the path: 0 at the entry, 100 at the exit. */
    double position;

    /** @brief false once the hit points reach 0. */
    bool alive;

    /** @brief Type of critter, for example "Normal" or "Tank". */
    std::string kind;

    /** @brief Damage dealt each second while burning. */
    int burnDamage;
    /** @brief Seconds of burning left. */
    double burnTime;
    /** @brief Seconds since the last burn damage was dealt. */
    double burnTick;
    /** @brief Seconds of freezing left. */
    double freezeTime;

public:
    /**
     * @brief Creates a living critter at the start of the path.
     * @param level     Difficulty level.
     * @param hitPoints Starting hit points.
     * @param strength  Coins stolen if it reaches the exit.
     * @param speed     Distance travelled per move.
     * @param reward    Coins earned by killing it.
     * @param kind      Type of critter, "Normal" by default.
     */
    Critter(
        int level,
        int hitPoints,
        int strength,
        double speed,
        int reward,
        std::string kind = "Normal"
    );

    /** @brief Returns the reward in coins. */
    int getReward() const;
    /** @brief Returns the remaining hit points. */
    int getHitPoints() const;
    /** @brief Returns the strength (coins stolen at the exit). */
    int getStrength() const;
    /** @brief Returns the current speed. */
    double getSpeed() const;
    /** @brief Returns the level. */
    int getLevel() const;

    /** @brief Returns the kind of critter ("Normal", "Fast", "Tank", "Boss"...). */
    std::string getKind() const;

    /** @brief Returns the progress along the path (0 to 100). */
    double getPosition() const;

    /** @brief Returns true while the critter has hit points left. */
    bool isAlive() const;

    /**
     * @brief Removes hit points and notifies the observers.
     *
     * Hit points never go below 0. When they reach 0 the critter dies.
     * Does nothing, and sends no notification, if the critter is already dead.
     *
     * @param damage Hit points to remove.
     */
    void takeDamage(int damage);

    /**
     * @brief Multiplies the speed by a factor, then notifies the observers.
     *
     * The slow is permanent. Only factors strictly between 0 and 1 change the
     * speed; any other value is ignored.
     *
     * @param multiplier Factor applied to the speed, for example 0.5 for half.
     */
    void slow(double multiplier);

    /**
     * @brief Sets the critter on fire (used by the BurningDecorator).
     *
     * For the given time the critter takes the given damage every second.
     * Burning again restarts the time. Does nothing to a dead critter.
     *
     * @param damagePerSecond Damage dealt each second.
     * @param seconds         How long the fire lasts.
     */
    void ignite(int damagePerSecond, double seconds);

    /**
     * @brief Freezes the critter (used by the FreezingDecorator).
     *
     * While frozen the critter moves at half speed. Freezing again restarts
     * the time but does not make it slower. Does nothing to a dead critter.
     *
     * @param seconds How long the freeze lasts.
     */
    void freeze(double seconds);

    /** @brief Tells whether the critter is on fire. */
    bool isBurning() const;
    /** @brief Tells whether the critter is frozen. */
    bool isFrozen() const;

    /**
     * @brief Lets the burn and freeze effects run for a time step.
     *
     * Deals the burn damage (once per second) and counts the effects down.
     * The game calls this every frame for each critter on the map.
     *
     * @param seconds Time elapsed since the previous call.
     */
    void updateEffects(double seconds);

    /**
     * @brief Advances the critter along the path and notifies the observers.
     *
     * Adds the speed to the position (half the speed while frozen). A dead
     * critter does not move.
     */
    void move();

    /**
     * @brief Marks the critter as having reached the exit and notifies the observers.
     *
     * Sets the position to 100.
     */
    void reachExit();

    /** @brief Prints the critter's statistics on one line. */
    void display() const;
};

#endif
