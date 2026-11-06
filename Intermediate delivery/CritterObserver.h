
#ifndef CRITTER_OBSERVER_H
#define CRITTER_OBSERVER_H

#include "Observer.h"
#include "Critter.h"
#include <SFML/Graphics.hpp>

/**
 * @file CritterObserver.h
 * @brief SFML view of a single Critter.
 */

/**
 * @brief Window that displays one critter and redraws it when it changes.
 *
 * Shows the critter's level, hit points, strength, speed, reward, position and
 * whether it is alive. The observer is attached to the critter with
 * Critter::attach(); the critter notifies it when it takes damage, is slowed,
 * moves or reaches the exit.
 */
class CritterObserver : public Observer
{
private:
    /** @brief The critter being displayed. Not owned by the observer. */
    Critter& critter;
    /** @brief The SFML window showing the critter. */
    sf::RenderWindow window;
    /** @brief true when the critter changed since the last drawing. */
    bool needsRedraw;

    /** @brief Redraws the critter information if it changed since the last drawing. */
    void drawView();

public:
    /**
     * @brief Opens the window for a critter.
     * @param critter The critter to display. It must outlive the observer.
     */
    explicit CritterObserver(Critter& critter);

    /** @brief Called by the critter when it changes; flags the view for redrawing. */
    void update() override;

    /**
     * @brief Runs the window loop until the window is closed.
     *
     * Handles window events and redraws the critter information whenever it
     * has changed. This call blocks until the user closes the window.
     */
    void run();
};

#endif
