#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <cmath> // For std::sqrt

class Player {
public:
    Player(const sf::Texture& texture);

    void handleInput(float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    sf::Sprite& getSprite(); // Needed for Game::render()

private:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed;

    sf::IntRect frameRect;
    int currentFrame;
    int directionRow;

    float animationTimer;
    float frameDuration;

    void updateAnimation(float dt);
};

#endif
