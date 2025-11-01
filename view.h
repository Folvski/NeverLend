#include <SFML/Graphics.hpp>
using namespace sf;
sf::View view;
void setPlayerCoordinateForView(float x, float y) {
    float tempX = x; float tempY = y;
    //if (x < 320) tempX = 320;
    //if (y < 240) tempY = 240;//верхнюю сторону
    if (y > 624) tempY = 624;//нижнюю стороню.для новой карты
    view.setCenter(tempX, tempY);
}

void GetPlayerCoordinateForView(float x, float y) {
    float tempX = x, tempY = y;

    // Границы камеры (подставьте свои значения)
    const float leftBound = 320;    // Левая граница
    const float rightBound = 1920;  // Правая граница (пример: 1280 для окна шириной 1280px)
    const float topBound = 500;     // Верхняя граница
    const float bottomBound = 554;   // Нижняя граница

    // Ограничиваем камеру
    if (x < leftBound) tempX = leftBound;          // Левая сторона
    if (x > rightBound - 640) tempX = rightBound - 640; // Правая сторона (640 - половина ширины view)
    if (y < topBound) tempY = topBound;            // Верхняя сторона
    if (y > bottomBound) tempY = bottomBound;      // Нижняя сторона

    view.setCenter(tempX, tempY);
}