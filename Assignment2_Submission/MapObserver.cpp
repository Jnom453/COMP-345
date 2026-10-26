#include "MapObserver.h"
#include "Cell.h"
#include <optional>

MapObserver::MapObserver(Map& map)
    : map(map),
      window(sf::VideoMode({800u, 600u}),
             "Tower Defense - Map Observer"),
      cellSize(40),
      needsRedraw(true)
{
}

void MapObserver::update()
{
    needsRedraw = true;
}

void MapObserver::drawMap()
{
    if (!window.isOpen())
        return;

    if (!needsRedraw)
    return;

    window.clear(sf::Color(35, 40, 50));

    const int mapWidth = map.getWidth();
    const int mapHeight = map.getHeight();

    const float offsetX =
        (800.0f - mapWidth * cellSize) / 2.0f;

    const float offsetY =
        (600.0f - mapHeight * cellSize) / 2.0f;

    for (int row = 0; row < mapHeight; ++row)
    {
        for (int column = 0; column < mapWidth; ++column)
        {
            sf::RectangleShape cell(
                sf::Vector2f(
                    static_cast<float>(cellSize - 2),
                    static_cast<float>(cellSize - 2)
                )
            );

            cell.setPosition(
                sf::Vector2f(
                    offsetX + column * cellSize,
                    offsetY + row * cellSize
                )
            );

            const int type = map.getCellType(row, column);

            if (type == ENTRY)
                cell.setFillColor(sf::Color(70, 160, 240));
            else if (type == EXIT)
                cell.setFillColor(sf::Color(230, 75, 75));
            else if (type == PATH)
                cell.setFillColor(sf::Color(190, 145, 90));
            else
                cell.setFillColor(sf::Color(75, 165, 105));

            window.draw(cell);
        }
    }

    window.display();

    needsRedraw = false;
}

void MapObserver::run()
{
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        drawMap();
    }
}