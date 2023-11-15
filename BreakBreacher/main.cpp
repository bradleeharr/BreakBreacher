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
    float BULLET_SPEED = 200.f;
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

void resizeWindow(HWND hwnd, float l, float r, float u, float d) {
    RECT rect;
    int dw = r + l;
    int dh = u + d;
    GetWindowRect(hwnd, &rect); 
    printf("Left %d", rect.left);
    printf("Right %d", rect.right);
    printf("Top %d", rect.top);
    printf("Bottom %d", rect.bottom);
    int width = rect.right - rect.left + dw;
    int height = rect.bottom - rect.top + dh;
    SetWindowPos(hwnd, NULL, rect.left-l, rect.top-u, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Plus Shaped Content");
    sf::Vector2u windowSize = window.getSize();
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

    sf::Texture playerTexture;
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

        window.clear(sf::Color::Blue); 

        window.draw(verticalBar);
        window.draw(horizontalBar);
        player.draw(window);
        player.shoot(bullets);
        player.handleInput(deltaTime);

        for (size_t i = 0; i < bullets.size();) {
            bullets[i].update(deltaTime);



            sf::Vector2f bullet_pos = bullets[i].getPosition();
            RECT rect;
            GetWindowRect(hwnd, &rect); 

            if (bullet_pos.x < 0) {
                printf("Bullet on Left\n");
                bullets.erase(bullets.begin() + i);
                resizeWindow(hwnd, 1, 0, 0, 0);
            }
            if (bullet_pos.x > rect.right - rect.left) {
                printf("Bullet on Right\n");
                bullets.erase(bullets.begin() + i);
                resizeWindow(hwnd, 0, 1, 0, 0);
            }
            if (bullet_pos.y < 0) {
                printf("Bullet on Top\n");
                bullets.erase(bullets.begin() + i);
                resizeWindow(hwnd, 0, 0, 1, 0);
            }
            if (bullet_pos.y > rect.bottom - rect.top) {
                printf("Bullet on Bottom\n");
                bullets.erase(bullets.begin() + i);
                resizeWindow(hwnd, 0, 0, 0, 1);
            }
            else {
                ++i;
            }
        }
        float dz = 1.;
        //resizeWindow(hwnd, 0, 0, 0, 0);
        for (Bullet& b : bullets) {
            b.draw(window);
        } 
        window.display();
    }

    return 0;
}
