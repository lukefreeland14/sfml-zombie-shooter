#include "Enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(const sf::Texture& texture) {
    health = 3;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 500, 433));
    sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(100.f, 100.f); // Example spawn position
    speed = 25.f;
    std::cout << "[DEBUG] Constructor: enemy starting health = " << health << "\n";
    
    
}


void Enemy::update(sf::Vector2f playerPosition, sf::Time deltaTime) {
    // Get current position of the enemy
    sf::Vector2f enemyPos = sprite.getPosition();
   
    // Calculate direction towards the player
    sf::Vector2f direction = playerPosition - enemyPos;


    // Normalize the direction vector to prevent faster diagonal movement
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    direction = (length != 0.f) ? direction / length : sf::Vector2f(0.f, 0.f);




    // Move the enemy towards the player based on the normalized direction and speed
    sprite.move(direction * speed * deltaTime.asSeconds());
}


sf::Vector2f Enemy::getPosition() const {
    return sprite.getPosition();
}


sf::FloatRect Enemy::getBounds() const {
    return sprite.getGlobalBounds();
}


void Enemy::takeDamage(int amount) {
    health -= amount;
    std::cout << "[DEBUG] Enemy took damage, new health: " << health << "\n";
}



bool Enemy::isDead() const {
    return health <= 0;
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!isDead()) {
        window.draw(sprite);
    }
}

void Enemy::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

