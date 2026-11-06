#ifndef MAP_OBSERVER_H
#define MAP_OBSERVER_H

#include "Observer.h"
#include "Map.h"
#include <SFML/Graphics.hpp>

/**
 * @file MapObserver.h
 * @brief SFML view of a Map.
 */

/**
 * @brief Window that displays a Map and redraws it when the map changes.
 *
 * Each cell is drawn as a square: blue for the entry, red for the exit,
 * brown for path cells and green for scenery cells. The observer is
 * attached to the Map with Map::attach().
 */
class MapObserver : public Observer
{
private:
    /** @brief The map being displayed. Not owned by the observer. */
    Map& map;
    /** @brief The SFML window showing the map. */
    sf::RenderWindow window;

    /** @brief Side of one drawn cell, in pixels. */
    const int cellSize;

    /** @brief true when the map changed since the last drawing. */
    bool needsRedraw;

    /** @brief Redraws the whole map if it changed since the last drawing. */
    void drawMap();

public:
    /**
     * @brief Opens the window for a map.
     * @param map The map to display. It must outlive the observer.
     */
    explicit MapObserver(Map& map);

    /** @brief Called by the map when it changes; flags the view for redrawing. */
    void update() override;

    /**
     * @brief Runs the window loop until the window is closed.
     *
     * Handles window events and redraws the map whenever it has changed.
     * This call blocks until the user closes the window.
     */
    void run();
};

#endif
