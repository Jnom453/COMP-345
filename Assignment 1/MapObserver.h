#ifndef MAPOBSERVER_H
#define MAPOBSERVER_H

#include "Observer.h"
#include "Map.h"
#include <_SFML/Graphics.hpp>

class MapObserver : public Observer 
{
    private:
        Map& map;
        sf::RenderWindow window;

        const int cellSize;
        void drawMap();

    public:
        explicit MabObserver(Map& map);
        void update() override;
        void run();
};

#endif