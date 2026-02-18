#pragma once

#include <SFML/Graphics.hpp>

class Arrow {
public:
    Arrow(sf::Vector2f position, sf::Vector2f target, const sf::Texture& texture);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;


private:
    sf::Sprite arrowSprite;
    sf::Vector2f velocity;
    float speed = 300.f; // Arrow speed in pixels/sec
};
