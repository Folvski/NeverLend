#pragma once
class About;//  вместо включения заголовка
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Menu {
private:
    // Анимация меню
    std::vector<sf::Texture> menuFrames;
    sf::Sprite menuSprite;
    int currentMenuFrame;
    float menuFrameTime;
    float menuElapsedTime;
    sf::Clock menuClock;
    bool animationFinished;

    // Кнопки
    sf::RectangleShape playButton;
    sf::RectangleShape newButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape customButton; // Новая кнопка

    // Текстуры для hover-состояния
    sf::Texture playButtonHoverTexture;
    sf::Texture newButtonHoverTexture;
    sf::Texture exitButtonHoverTexture;
    sf::Texture customButtonTexture; // Текстура новой кнопки

    // Области кнопок
    sf::FloatRect playButtonArea;
    sf::FloatRect newButtonArea;
    sf::FloatRect exitButtonArea;
    sf::FloatRect customButtonArea; // Область новой кнопки

    // Звук при наведении
    sf::SoundBuffer hoverSoundBuffer;
    sf::Sound hoverSound;
    bool wasHovered;
    bool customButtonInitialized; // Флаг инициализации кастомной кнопки

public:
    Menu() : currentMenuFrame(0), menuFrameTime(0.7f), menuElapsedTime(0.0f),
        animationFinished(false), wasHovered(false), customButtonInitialized(false) {

        // Загрузка кадров анимации меню
        menuFrames.resize(9);
        for (int i = 0; i < 9; ++i) {
            if (!menuFrames[i].loadFromFile("images/m" + std::to_string(i + 1) + ".png")) {
                throw std::runtime_error("Failed to load menu frame " + std::to_string(i + 1));
            }
        }
        menuSprite.setTexture(menuFrames[0]);

        // Загрузка hover-текстур для кнопок
        if (!playButtonHoverTexture.loadFromFile("images/play_hover.png") ||
            !newButtonHoverTexture.loadFromFile("images/new_hover.png") ||
            !exitButtonHoverTexture.loadFromFile("images/exit_hover.png") ||
            !customButtonTexture.loadFromFile("images/custom_button.png")) { // Загрузка текстуры новой кнопки
            throw std::runtime_error("Failed to load button hover textures");
        }

        // Инициализация кнопки Play (313x79)
        playButton.setSize(sf::Vector2f(313.f, 79.f));
        playButton.setPosition(804.f, 687.f);
        playButton.setFillColor(sf::Color::Transparent);
        playButton.setOutlineThickness(0);
        playButton.setTexture(nullptr);
        playButtonArea = playButton.getGlobalBounds();

        // Инициализация новой кнопки (351x57)
        newButton.setSize(sf::Vector2f(351.f, 57.f));
        newButton.setPosition(785.f, 787.f);
        newButton.setFillColor(sf::Color::Transparent);
        newButton.setOutlineThickness(0);
        newButton.setTexture(nullptr);
        newButtonArea = newButton.getGlobalBounds();

        // Инициализация кнопки Exit (317x57)
        exitButton.setSize(sf::Vector2f(317.f, 57.f));
        exitButton.setPosition(801.f, 865.f);
        exitButton.setFillColor(sf::Color::Transparent);
        exitButton.setOutlineThickness(0);
        exitButton.setTexture(nullptr);
        exitButtonArea = exitButton.getGlobalBounds();

        // Инициализация новой кастомной кнопки (139x47)
        customButton.setSize(sf::Vector2f(139.f, 47.f));
        customButton.setTexture(&customButtonTexture); // Устанавливаем текстуру
        customButton.setFillColor(sf::Color::White);
        // Позиция будет установлена в run() когда будет доступно окно

        // Загрузка звука наведения
        if (!hoverSoundBuffer.loadFromFile("hover_sound.ogg")) {
            throw std::runtime_error("Failed to load hover sound");
        }
        hoverSound.setBuffer(hoverSoundBuffer);
    }

    void initCustomButton(const sf::RenderWindow& window) {
        if (!customButtonInitialized) {
            customButton.setPosition(0.f, window.getSize().y - 47.f); // Нижний левый угол
            customButtonArea = customButton.getGlobalBounds();
            customButtonInitialized = true;
        }
    }

    int run(sf::RenderWindow& window) {
        initCustomButton(window); // Инициализируем позицию кнопки

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 3; // Exit
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (playButtonArea.contains(mousePos.x, mousePos.y)) {
                            return 1; // Play
                        }
                        else if (newButtonArea.contains(mousePos.x, mousePos.y)) {
                            return 2; // About
                        }
                        else if (exitButtonArea.contains(mousePos.x, mousePos.y)) {
                            return 3; // Exit
                        }
                        else if (customButtonArea.contains(mousePos.x, mousePos.y)) {
                            
                            system("start NewProject.chm");
                            return 4; // Custom action
                        }
                    }
                }
            }

            // Обновление анимации меню
            if (!animationFinished) {
                menuElapsedTime += menuClock.restart().asSeconds();
                if (menuElapsedTime >= menuFrameTime) {
                    menuElapsedTime = 0;
                    currentMenuFrame++;

                    if (currentMenuFrame >= menuFrames.size()) {
                        currentMenuFrame = menuFrames.size() - 1;
                        animationFinished = true;
                    }
                    else {
                        menuSprite.setTexture(menuFrames[currentMenuFrame]);
                    }
                }
            }

            // Обработка наведения на кнопки
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            bool isHovered = false;

            // Play button
            if (playButtonArea.contains(mousePos.x, mousePos.y)) {
                playButton.setTexture(&playButtonHoverTexture);
                playButton.setFillColor(sf::Color::White);
                isHovered = true;
            }
            else {
                playButton.setTexture(nullptr);
                playButton.setFillColor(sf::Color::Transparent);
            }

            // New button
            if (newButtonArea.contains(mousePos.x, mousePos.y)) {
                newButton.setTexture(&newButtonHoverTexture);
                newButton.setFillColor(sf::Color::White);
                isHovered = true;
            }
            else {
                newButton.setTexture(nullptr);
                newButton.setFillColor(sf::Color::Transparent);
            }

            // Exit button
            if (exitButtonArea.contains(mousePos.x, mousePos.y)) {
                exitButton.setTexture(&exitButtonHoverTexture);
                exitButton.setFillColor(sf::Color::White);
                isHovered = true;
            }
            else {
                exitButton.setTexture(nullptr);
                exitButton.setFillColor(sf::Color::Transparent);
            }

            // Для новой кнопки не делаем hover-эффект

            // Воспроизведение звука при наведении (только для основных кнопок)
            if (isHovered && !wasHovered) {
                hoverSound.play();
            }
            wasHovered = isHovered;

            // Отрисовка
            window.clear();
            window.draw(menuSprite);
            if (animationFinished) {
                window.draw(playButton);
                window.draw(newButton);
                window.draw(exitButton);
                window.draw(customButton); // Рисуем новую кнопку
            }
            window.display();
        }
        return 0;
    }
};