
#include "TowerObserver.h"
#include <optional>
#include <sstream>

TowerObserver::TowerObserver(Tower& tower)
    : tower(tower),
      window(sf::VideoMode({700u, 400u}),
             "Tower Defense - Tower Observer"),
      needsRedraw(true)
{
}

void TowerObserver::update()
{
    needsRedraw = true;
}

void TowerObserver::drawView()
{
    if (!window.isOpen() || !needsRedraw)
        return;

    window.clear(sf::Color(35, 40, 50));

    sf::Font font;

    if (!font.openFromFile(
            "/System/Library/Fonts/Supplemental/Arial.ttf"))
    {
        window.display();
        needsRedraw = false;
        return;
    }

    sf::Text title(font, "Tower Information", 28);
    title.setFillColor(sf::Color(240, 200, 90));
    title.setPosition(sf::Vector2f(30.f, 25.f));
    window.draw(title);

    std::ostringstream information;
    information << "Name: " << tower.getName()
                << "\nLevel: " << tower.getLevel()
                << "\nPower: " << tower.getPower()
                << "\nRange: " << tower.getRange()
                << "\nRate of fire: " << tower.getRateOfFire();

    sf::Text details(font, information.str(), 22);
    details.setFillColor(sf::Color::White);
    details.setPosition(sf::Vector2f(30.f, 100.f));
    window.draw(details);

    window.display();
    needsRedraw = false;
}

void TowerObserver::run()
{
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        drawView();
    }
}