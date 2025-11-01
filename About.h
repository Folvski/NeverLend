#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class About {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture backButtonHoverTexture;
    sf::Sprite backButtonHoverSprite;
    sf::FloatRect backButtonArea;

    sf::SoundBuffer hoverSoundBuffer;
    sf::Sound hoverSound;

    bool isHovered;
    bool wasHovered;

    // Базовые координаты для 1920x1080
    const sf::Vector2f baseBackButtonPos = { 1556.f, 985.f };
    const sf::Vector2f baseBackButtonSize = { 308.f, 34.f };
    const sf::Vector2f baseSize = { 1920.f, 1080.f };

    void updateLayout(sf::RenderWindow& window) {
        sf::Vector2u windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / baseSize.x;
        float scaleY = static_cast<float>(windowSize.y) / baseSize.y;

        // Масштабирование фона
        backgroundSprite.setScale(scaleX, scaleY);

        // Масштабирование кнопки
        backButtonHoverSprite.setPosition(baseBackButtonPos.x * scaleX, baseBackButtonPos.y * scaleY);
        backButtonHoverSprite.setScale(scaleX, scaleY);

        // Обновление области кнопки
        backButtonArea = backButtonHoverSprite.getGlobalBounds();
    }

public:
    About() : isHovered(false), wasHovered(false) {
        // Загрузка фонового изображения
        if (!backgroundTexture.loadFromFile("images/about.png")) {
            throw std::runtime_error("Failed to load about background image");
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Загрузка текстуры для hover-эффекта
        if (!backButtonHoverTexture.loadFromFile("images/back_button_hover.png")) {
            throw std::runtime_error("Failed to load back button hover texture");
        }
        backButtonHoverSprite.setTexture(backButtonHoverTexture);
        backButtonHoverSprite.setPosition(baseBackButtonPos);

        // Загрузка звука наведения
        if (!hoverSoundBuffer.loadFromFile("hover_sound.ogg")) {
            throw std::runtime_error("Failed to load hover sound");
        }
        hoverSound.setBuffer(hoverSoundBuffer);
    }

    bool run(sf::RenderWindow& window) {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return false;
                }

                // Обработка Escape для возврата
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    return true;
                }

                // Обработка движения мыши
                if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    isHovered = backButtonArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                }

                // Обработка нажатия кнопки
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (backButtonArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            return true;
                        }
                    }
                }
            }
            updateLayout(window);

            // Воспроизведение звука при наведении
            if (isHovered && !wasHovered) {
                hoverSound.play();
            }
            wasHovered = isHovered;

            // Отрисовка
            window.clear();
            window.draw(backgroundSprite);

            // Рисуем кнопку только при наведении
            if (isHovered) {
                window.draw(backButtonHoverSprite);
            }

            window.display();
        }
        return false;
    }
};