using namespace sf;

class LifeBar {
public:
    Texture fullHeartTexture;
    Texture emptyHeartTexture;
    Sprite hearts[3]; // три спрайта для сердечек
    int maxHealth;
    int heartsCount;

    LifeBar() {
        if (!fullHeartTexture.loadFromFile("images/full_heart.png")) {
            // Обработка ошибки загрузки
        }
        if (!emptyHeartTexture.loadFromFile("images/empty_heart.png")) {
            // Обработка ошибки загрузки
        }

        for (int i = 0; i < 3; i++) {
            hearts[i].setTexture(fullHeartTexture);
            hearts[i].setScale(0.3f, 0.3f); // Масштабирование при необходимости
        }
        maxHealth = 30; // Максимальное здоровье игрока
        heartsCount = 3; // Количество сердечек
    }

    void update(int health) {
        // Рассчитываем количество полных сердечек
        int fullHearts = (health * heartsCount) / maxHealth;
        if (fullHearts < 0) fullHearts = 0;

        for (int i = 0; i < heartsCount; i++) {
            if (i < fullHearts) {
                hearts[i].setTexture(fullHeartTexture);
            }
            else {
                hearts[i].setTexture(emptyHeartTexture);
            }
        }
    }

    void draw(RenderWindow& window) {
        Vector2f center = window.getView().getCenter();
        Vector2f size = window.getView().getSize();
        float startX = center.x - size.x / 2 + 10;
        float startY = center.y - size.y / 2 + 10;

        for (int i = 0; i < heartsCount; i++) {
            hearts[i].setPosition(startX + i * 40, startY); // Располагаем сердечки горизонтально
            window.draw(hearts[i]);
        }
    }
};