
#ifndef CRITTER_OBSERVER_H
#define CRITTER_OBSERVER_H

#include "Observer.h"
#include "Critter.h"
#include <SFML/Graphics.hpp>

class CritterObserver : public Observer
{
private:
    Critter& critter;
    sf::RenderWindow window;
    bool needsRedraw;

    void drawView();

public:
    explicit CritterObserver(Critter& critter);
    void update() override;
    void run();
};

#endif