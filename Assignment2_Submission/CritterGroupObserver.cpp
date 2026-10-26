#include "CritterGroupObserver.h"
#include <optional>
#include <string>

CritterGroupObserver::CritterGroupObserver(CritterGroup& group)
    : group(group),
      window(sf::VideoMode({700u, 450u}), "Critter Group Observer"),
      needsRedraw(true)
{
}

void CritterGroupObserver::update()
{
    needsRedraw = true;
}

void CritterGroupObserver::drawView()
{
    if (!window.isOpen() || !needsRedraw)
        return;

    window.clear(sf::Color(35, 35, 45));

    sf::Font font;
    if (!font.openFromFile(
            "/System/Library/Fonts/Supplemental/Arial.ttf"))
        return;

    sf::Text title(font, "Critter Group", 28);
    title.setFillColor(sf::Color::White);
    title.setPosition({25.f, 20.f});
    window.draw(title);

    sf::Text count(
        font,
        "Number of critters: " + std::to_string(group.getSize()),
        20
    );
    count.setFillColor(sf::Color::Green);
    count.setPosition({25.f, 70.f});
    window.draw(count);

    float y = 120.f;

    for (int i = 0; i < group.getSize(); ++i) {
        const Critter& critter = group.getCritter(i);

        sf::CircleShape circle(12.f);
        circle.setPosition({30.f, y + 3.f});
        circle.setFillColor(sf::Color::Yellow);
        window.draw(circle);

        sf::Text info(
            font,
            "Critter " + std::to_string(i + 1) +
            " | HP: " + std::to_string(critter.getHitPoints()),
            18
        );
        info.setFillColor(sf::Color::White);
        info.setPosition({60.f, y});
        window.draw(info);

        y += 45.f;
    }

    window.display();
    needsRedraw = false;
}

void CritterGroupObserver::run()
{
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        drawView();
    }
}