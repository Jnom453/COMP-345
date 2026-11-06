#ifndef CRITTER_H
#define CRITTER_H

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

public:
    /**
     * @brief Creates a living critter at the start of the path.
     * @param level     Difficulty level.
     * @param hitPoints Starting hit points.
     * @param strength  Coins stolen if it reaches the exit.
     * @param speed     Distance travelled per move.
     * @param reward    Coins earned by killing it.
     */
    Critter(
        int level,
        int hitPoints,
        int strength,
        double speed,
        int reward
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
     * @brief Advances the critter along the path and notifies the observers.
     *
     * Adds the speed to the position. A dead critter does not move.
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
