#include "TargetingStrategy.h"
#include "Critter.h"

// All four strategies have the same shape: look at every living candidate and
// keep the best one according to a different measure. The first candidate wins
// a tie because only a strictly better one replaces the current best.

int NearestToTowerStrategy::chooseTarget(
    const std::vector<TargetCandidate>& candidates) const
{
    int best = -1;
    for (std::size_t i = 0; i < candidates.size(); ++i)
    {
        if (candidates[i].critter == NULL || !candidates[i].critter->isAlive())
            continue;
        if (best < 0 || candidates[i].distanceToTower < candidates[best].distanceToTower)
            best = static_cast<int>(i);
    }
    return best;
}
StrategyKind NearestToTowerStrategy::getKind() const { return NEAREST_TO_TOWER; }
std::string NearestToTowerStrategy::getName() const { return "Nearest to tower"; }

// The position of a critter goes from 0 (entry) to 100 (exit), so the largest
// position is the critter that is closest to the exit.
int NearestToExitStrategy::chooseTarget(
    const std::vector<TargetCandidate>& candidates) const
{
    int best = -1;
    for (std::size_t i = 0; i < candidates.size(); ++i)
    {
        if (candidates[i].critter == NULL || !candidates[i].critter->isAlive())
            continue;
        if (best < 0 ||
            candidates[i].critter->getPosition() > candidates[best].critter->getPosition())
            best = static_cast<int>(i);
    }
    return best;
}
StrategyKind NearestToExitStrategy::getKind() const { return NEAREST_TO_EXIT; }
std::string NearestToExitStrategy::getName() const { return "Nearest to exit"; }

int StrongestStrategy::chooseTarget(
    const std::vector<TargetCandidate>& candidates) const
{
    int best = -1;
    for (std::size_t i = 0; i < candidates.size(); ++i)
    {
        if (candidates[i].critter == NULL || !candidates[i].critter->isAlive())
            continue;
        if (best < 0 ||
            candidates[i].critter->getHitPoints() > candidates[best].critter->getHitPoints())
            best = static_cast<int>(i);
    }
    return best;
}
StrategyKind StrongestStrategy::getKind() const { return STRONGEST; }
std::string StrongestStrategy::getName() const { return "Strongest"; }

int WeakestStrategy::chooseTarget(
    const std::vector<TargetCandidate>& candidates) const
{
    int best = -1;
    for (std::size_t i = 0; i < candidates.size(); ++i)
    {
        if (candidates[i].critter == NULL || !candidates[i].critter->isAlive())
            continue;
        if (best < 0 ||
            candidates[i].critter->getHitPoints() < candidates[best].critter->getHitPoints())
            best = static_cast<int>(i);
    }
    return best;
}
StrategyKind WeakestStrategy::getKind() const { return WEAKEST; }
std::string WeakestStrategy::getName() const { return "Weakest"; }

std::shared_ptr<TargetingStrategy> createStrategy(int kind)
{
    switch (kind)
    {
    case NEAREST_TO_TOWER: return std::make_shared<NearestToTowerStrategy>();
    case STRONGEST:        return std::make_shared<StrongestStrategy>();
    case WEAKEST:          return std::make_shared<WeakestStrategy>();
    default:               return std::make_shared<NearestToExitStrategy>();
    }
}
