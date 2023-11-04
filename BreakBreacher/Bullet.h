#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>
#include <vector>

class Bullet {
public:
    Bullet(sf::Vector2f& position, sf::Vector2f& velocity0) {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(position);

        velocity = velocity0;
    }

    void update(sf::Time& deltaTime) {
        shape.move(-velocity.x * deltaTime.asSeconds(), -velocity.y * deltaTime.asSeconds());
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::Vector2f getPosition() {
        return shape.getPosition();
    }

private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    sf::Vector2f velocity = sf::Vector2f(0., 0.); // pixels per second
};