
#ifndef TOWER_OBSERVER_H
#define TOWER_OBSERVER_H

#include "Observer.h"
#include "Tower.h"
#include <SFML/Graphics.hpp>

class TowerObserver : public Observer
{
private:
    Tower& tower;
    sf::RenderWindow window;
    bool needsRedraw;

    void drawView();

public:
    explicit TowerObserver(Tower& tower);
    void update() override;
    void run();
};

#endif