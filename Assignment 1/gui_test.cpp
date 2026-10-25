#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}), 
        "Tower Defense - GUI Test"
        );

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
            }
                window.clear(sf::Color(35, 40, 50));
                window.display();
            }

            return 0;
}