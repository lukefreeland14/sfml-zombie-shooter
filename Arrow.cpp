#include "Arrow.h"
#include <cmath>

Arrow::Arrow(sf::Vector2f position, sf::Vector2f target, const sf::Texture& texture) {
    arrowSprite.setTexture(texture);

    // Frame location: Row 5, Column 0 => (0, 5 * 16)
    arrowSprite.setTextureRect(sf::IntRect(80, 48, 16, 8));
    arrowSprite.setOrigin(8.f, 8.f);
    arrowSprite.setPosition(position);

    // Calculate normalized direction vector
    sf::Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        velocity = (direction / length) * speed;
    else
        velocity = {0.f, 0.f};

    // Set rotation (convert to degrees)
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    arrowSprite.setRotation(angle);
}

void Arrow::update(sf::Time deltaTime) {
    arrowSprite.move(velocity * deltaTime.asSeconds());
    
}

void Arrow::draw(sf::RenderWindow& window) {
    window.draw(arrowSprite);
}

sf::FloatRect Arrow::getBounds() const {
    return arrowSprite.getGlobalBounds();
}

sf::Vector2f Arrow::getPosition() const {
    return arrowSprite.getPosition();
}

