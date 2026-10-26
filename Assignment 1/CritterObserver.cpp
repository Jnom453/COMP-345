
#include "CritterObserver.h"
#include <optional>
#include <sstream>

CritterObserver::CritterObserver(Critter& critter)
    : critter(critter),
      window(sf::VideoMode({700u, 450u}),
             "Tower Defense - Critter Observer"),
      needsRedraw(true)
{
}

void CritterObserver::update()
{
    needsRedraw = true;
}

void CritterObserver::drawView()
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

    sf::Text title(font, "Critter Information", 28);
    title.setFillColor(sf::Color(240, 200, 90));
    title.setPosition(sf::Vector2f(30.f, 25.f));
    window.draw(title);

    std::ostringstream information;
    information << "Level: " << critter.getLevel()
                << "\nHit points: " << critter.getHitPoints()
                << "\nStrength: " << critter.getStrength()
                << "\nSpeed: " << critter.getSpeed()
                << "\nReward: " << critter.getReward()
                << "\nPosition: " << critter.getPosition()
                << "\nAlive: " << (critter.isAlive() ? "Yes" : "No");

    sf::Text details(font, information.str(), 22);
    details.setFillColor(sf::Color::White);
    details.setPosition(sf::Vector2f(30.f, 100.f));
    window.draw(details);

    window.display();
    needsRedraw = false;
}

void CritterObserver::run()
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