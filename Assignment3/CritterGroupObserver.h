#ifndef CRITTER_GROUP_OBSERVER_H
#define CRITTER_GROUP_OBSERVER_H

#include "Observer.h"
#include "CritterGroup.h"
#include <SFML/Graphics.hpp>

/**
 * @file CritterGroupObserver.h
 * @brief SFML view of a CritterGroup.
 */

/**
 * @brief Window that lists the critters of a group and redraws the list when
 *        critters are added or removed.
 *
 * Shows the number of critters and, for each one, its number and hit points.
 * The observer is attached to the group with CritterGroup::attach().
 */
class CritterGroupObserver : public Observer {
private:
    /** @brief The group being displayed. Not owned by the observer. */
    CritterGroup& group;
    /** @brief The SFML window showing the group. */
    sf::RenderWindow window;
    /** @brief true when the group changed since the last drawing. */
    bool needsRedraw;

    /** @brief Redraws the list of critters if the group changed since the last drawing. */
    void drawView();

public:
    /**
     * @brief Opens the window for a group.
     * @param group The group to display. It must outlive the observer.
     */
    explicit CritterGroupObserver(CritterGroup& group);

    /** @brief Called by the group when it changes; flags the view for redrawing. */
    void update() override;

    /**
     * @brief Runs the window loop until the window is closed.
     *
     * Handles window events and redraws the list whenever the group has
     * changed. This call blocks until the user closes the window.
     */
    void run();
};

#endif
