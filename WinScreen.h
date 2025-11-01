#pragma once
#pragma once
#include <SFML/Graphics.hpp>

class WinScreen {
private:
    sf::RenderWindow& window;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Clock displayClock;
    float displayTime;

public:
    WinScreen(sf::RenderWindow& win) : window(win), displayTime(5.0f) {
        if (!texture.loadFromFile("images/win_screen.png")) {
            // Обработка ошибки загрузки
        }
        sprite.setTexture(texture);
        sprite.setPosition(0, 0);
    }

    void show() {
        displayClock.restart();
        window.setView(window.getDefaultView());

        while (window.isOpen() && displayClock.getElapsedTime().asSeconds() < displayTime) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
};
