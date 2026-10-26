#ifndef CRITTER_GROUP_OBSERVER_H
#define CRITTER_GROUP_OBSERVER_H

#include "Observer.h"
#include "CritterGroup.h"
#include <SFML/Graphics.hpp>

class CritterGroupObserver : public Observer {
private:
    CritterGroup& group;
    sf::RenderWindow window;
    bool needsRedraw;

    void drawView();

public:
    explicit CritterGroupObserver(CritterGroup& group);
    void update() override;
    void run();
};

#endif