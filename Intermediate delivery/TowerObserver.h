
#ifndef TOWER_OBSERVER_H
#define TOWER_OBSERVER_H

#include "Observer.h"
#include "Tower.h"
#include <SFML/Graphics.hpp>

/**
 * @file TowerObserver.h
 * @brief SFML view of a Tower.
 */

/**
 * @brief Window that displays a tower's characteristics and redraws them when
 *        the tower changes.
 *
 * Shows the tower's name, level, power, range and rate of fire. The observer
 * is attached to the tower with Tower::attach(); the tower notifies it after
 * each upgrade.
 *
 * The observer reads the tower's current values each time it redraws, so
 * update() does not need to carry any data.
 */
class TowerObserver : public Observer
{
private:
    /** @brief The tower being displayed. Not owned by the observer. */
    Tower& tower;
    /** @brief The SFML window showing the tower. */
    sf::RenderWindow window;
    /** @brief true when the tower changed since the last drawing. */
    bool needsRedraw;

    /**
     * @brief Redraws the tower information if it changed since the last drawing.
     *
     * Clears the window, draws the title and the tower's current values, then
     * displays the frame and clears the needsRedraw flag.
     */
    void drawView();

public:
    /**
     * @brief Opens the window for a tower.
     * @param tower The tower to display. It must outlive the observer.
     */
    explicit TowerObserver(Tower& tower);

    /**
     * @brief Called by the tower when it changes.
     *
     * Only flags the view as out of date. The redraw happens on the next
     * pass of the window loop in run().
     */
    void update() override;

    /**
     * @brief Runs the window loop until the window is closed.
     *
     * Handles window events and redraws the tower information whenever it
     * has changed. This call blocks until the user closes the window.
     */
    void run();
};

#endif
