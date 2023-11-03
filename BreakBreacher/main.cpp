#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include <vector>

class Bullet {
public:
    Bullet(sf::Vector2f& position) {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);
    }

    void update(sf::Time& deltaTime) {
        shape.move(0, -speed * deltaTime.asSeconds());
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    bool isOffScreen() {
        return shape.getPosition().y < 0;
    }

private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed = 400.f; // pixels per second

};

class Player {
public:
    Player(const sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
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
        sprite.move(movement);
    }
    float BULLET_RADIUS = 5.f;

    void shoot(std::vector<Bullet>& bullets) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            sf::Vector2f pos(0.f, 0.f);
            pos.x = sprite.getPosition().x;
            pos.y = sprite.getPosition().y - sprite.getGlobalBounds().height / 2 - BULLET_RADIUS;
            Bullet bullet = Bullet(pos);
            bullets.push_back(bullet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            sf::Vector2f pos(0.f, 0.f);
            pos.x = sprite.getPosition().x;
            pos.y = sprite.getPosition().y + sprite.getGlobalBounds().height / 2 - BULLET_RADIUS;
            Bullet bullet = Bullet(pos);
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
    const float playerSpeed = 200.f; // pixels per second
};




int main() {
    // Create the main window with an opaque background
    sf::RenderWindow window(sf::VideoMode(800, 600), "Plus Shaped Content");
    sf::Vector2u windowSize = window.getSize();

    // Define the plus shape using rectangles
    sf::RectangleShape verticalBar(sf::Vector2f(100, 300)); // width, height
    sf::RectangleShape horizontalBar(sf::Vector2f(300, 100)); // width, height
    // Set the origin of the bars to the center of each rectangle
    verticalBar.setOrigin(50, 150); // half of width, half of height
    horizontalBar.setOrigin(150, 50); // half of width, half of height
    // Position the bars to the center of the window
    verticalBar.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    horizontalBar.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    // Set fill colors (or textures)
    verticalBar.setFillColor(sf::Color::Green);
    horizontalBar.setFillColor(sf::Color::Green);

    std::vector<Bullet> bullets;

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png")) {
        return -1;
    }
    Player player = Player(playerTexture);

    // playerSprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
    float playerSpeed = 200.f; // pixels per second

    // Clock for timing
    sf::Clock clock;

    // Start the game loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();

        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear(sf::Color::Blue); // Fill background with blue

        // Draw the plus shape
        window.draw(verticalBar);
        window.draw(horizontalBar);

        
        player.draw(window);
        player.shoot(bullets);
        player.handleInput(deltaTime);

        HWND hwnd = window.getSystemHandle(); // Get the Win32 window handle from SFML
        // Update bullets' positions
        for (size_t i = 0; i < bullets.size();) {
            // Move each bullet upwards
            bullets[i].update(deltaTime);

            // Check for bullet collision with window top
            if (bullets[i].isOffScreen()) {
                bullets.erase(bullets.begin() + i);

                // Resize the window using the Win32 API
                RECT rect;
                GetWindowRect(hwnd, &rect); // Get the current size of the window
                // Increase the window size by 1 pixel in height
                SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top + 5, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            else {
                ++i;
            }
        }

        RECT rect;
        GetWindowRect(hwnd, &rect); // Get the current size of the window
        SetWindowPos(hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top - 0.00001, SWP_NOZORDER | SWP_NOACTIVATE);
         
        for (Bullet& b : bullets) {
            b.draw(window);
        } 
        window.display();
    }

    return 0;
}
