#ifndef TARGETING_STRATEGY_H
#define TARGETING_STRATEGY_H

#include <memory>
#include <string>
#include <vector>

class Critter;

/**
 * @file TargetingStrategy.h
 * @brief Strategy pattern: the different ways a tower can choose its target.
 */

/**
 * @brief A critter a tower could shoot, with its distance from the tower.
 *
 * The game builds one TargetCandidate for every critter that is inside a
 * tower's range and gives the list to the tower's TargetingStrategy.
 */
struct TargetCandidate
{
    Critter* critter;        /**< The critter that is in range. */
    double distanceToTower;  /**< Distance from the tower, in cells. */
};

/**
 * @brief The kinds of targeting strategy a tower can use.
 */
enum StrategyKind
{
    NEAREST_TO_TOWER = 0, /**< Shoot the critter closest to the tower. */
    NEAREST_TO_EXIT = 1,  /**< Shoot the critter closest to the exit (furthest along the path). */
    STRONGEST = 2,        /**< Shoot the critter with the most hit points. */
    WEAKEST = 3           /**< Shoot the critter with the fewest hit points. */
};

/** @brief Number of different StrategyKind values. */
const int STRATEGY_COUNT = 4;

/**
 * @brief Strategy interface: chooses one target among the critters in range.
 *
 * This is the Strategy design pattern. A Tower holds a pointer to a
 * TargetingStrategy and asks it which critter to shoot. Because the tower only
 * knows this interface, the strategy can be replaced while the game is
 * running (Tower::setStrategy()) without changing the tower or the game loop.
 *
 * Game rule: when several critters are in range, the tower must pick exactly
 * one of them as its target.
 */
class TargetingStrategy
{
public:
    virtual ~TargetingStrategy() {}

    /**
     * @brief Chooses a target among the candidates.
     *
     * Dead critters are ignored. When two candidates are equally good, the
     * first one in the list wins.
     *
     * @param candidates The critters in range of the tower.
     * @return The index of the chosen candidate, or -1 if there is none.
     */
    virtual int chooseTarget(const std::vector<TargetCandidate>& candidates) const = 0;

    /** @brief Returns the StrategyKind of this strategy. */
    virtual StrategyKind getKind() const = 0;

    /** @brief Returns the name of the strategy, for display. */
    virtual std::string getName() const = 0;
};

/** @brief Strategy: the critter closest to the tower. */
class NearestToTowerStrategy : public TargetingStrategy
{
public:
    int chooseTarget(const std::vector<TargetCandidate>& candidates) const;
    StrategyKind getKind() const;
    std::string getName() const;
};

/** @brief Strategy: the critter closest to the exit, meaning the furthest along the path. */
class NearestToExitStrategy : public TargetingStrategy
{
public:
    int chooseTarget(const std::vector<TargetCandidate>& candidates) const;
    StrategyKind getKind() const;
    std::string getName() const;
};

/** @brief Strategy: the strongest critter, meaning the one with the most hit points left. */
class StrongestStrategy : public TargetingStrategy
{
public:
    int chooseTarget(const std::vector<TargetCandidate>& candidates) const;
    StrategyKind getKind() const;
    std::string getName() const;
};

/** @brief Strategy: the weakest critter, meaning the one with the fewest hit points left. */
class WeakestStrategy : public TargetingStrategy
{
public:
    int chooseTarget(const std::vector<TargetCandidate>& candidates) const;
    StrategyKind getKind() const;
    std::string getName() const;
};

/**
 * @brief Creates the strategy of a given kind.
 * @param kind A StrategyKind value. Any other value gives NearestToExitStrategy.
 */
std::shared_ptr<TargetingStrategy> createStrategy(int kind);

#endif
