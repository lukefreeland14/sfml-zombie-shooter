#ifndef GAME_H
#define GAME_H


#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Arrow.h"
#include <random>


class Game {
public:
    Game();
    void run();
    void takeDamage(int amount);
    bool isDead() const;



private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    bool isGameOver = false;
    sf::Font font;
    sf::Text gameOverText;
    int health;

    bool roundTransitioning = false;
    sf::Clock roundTimer;

    sf::RenderWindow window;
    sf::Texture playerTexture;
    sf::Texture enemyTexture;

    int highScore = 0;

    void loadHighScore();
    void saveHighScore();



    Player player;

    std::vector<Enemy> enemies;     
    int round = 1;
    sf::Text roundText;

    void spawnWave(int count);


    std::vector<Arrow> arrows;

    std::mt19937 mt;
};


#endif


