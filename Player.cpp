#include "Player.h"

Player::Player(const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(2.0f, 2.0f);
    speed = 100.f;

    // Initialize frame rect 
    frameRect = sf::IntRect(0, 0, 16, 16);
    sprite.setTextureRect(frameRect);
    sprite.setPosition(400.f, 300.f);

    currentFrame = 0;
    directionRow = 1; 
    animationTimer = 0.f;
    frameDuration = 0.1f;
}

void Player::handleInput(float dt) {
    velocity = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= 1.f;
        sprite.setScale(-2.0f, 2.0f);
        sprite.setOrigin(16.f, 0.f);
        directionRow = 1; 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += 1.f;
        sprite.setScale(2.0f, 2.0f);
        sprite.setOrigin(0.f, 0.f);
        directionRow = 1;
    }

    // Normalize diagonal movement
    if (velocity.x != 0.f || velocity.y != 0.f) {
        float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= length;
    }

    sprite.move(velocity * speed * dt);
}

sf::Sprite& Player::getSprite() {
    return sprite;
}

void Player::update(float dt) {
    updateAnimation(dt);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::updateAnimation(float dt) {
    if (velocity.x != 0 || velocity.y != 0) {
        animationTimer += dt;
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f;
            currentFrame = (currentFrame + 1) %4; 

            frameRect.left = currentFrame * 16;
            frameRect.top = directionRow * 16;
            sprite.setTextureRect(frameRect);
        }
    } else {
        currentFrame = 0;
        frameRect.left = currentFrame * 16;
        frameRect.top = directionRow * 16;
        sprite.setTextureRect(frameRect);
    }
}
