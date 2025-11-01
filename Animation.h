#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

class Animation {
private:
    sf::RenderWindow& window;
    std::vector<sf::Texture> frames;
    sf::Sprite currentFrame;
    sf::Music animationMusic;
    sf::Clock animationClock;

    float frameDuration;
    int currentFrameIndex;
    bool isPlaying;
    bool animationFinished;
    bool reachedLastFrame;

public:
    Animation(sf::RenderWindow& win)
        : window(win), frameDuration(0.09f),
        currentFrameIndex(0), isPlaying(false),
        animationFinished(false), reachedLastFrame(false) {

        // Загрузка 50 кадров анимации
        frames.resize(50);
        for (int i = 0; i < 50; ++i) {
            if (!frames[i].loadFromFile("images/animation/a" + std::to_string(i + 1) + ".png")) {
                throw std::runtime_error("Failed to load animation frame " + std::to_string(i + 1));
            }
        }

        currentFrame.setTexture(frames[0]);

        // Центрируем анимацию
        sf::FloatRect bounds = currentFrame.getLocalBounds();
        currentFrame.setOrigin(bounds.width / 2, bounds.height / 2);
        currentFrame.setPosition(window.getSize().x / 2, window.getSize().y / 2);

        // Загрузка музыки
        if (!animationMusic.openFromFile("music.ogg")) {
            throw std::runtime_error("Failed to load animation music");
        }
        animationMusic.setLoop(true);
    }

    void start() {
        if (isPlaying) return;

        isPlaying = true;
        animationFinished = false;
        reachedLastFrame = false;
        currentFrameIndex = 0;
        currentFrame.setTexture(frames[0]);
        animationClock.restart();
        animationMusic.play();
    }

    void stopMusic() { animationMusic.stop(); }
    void continueMusic() { animationMusic.play(); }
    bool isMusicPlaying() const {
        return animationMusic.getStatus() == sf::Music::Playing;
    }

    bool update() {
        if (!isPlaying) return false;

        if (reachedLastFrame) {
            // Проверяем нажатие любой клавиши
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
                isPlaying = false;
                animationFinished = true;
                return true;
            }
            return false;
        }

        float elapsed = animationClock.getElapsedTime().asSeconds();
        int newFrameIndex = static_cast<int>(elapsed / frameDuration);

        if (newFrameIndex >= 50) {
            reachedLastFrame = true;
            currentFrameIndex = 49; // Устанавливаем последний кадр
            currentFrame.setTexture(frames[currentFrameIndex]);
        }
        else if (newFrameIndex != currentFrameIndex) {
            currentFrameIndex = newFrameIndex;
            currentFrame.setTexture(frames[currentFrameIndex]);
        }

        return false;
    }

    void draw() {
        if (!isPlaying) return;
        window.draw(currentFrame);
    }

    bool isAnimationFinished() const {
        return animationFinished;
    }
};