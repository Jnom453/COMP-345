#ifndef MAP_OBSERVER_H
#define MAP_OBSERVER_H

#include "Observer.h"
#include "Map.h"
#include <SFML/Graphics.hpp>

class MapObserver : public Observer
{
private:
    Map& map;
    sf::RenderWindow window;

    const int cellSize;

    void drawMap();

public:
    explicit MapObserver(Map& map);
    void update() override;
    void run();
};

#endif
