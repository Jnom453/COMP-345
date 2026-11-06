#include <optional>
#include <string>

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "FontLoader.h"

/**
 * @file game_main.cpp
 * @brief The SFML window of the playable game.
 *
 * Draws the Game (map, towers, critters, coins, lives) and turns mouse clicks
 * and key presses into calls on Game. All the rules live in Game.cpp.
 *
 * Controls:
 * - 1, 2, 3: choose the tower type to build (direct, area, slow)
 * - left click on a green cell: buy and place the chosen tower
 * - left click on a tower: select it
 * - U: upgrade the selected tower, S: sell it
 * - SPACE: start the next wave
 * - R: restart, ESC: quit
 */

namespace
{
const float CELL = 48.f;
const float MAP_X = 32.f;
const float MAP_Y = 100.f;
const unsigned WINDOW_W = 640;
const unsigned WINDOW_H = 700;

sf::Color towerColor(int type)
{
    if (type == Game::AREA_DAMAGE) return sf::Color(160, 90, 220);
    if (type == Game::SLOW)        return sf::Color(60, 190, 220);
    return sf::Color(240, 150, 40);
}

void drawText(sf::RenderWindow& window, const sf::Font& font, bool hasFont,
              const std::string& str, float x, float y, unsigned size,
              sf::Color color = sf::Color::White)
{
    if (!hasFont)
        return;
    sf::Text text(font, str, size);
    text.setFillColor(color);
    text.setPosition({x, y});
    window.draw(text);
}

void drawLine(sf::RenderWindow& window, float x1, float y1, float x2, float y2,
              sf::Color color)
{
    sf::Vertex line[2];
    line[0].position = {x1, y1};
    line[0].color = color;
    line[1].position = {x2, y2};
    line[1].color = color;
    window.draw(line, 2, sf::PrimitiveType::Lines);
}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({WINDOW_W, WINDOW_H}), "Tower Defense");
    window.setFramerateLimit(60);

    sf::Font font;
    const bool hasFont = loadSystemFont(font);

    Game game;
    int selectedType = Game::DIRECT_DAMAGE;
    int selectedRow = -1;
    int selectedColumn = -1;
    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                switch (key->code)
                {
                case sf::Keyboard::Key::Num1: selectedType = Game::DIRECT_DAMAGE; break;
                case sf::Keyboard::Key::Num2: selectedType = Game::AREA_DAMAGE; break;
                case sf::Keyboard::Key::Num3: selectedType = Game::SLOW; break;
                case sf::Keyboard::Key::Space: game.startWave(); break;
                case sf::Keyboard::Key::U: game.upgradeTower(selectedRow, selectedColumn); break;
                case sf::Keyboard::Key::S:
                    if (game.sellTower(selectedRow, selectedColumn))
                    {
                        selectedRow = -1;
                        selectedColumn = -1;
                    }
                    break;
                case sf::Keyboard::Key::R:
                    game.reset();
                    selectedRow = -1;
                    selectedColumn = -1;
                    break;
                case sf::Keyboard::Key::Escape: window.close(); break;
                default: break;
                }
            }
            else if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (click->button == sf::Mouse::Button::Left)
                {
                    float fx = (click->position.x - MAP_X) / CELL;
                    float fy = (click->position.y - MAP_Y) / CELL;
                    int column = static_cast<int>(fx);
                    int row = static_cast<int>(fy);
                    bool inside = fx >= 0 && fy >= 0 &&
                                  game.getMap().isInside(row, column);
                    if (inside)
                    {
                        if (game.findTower(row, column) >= 0)
                        {
                            selectedRow = row;
                            selectedColumn = column;
                        }
                        else if (game.placeTower(row, column, selectedType))
                        {
                            selectedRow = row;
                            selectedColumn = column;
                        }
                    }
                }
            }
        }

        game.update(clock.restart().asSeconds());

        // ---- drawing ----
        window.clear(sf::Color(35, 40, 50));

        const Map& map = game.getMap();
        for (int r = 0; r < map.getHeight(); ++r)
        {
            for (int c = 0; c < map.getWidth(); ++c)
            {
                sf::RectangleShape cell({CELL - 2.f, CELL - 2.f});
                cell.setPosition({MAP_X + c * CELL, MAP_Y + r * CELL});
                int type = map.getCellType(r, c);
                if (type == ENTRY)      cell.setFillColor(sf::Color(70, 160, 240));
                else if (type == EXIT)  cell.setFillColor(sf::Color(230, 75, 75));
                else if (type == PATH)  cell.setFillColor(sf::Color(190, 145, 90));
                else                    cell.setFillColor(sf::Color(75, 165, 105));
                window.draw(cell);
            }
        }

        // Range of the selected tower.
        int selectedIndex = game.findTower(selectedRow, selectedColumn);
        if (selectedIndex >= 0)
        {
            const Game::PlacedTower& t = game.getPlacedTower(selectedIndex);
            float radius = static_cast<float>(t.tower->getRange()) * CELL;
            sf::CircleShape range(radius);
            range.setOrigin({radius, radius});
            range.setPosition({MAP_X + (t.column + 0.5f) * CELL - 1.f,
                               MAP_Y + (t.row + 0.5f) * CELL - 1.f});
            range.setFillColor(sf::Color(255, 255, 255, 40));
            range.setOutlineColor(sf::Color(255, 255, 255, 160));
            range.setOutlineThickness(2.f);
            window.draw(range);
        }

        // Cover the top and bottom bars so the range circle never draws over the text.
        sf::RectangleShape topBar({static_cast<float>(WINDOW_W), MAP_Y - 4.f});
        topBar.setFillColor(sf::Color(35, 40, 50));
        window.draw(topBar);
        float mapBottom = MAP_Y + map.getHeight() * CELL;
        sf::RectangleShape bottomBar({static_cast<float>(WINDOW_W),
                                      static_cast<float>(WINDOW_H) - mapBottom});
        bottomBar.setPosition({0.f, mapBottom});
        bottomBar.setFillColor(sf::Color(35, 40, 50));
        window.draw(bottomBar);

        // Towers.
        for (int i = 0; i < game.getTowerCount(); ++i)
        {
            const Game::PlacedTower& t = game.getPlacedTower(i);
            sf::RectangleShape body({CELL - 14.f, CELL - 14.f});
            body.setPosition({MAP_X + t.column * CELL + 6.f, MAP_Y + t.row * CELL + 6.f});
            body.setFillColor(towerColor(t.type));
            if (i == selectedIndex)
            {
                body.setOutlineColor(sf::Color::White);
                body.setOutlineThickness(3.f);
            }
            window.draw(body);
            drawText(window, font, hasFont, std::to_string(t.tower->getLevel()),
                     MAP_X + t.column * CELL + 17.f, MAP_Y + t.row * CELL + 6.f, 24,
                     sf::Color::Black);
        }

        // Shots.
        for (std::size_t i = 0; i < game.getShots().size(); ++i)
        {
            const Game::Shot& s = game.getShots()[i];
            drawLine(window,
                     MAP_X + (static_cast<float>(s.fromColumn) + 0.5f) * CELL - 1.f,
                     MAP_Y + (static_cast<float>(s.fromRow) + 0.5f) * CELL - 1.f,
                     MAP_X + (static_cast<float>(s.toColumn) + 0.5f) * CELL - 1.f,
                     MAP_Y + (static_cast<float>(s.toRow) + 0.5f) * CELL - 1.f,
                     sf::Color(255, 255, 120));
        }

        // Critters with a hit point bar.
        for (int i = 0; i < game.getCritterCount(); ++i)
        {
            if (!game.isCritterOnMap(i))
                continue;
            double r, c;
            game.getCritterLocation(i, r, c);
            float cx = MAP_X + (static_cast<float>(c) + 0.5f) * CELL - 1.f;
            float cy = MAP_Y + (static_cast<float>(r) + 0.5f) * CELL - 1.f;

            sf::CircleShape dot(11.f);
            dot.setOrigin({11.f, 11.f});
            dot.setPosition({cx, cy});
            dot.setFillColor(sf::Color(225, 50, 50));
            dot.setOutlineColor(sf::Color::Black);
            dot.setOutlineThickness(2.f);
            window.draw(dot);

            float fraction = static_cast<float>(game.getCritter(i).getHitPoints()) /
                             static_cast<float>(game.getCritterMaxHitPoints(i));
            sf::RectangleShape back({26.f, 4.f});
            back.setPosition({cx - 13.f, cy - 20.f});
            back.setFillColor(sf::Color(60, 0, 0));
            window.draw(back);
            sf::RectangleShape bar({26.f * fraction, 4.f});
            bar.setPosition({cx - 13.f, cy - 20.f});
            bar.setFillColor(sf::Color(60, 230, 60));
            window.draw(bar);
        }

        // HUD.
        std::string waveText = "Wave " + std::to_string(game.getWave()) + "/" +
                               std::to_string(Game::MAX_WAVES);
        drawText(window, font, hasFont, "Coins: " + std::to_string(game.getCoins()),
                 20.f, 12.f, 24, sf::Color(250, 210, 80));
        drawText(window, font, hasFont, "Lives: " + std::to_string(game.getLives()),
                 220.f, 12.f, 24, sf::Color(240, 110, 110));
        drawText(window, font, hasFont, waveText, 420.f, 12.f, 24);
        drawText(window, font, hasFont, game.getMessage(), 20.f, 52.f, 17,
                 sf::Color(200, 220, 255));

        float y = MAP_Y + map.getHeight() * CELL + 10.f;
        std::string build = "Build: [1] " + Game::towerName(Game::DIRECT_DAMAGE) + " $" +
                            std::to_string(Game::towerCost(Game::DIRECT_DAMAGE)) +
                            "   [2] " + Game::towerName(Game::AREA_DAMAGE) + " $" +
                            std::to_string(Game::towerCost(Game::AREA_DAMAGE)) +
                            "   [3] " + Game::towerName(Game::SLOW) + " $" +
                            std::to_string(Game::towerCost(Game::SLOW));
        drawText(window, font, hasFont, build, 20.f, y, 14);
        drawText(window, font, hasFont,
                 "Selected to build: " + Game::towerName(selectedType), 20.f, y + 22.f, 16,
                 towerColor(selectedType));

        if (selectedIndex >= 0)
        {
            const Tower& t = *game.getPlacedTower(selectedIndex).tower;
            drawText(window, font, hasFont,
                     t.getName() + "  Lv " + std::to_string(t.getLevel()) +
                     "  Power " + std::to_string(t.getPower()) +
                     "  Range " + std::to_string(static_cast<int>(t.getRange())) +
                     "  [U] upgrade $" + std::to_string(t.getBuyingCost() * t.getLevel()) +
                     "  [S] sell $" + std::to_string(t.getRefundValue()),
                     20.f, y + 46.f, 14);
        }
        drawText(window, font, hasFont,
                 "Click a green cell to build | SPACE start wave | R restart | ESC quit",
                 20.f, y + 70.f, 14, sf::Color(170, 170, 170));

        window.display();
    }
    return 0;
}