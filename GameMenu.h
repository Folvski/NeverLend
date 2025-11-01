#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdexcept>

class GameMenu {
public:
    enum MenuResult { Nothing, Continue, Restart, Exit };

private:
    sf::RenderWindow& gameWindow;

    // Фон меню
    sf::Texture menuTexture;
    sf::Sprite menuSprite;

    // Кнопки (невидимые области)
    sf::RectangleShape continueButton;
    sf::RectangleShape restartButton;
    sf::RectangleShape exitButton;
    sf::FloatRect continueArea;
    sf::FloatRect restartArea;
    sf::FloatRect exitArea;

    // Текстуры кнопок при наведении
    sf::Texture continueHoverTexture;
    sf::Texture restartHoverTexture;
    sf::Texture exitHoverTexture;

    // Звук
    sf::SoundBuffer hoverSoundBuffer;
    sf::Sound hoverSound;
    bool wasHovered;

    bool isMenuOpen;
    bool wasMousePressed;

public:
    GameMenu(sf::RenderWindow& window)
        : gameWindow(window),
        isMenuOpen(false),
        wasHovered(false),
        wasMousePressed(false)
    {
        // Загрузка фона
        if (!menuTexture.loadFromFile("images/game_menu_bg.png")) {
            throw std::runtime_error("Failed to load menu background");
        }
        menuSprite.setTexture(menuTexture);
        menuSprite.setPosition(0, 0);

        // Загрузка текстур для hover-состояния
        if (!continueHoverTexture.loadFromFile("images/continue_btn.png") ||
            !restartHoverTexture.loadFromFile("images/restart_btn.png") ||
            !exitHoverTexture.loadFromFile("images/exit_btn.png"))
        {
            throw std::runtime_error("Failed to load button textures");
        }

        // Инициализация кнопок (размеры и позиции жестко заданы)
        continueButton.setSize(sf::Vector2f(517.f, 79.f));
        continueButton.setPosition((1920.f - 517.f) / 2, 687.f);
        continueButton.setFillColor(sf::Color::Transparent);
        continueArea = continueButton.getGlobalBounds();

        restartButton.setSize(sf::Vector2f(302.f, 57.f));
        restartButton.setPosition((1920.f - 302.f) / 2, 787.f);
        restartButton.setFillColor(sf::Color::Transparent);
        restartArea = restartButton.getGlobalBounds();

        exitButton.setSize(sf::Vector2f(389.f, 57.f));
        exitButton.setPosition((1920.f - 389.f) / 2, 865.f);
        exitButton.setFillColor(sf::Color::Transparent);
        exitArea = exitButton.getGlobalBounds();

        // Звук наведения
        if (!hoverSoundBuffer.loadFromFile("hover_sound.ogg")) {
            throw std::runtime_error("Failed to load hover sound");
        }
        hoverSound.setBuffer(hoverSoundBuffer);
    }

    void toggleMenu() {
        isMenuOpen = !isMenuOpen;
        if (isMenuOpen) {
            wasMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        }
    }

    bool isOpen() const { return isMenuOpen; }

    MenuResult handleInput() {
        if (!isMenuOpen) return Nothing;

        bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        MenuResult result = Nothing;

        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);

        if (isMousePressed && !wasMousePressed) {
            if (continueArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                result = Continue;
            }
            else if (restartArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                result = Restart;
            }
            else if (exitArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                result = Exit;
            }
        }

        wasMousePressed = isMousePressed;
        return result;
    }

    void draw() {
        if (!isMenuOpen) return;

        // Затемнение фона
        sf::RectangleShape overlay(sf::Vector2f(gameWindow.getSize()));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        gameWindow.draw(overlay);

        // Фон меню
        gameWindow.draw(menuSprite);

        // Отрисовка кнопок при наведении
        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
        bool isHovered = false;

        // Continue button
        if (continueArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            continueButton.setTexture(&continueHoverTexture);
            isHovered = true;
        }
        else {
            continueButton.setTexture(nullptr);
        }

        // Restart button
        if (restartArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            restartButton.setTexture(&restartHoverTexture);
            isHovered = true;
        }
        else {
            restartButton.setTexture(nullptr);
        }

        // Exit button
        if (exitArea.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            exitButton.setTexture(&exitHoverTexture);
            isHovered = true;
        }
        else {
            exitButton.setTexture(nullptr);
        }

        // Воспроизведение звука при наведении
        if (isHovered && !wasHovered) {
            hoverSound.play();
        }
        wasHovered = isHovered;

        // Рисуем кнопки
        gameWindow.draw(continueButton);
        gameWindow.draw(restartButton);
        gameWindow.draw(exitButton);
    }
};