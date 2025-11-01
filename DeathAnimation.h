#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>

class DeathAnimation {
private:
    sf::RenderWindow& window;
    std::vector<sf::Texture> frames;
    sf::Sprite currentSprite;
    sf::Clock animationClock;
    sf::SoundBuffer deathSoundBuffer;
    sf::Sound deathSound;
    bool isPlaying;
    bool soundPlayed; // Добавляем флаг для отслеживания воспроизведения звука
    float totalDuration = 3.0f;
    float fadeInDuration = 1.0f;
    float fadeOutDuration = 1.0f;

public:
    DeathAnimation(sf::RenderWindow& win) : window(win), isPlaying(false), soundPlayed(false) {
        // Загрузка звука смерти
        if (!deathSoundBuffer.loadFromFile("Smert.ogg")) {
            throw std::runtime_error("Failed to load death sound");
        }
        deathSound.setBuffer(deathSoundBuffer);
        deathSound.setVolume(70);

        // Загрузка кадров анимации
        frames.resize(40);
        for (int i = 0; i < 40; i++) {
            if (!frames[i].loadFromFile("images/death/death" + std::to_string(i + 1) + ".png")) {
                throw std::runtime_error("Failed to load death frame " + std::to_string(i + 1));
            }
        }
        currentSprite.setTexture(frames[0]);

        // Масштабирование
        float scaleX = static_cast<float>(window.getSize().x) / frames[0].getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / frames[0].getSize().y;
        currentSprite.setScale(scaleX, scaleY);
    }

    void start() {
        isPlaying = true;
        animationClock.restart();
        deathSound.play(); // ВОТ ТУТ ВОСПРОИЗВОДИМ ЗВУК
        soundPlayed = true;
    }

    bool update() {
        if (!isPlaying) return false;

        float elapsed = animationClock.getElapsedTime().asSeconds();
        float progress = elapsed / totalDuration;

        // Расчет текущего кадра
        int frameIndex = static_cast<int>(progress * 40);
        if (frameIndex >= 40) frameIndex = 39;
        currentSprite.setTexture(frames[frameIndex]);

        // Эффекты затемнения
        sf::Uint8 alpha = 255;

        // Затемнение в начале
        if (elapsed < fadeInDuration) {
            alpha = static_cast<sf::Uint8>(255 * (elapsed / fadeInDuration));
        }
        // Затемнение в конце
        else if (elapsed > totalDuration - fadeOutDuration) {
            float fadeProgress = (elapsed - (totalDuration - fadeOutDuration)) / fadeOutDuration;
            alpha = static_cast<sf::Uint8>(255 * (1.0f - fadeProgress));
        }

        currentSprite.setColor(sf::Color(255, 255, 255, alpha));

        return elapsed < totalDuration;
    }

    void draw() {
        window.draw(currentSprite);
    }
};