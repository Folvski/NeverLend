#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Choice {
private:
    // Фон
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Кнопки уровней
    std::vector<sf::RectangleShape> levelButtons;
    std::vector<sf::FloatRect> levelButtonAreas;
    std::vector<sf::Texture> levelButtonHoverTextures;

    // Кнопка возврата в меню
    sf::RectangleShape backButton;
    sf::FloatRect backButtonArea;
    sf::Texture backButtonHoverTexture;

    // Звук при наведении
    sf::SoundBuffer hoverSoundBuffer;
    sf::Sound hoverSound;
    bool wasHovered;

public:
    Choice() : wasHovered(false) {
        // Загрузка фона
        if (!backgroundTexture.loadFromFile("images/level_choice_bg.png")) {
            throw std::runtime_error("Failed to load level choice background");
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Инициализация кнопок (5 кнопок для 5 уровней)
        levelButtons.resize(5);
        levelButtonAreas.resize(5);
        levelButtonHoverTextures.resize(5);

        // Загрузка текстур для кнопок при наведении
        for (int i = 0; i < 5; ++i) {
            if (!levelButtonHoverTextures[i].loadFromFile("images/level" + std::to_string(i + 1) + "_hover.png")) {
                throw std::runtime_error("Failed to load level button hover texture " + std::to_string(i + 1));
            }
        }

        // Установка размеров и позиций кнопок согласно заданным отступам
        // Размер всех кнопок: 274x528 пикселей
        const sf::Vector2f buttonSize(274.f, 528.f);

        // Позиции кнопок (x, y) = (left, top)
        levelButtons[0].setSize(buttonSize);
        levelButtons[0].setPosition(181.f, 485.f);  // Слева 181, сверху 485

        levelButtons[1].setSize(buttonSize);
        levelButtons[1].setPosition(502.f, 485.f);  // Слева 502, сверху 485

        levelButtons[2].setSize(buttonSize);
        levelButtons[2].setPosition(823.f, 485.f);  // Слева 823, сверху 485

        levelButtons[3].setSize(buttonSize);
        levelButtons[3].setPosition(1144.f, 485.f); // Слева 1144, сверху 485

        levelButtons[4].setSize(buttonSize);
        levelButtons[4].setPosition(1465.f, 485.f); // Слева 1465, сверху 485

        // Сохранение областей для проверки наведения/клика
        for (int i = 0; i < 5; ++i) {
            levelButtonAreas[i] = levelButtons[i].getGlobalBounds();
        }

        // Настройка прозрачности для всех кнопок уровней
        for (auto& button : levelButtons) {
            button.setFillColor(sf::Color::Transparent);
            button.setOutlineThickness(0);
            button.setTexture(nullptr);
        }

        // Инициализация кнопки возврата в меню (95x95)
        // Позиция: слева 58, сверху 64
        if (!backButtonHoverTexture.loadFromFile("images/back_hover.png")) {
            throw std::runtime_error("Failed to load back button hover texture");
        }

        backButton.setSize(sf::Vector2f(95.f, 95.f));
        backButton.setPosition(58.f, 64.f); // Левый верхний угол
        backButton.setFillColor(sf::Color::Transparent);
        backButton.setOutlineThickness(0);
        backButton.setTexture(nullptr);
        backButtonArea = backButton.getGlobalBounds();

        // Загрузка звука наведения
        if (!hoverSoundBuffer.loadFromFile("hover_sound.ogg")) {
            throw std::runtime_error("Failed to load hover sound");
        }
        hoverSound.setBuffer(hoverSoundBuffer);
    }

    int run(sf::RenderWindow& window) {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0; // Выход из игры
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        return 0; // Возврат в меню
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                        // Проверка нажатия на кнопки уровней
                        for (int i = 0; i < 5; ++i) {
                            if (levelButtonAreas[i].contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                                return i + 1; // Возвращаем номер уровня (1-5)
                            }
                        }

                        // Проверка нажатия на кнопку возврата
                        if (backButtonArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                            return 0; // Возврат в меню
                        }
                    }
                }
            }

            // Обработка наведения на кнопки
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            bool isHovered = false;

            // Кнопки уровней
            for (int i = 0; i < 5; ++i) {
                if (levelButtonAreas[i].contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    levelButtons[i].setTexture(&levelButtonHoverTextures[i]);
                    levelButtons[i].setFillColor(sf::Color::White);
                    isHovered = true;
                }
                else {
                    levelButtons[i].setTexture(nullptr);
                    levelButtons[i].setFillColor(sf::Color::Transparent);
                }
            }

            // Кнопка возврата
            if (backButtonArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                backButton.setTexture(&backButtonHoverTexture);
                backButton.setFillColor(sf::Color::White);
                isHovered = true;
            }
            else {
                backButton.setTexture(nullptr);
                backButton.setFillColor(sf::Color::Transparent);
            }

            // Воспроизведение звука при наведении
            if (isHovered && !wasHovered) {
                hoverSound.play();
            }
            wasHovered = isHovered;

            // Отрисовка
            window.clear();
            window.draw(backgroundSprite);

            // Рисуем кнопки уровней
            for (const auto& button : levelButtons) {
                window.draw(button);
            }

            // Рисуем кнопку возврата
            window.draw(backButton);

            window.display();
        }
        return 0;
    }
};