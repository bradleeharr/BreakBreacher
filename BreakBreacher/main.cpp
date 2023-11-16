#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    }

    void move(sf::Vector2f& movement) {
        sprite.move(movement);

    }

    void handleInput(const sf::Time& deltaTime) {
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= playerSpeed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= playerSpeed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += playerSpeed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += playerSpeed * deltaTime.asSeconds();
        move(movement);
    }
    float BULLET_RADIUS = 10.f;
    float BULLET_SPEED = 400.f;
    void shoot(std::vector<Bullet>& bullets) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            sf::Vector2f pos(sprite.getPosition().x, sprite.getPosition().y - sprite.getGlobalBounds().height / 2 - BULLET_RADIUS);
            sf::Vector2f vel(0, BULLET_SPEED);
            Bullet bullet = Bullet(pos, vel);
            bullets.push_back(bullet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            sf::Vector2f pos(sprite.getPosition().x, sprite.getPosition().y + sprite.getGlobalBounds().height / 2 - BULLET_RADIUS);
            sf::Vector2f vel(0, -BULLET_SPEED);
            Bullet bullet = Bullet(pos, vel);
            bullets.push_back(bullet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            sf::Vector2f pos(sprite.getPosition().x - sprite.getGlobalBounds().width / 2 - BULLET_RADIUS, sprite.getPosition().y);
            sf::Vector2f vel(BULLET_SPEED, 0);
            Bullet bullet = Bullet(pos, vel);
            bullets.push_back(bullet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            sf::Vector2f pos(sprite.getPosition().x + sprite.getGlobalBounds().width / 2 - BULLET_RADIUS, sprite.getPosition().y);
            sf::Vector2f vel(-BULLET_SPEED, 0);
            Bullet bullet = Bullet(pos, vel);
            bullets.push_back(bullet);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    sf::Vector2f getPosition() {
        return sprite.getPosition();
    }

private:
    sf::Sprite sprite;
    const float playerSpeed = 200.f; 
};

void resizeWindow(sf::RenderWindow& window, HWND hwnd, float l, float r, float u, float d) {
    RECT rect;
    int dw = r + l;
    int dh = u + d;
    GetWindowRect(hwnd, &rect); 
    printf("Left %d\n", rect.left);
    printf("Right %d\n", rect.right);
    printf("Top %d\n", rect.top);
    printf("Bottom %d\n", rect.bottom);
    int newWidth = rect.right - rect.left + dw;
    int newHeight = rect.bottom - rect.top + dh;
    SetWindowPos(hwnd, NULL, rect.left-l, rect.top-u, newWidth, newHeight, SWP_NOZORDER | SWP_NOACTIVATE);
    sf::View view = sf::View();
    view.setSize(newWidth, newHeight);
    window.setView(view);
}

void handleBulletBoundaries(sf::RenderWindow& window, HWND hwnd, std::vector<Bullet>& bullets, sf::Time deltaTime) {
    for (size_t i = 0; i < bullets.size();) {
        bullets[i].update(deltaTime);
        sf::Vector2f bullet_pos = bullets[i].getPosition();
        RECT rect;
        GetWindowRect(hwnd, &rect);
        int l = 0, r = 0, t = 0, b = 0;
        if (bullet_pos.x < 0) {
            printf("Bullet on Left\n");
            bullets.erase(bullets.begin() + i);
            l = 1;
        }
        else if (bullet_pos.x > rect.right - rect.left) {
            printf("Bullet on Right\n");
            bullets.erase(bullets.begin() + i);
            r = 1;
        }
        else if (bullet_pos.y < 0) {
            printf("Bullet on Top\n");
            bullets.erase(bullets.begin() + i);
            t = 1;
        }
        else if (bullet_pos.y > rect.bottom - rect.top) {
            printf("Bullet on Bottom\n");
            bullets.erase(bullets.begin() + i);
            b = 1;
        }
        else {
            ++i;
        }
        resizeWindow(window, hwnd, l, r, t, b);
    }
}

std::pair<sf::RectangleShape, sf::RectangleShape> drawCross(sf::Vector2u windowSize) {
    sf::RectangleShape verticalBar(sf::Vector2f(100, 300)); // width, height
    sf::RectangleShape horizontalBar(sf::Vector2f(300, 100)); // width, height
    // Set the origin of the bars to the center of each rectangle
    verticalBar.setOrigin(50, 150); // half of width, half of height
    horizontalBar.setOrigin(150, 50); // half of width, half of height
    // Position the bars to the center of the window
    verticalBar.setPosition(windowSize.x / 2, windowSize.y / 2);
    horizontalBar.setPosition(windowSize.x / 2, windowSize.y / 2);
    // Set fill colors (or textures)
    verticalBar.setFillColor(sf::Color::Green);
    horizontalBar.setFillColor(sf::Color::Green);

    return std::make_pair(verticalBar, horizontalBar);
}

void drawGrid(sf::RenderWindow& window, int gridSize) {
    int windowWidth = window.getSize().x;
    int windowHeight = window.getSize().y;

    sf::RectangleShape line;
    line.setFillColor(sf::Color(128, 128, 128)); // Gray color

    // Draw vertical lines
    for (int x = 0; x <= windowWidth; x += gridSize) {
        line.setSize(sf::Vector2f(1, windowHeight));
        line.setPosition(x, 0);
        window.draw(line);
    }

    // Draw horizontal lines
    for (int y = 0; y <= windowHeight; y += gridSize) {
        line.setSize(sf::Vector2f(windowWidth, 1));
        line.setPosition(0, y);
        window.draw(line);
    }
}

void displayCoordinates(sf::RenderWindow& window, sf::Font& font, int gridSize) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(12); // Choose an appropriate size
    text.setFillColor(sf::Color::Black);

    int windowWidth = window.getSize().x;
    int windowHeight = window.getSize().y;

    for (int x = 0; x <= windowWidth; x += gridSize) {
        for (int y = 0; y <= windowHeight; y += gridSize) {
            text.setString(std::to_string(x) + "," + std::to_string(y));
            text.setPosition(x + 5, y); // Offset a bit for visibility
            window.draw(text);
        }
    }
}
int main() {
    int gridSize = 50; 
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Plus Shaped Content. Now with Coordinates");
    sf::Vector2u windowSize = window.getSize();
    sf::Texture playerTexture;

    std::pair<sf::RectangleShape, sf::RectangleShape> pair = drawCross(windowSize);
    auto verticalBar = pair.first;
    auto horizontalBar = pair.second;

    sf::Font font;
    if (!font.loadFromFile("path/to/font.ttf")) {
        // Handle error
    }
    if (!playerTexture.loadFromFile("player.png")) {
        return -1;
    }
    Player player = Player(playerTexture);
    std::vector<Bullet> bullets;
    sf::Clock clock;
    HWND hwnd = window.getSystemHandle();

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        handleBulletBoundaries(window, hwnd, bullets, deltaTime);
        float dz = 1.;
        //resizeWindow(hwnd, 0, 0, 0, 0);
        window.clear(sf::Color::Blue); 
        drawGrid(window, gridSize);
        displayCoordinates(window, font, gridSize);
        window.draw(verticalBar);
        window.draw(horizontalBar);
        player.draw(window);
        player.shoot(bullets);
        player.handleInput(deltaTime);
        for (Bullet& b : bullets) {
            b.draw(window);
        } 

        window.display();
    }

    return 0;
}
