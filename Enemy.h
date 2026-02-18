#pragma once
#include <SFML/Graphics.hpp>


class Enemy {
public:
    Enemy(const sf::Texture& texture);


    void update(sf::Vector2f playerPosition, sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    void setPosition(sf::Vector2f pos);

    int getHealth() const { return health; }



    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;


    void takeDamage(int amount);
    bool isDead() const;


private:
    sf::Sprite sprite;
    float speed=25;
    int health =3;


};
