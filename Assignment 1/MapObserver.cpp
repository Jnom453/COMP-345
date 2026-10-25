#include "MapObserver.h"
#include "Cell.h"

MapObserver:: MapObserver(Map& mao)
    : map(map),
        window(sf::VideoMode({800u, 600u}), "Tower Defense - Map Obserrver"),
        cellSize(40)

{
}

void MapObserver::update()
{
    drawMap();
}

void MapObserver::drawMap()
{
    if (!window.isOpen())
        return;

    window.clear(sf::Color::(35, 40, 50));

    int mapWidth = map.getWidth();
    int mapHeight = map.getHeight();

    float offsetX = 
        (800.0f - mapWidth * cellSize) / 2.0f;
    float offsetY = 
        (600.0f - mapHeight * cellSize) / 2.0f;

    for ( int row = 0; row < mapHeight; ++row)
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

            int type = map.getCellType(row, column);

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