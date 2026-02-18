#include "Game.h"
#include <iostream>
#include <fstream>


Game::Game()
    : window(sf::VideoMode(800, 600), "Archer Hero Clone"),
      player(playerTexture), mt(std::random_device()()) //Setting the mt seed with a new random device
{
    loadHighScore();

    // Load player texture
    sf::Image playerImage;
    if (playerImage.loadFromFile("Assets/Archer.png")) {
        playerImage.createMaskFromColor(sf::Color(255, 0, 255));
        playerTexture.loadFromImage(playerImage);
    }


    // Load enemy texture
    sf::Image enemyImage;
    if (enemyImage.loadFromFile("Assets/Zombie 01/02-Walk/__Zombie01_Walk_000.png")) {
        enemyTexture.loadFromImage(enemyImage);
    }

    if (!font.loadFromFile("Assets/arial.ttf")) {
        std::cout << "Failed to load font!\n";
    }
    
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    
    // Center text
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.width / 2, textRect.height / 2);
    gameOverText.setPosition(800 / 2.f, 600 / 2.f);

    // Round display
    roundText.setFont(font);
    roundText.setCharacterSize(24);
    roundText.setFillColor(sf::Color::White);
    roundText.setPosition(10.f, 10.f);
    roundText.setString("Round 1");

    // attempt to fix loop
    isGameOver = false;
    round = 1;
    roundTransitioning = false;

    // Spawn first wave
    spawnWave(round);

    
}


void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        processEvents();
        update(dt);
        render();
    }
}


void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();


        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {


            sf::Vector2f playerPos = player.getPosition();
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePos);


            arrows.emplace_back(playerPos, mousePosWorld, playerTexture);
        }
    }
}


void Game::update(sf::Time deltaTime) {
    if (isGameOver) return;
    // std::cout << "[DEBUG] roundTransitioning: " << roundTransitioning << ", enemies: " << enemies.size() << "\n";


    // Handle round delay only once
    if (roundTransitioning) {
        if (roundTimer.getElapsedTime().asSeconds() > 1.f) {
            ++round;
            spawnWave(round);
            roundTransitioning = false;
        }
        return;
    }

    float dt = deltaTime.asSeconds();

    // player input
    player.handleInput(dt);
    player.update(dt);

    // arrow updates
    for (auto it = arrows.begin(); it != arrows.end();  ) {
        it->update(deltaTime);

        bool erased = false;
        for (auto& enemy : enemies) {
            if (!enemy.isDead() && enemy.getBounds().intersects(it->getBounds())) {
                enemy.takeDamage(1);
                std::cout << "[DEBUG] Arrow hit! Enemy health: " << enemy.getHealth() << "\n";
                std::cout << "[DEBUG] Checking collision with arrow and enemy\n";

                it = arrows.erase(it);
                erased = true;
                break;
            }
        }

        if (erased) continue;

        sf::Vector2f pos = it->getPosition();
        if (pos.x < 0 || pos.y < 0 || pos.x > window.getSize().x || pos.y > window.getSize().y) {
            it = arrows.erase(it);
        } else {
            ++it;
        }
    }

    // enemy updates
    for (auto& enemy : enemies) {
        if (!enemy.isDead()) {
            enemy.update(player.getPosition(), deltaTime);
        }
    }

    // player collision with enemies
    for (auto& enemy : enemies) {
        if (enemy.isDead()) continue;

        sf::FloatRect enemyBounds = enemy.getBounds();
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();

        enemyBounds.left += 40.f;
        enemyBounds.top += 40.f;
        enemyBounds.width -= 80.f;
        enemyBounds.height -= 80.f;

        playerBounds.left += 40.f;
        playerBounds.top += 40.f;
        playerBounds.width -= 80.f;
        playerBounds.height -= 80.f;

        if (enemyBounds.intersects(playerBounds)) {
            std::cout << "Enemy touched player! Game over.\n";
            saveHighScore();
            isGameOver = true;
            return;
        }
    }

    // trigger round transition if enemies exist and all are dead
    // Only evaluate round transition if there are enemies
    if (!roundTransitioning && !isGameOver && !enemies.empty()) {
        bool allDead = true;
        for (const auto& enemy : enemies) {
            if (!enemy.isDead()) {
                allDead = false;
                break;
            }
        }

        if (allDead) {
            std::cout << "[DEBUG] All enemies dead. Starting round delay timer.\n";
            roundTransitioning = true;
            roundTimer.restart();
        }
    }

    
}


void Game::render() {
    window.clear(sf::Color::Black);


    player.draw(window);


    for (auto& arrow : arrows)
        arrow.draw(window);


    for (auto& enemy : enemies) {
       // std::cout << "Enemies in wave: " << enemies.size() << "\n";

        enemy.draw(window); 
    }

    if (isGameOver) {
        window.draw(gameOverText);
    } else {
        window.draw(roundText);
    }
    

    window.display();

}

void Game::spawnWave(int count) {
    enemies.clear();

    std::uniform_real_distribution<float> xdist(50, 750);
    std::uniform_real_distribution<float> ydist(50, 550);

    float safeRadius = 100.f; // Don't spawn enemies within 100 pixels of player
    sf::Vector2f playerPos = player.getPosition();

    for (int i = 0; i < count; ) {
        float x = xdist(mt);
        float y = ydist(mt);
        sf::Vector2f spawnPos(x, y);

        float dx = spawnPos.x - playerPos.x;
        float dy = spawnPos.y - playerPos.y;
        float distanceSquared = dx * dx + dy * dy;

        if (distanceSquared < safeRadius * safeRadius) {
            continue;
        }

        Enemy newEnemy(enemyTexture);
        newEnemy.setPosition(spawnPos);
        enemies.push_back(newEnemy);
        ++i;
    }

    roundText.setString("Round " + std::to_string(round));
    std::cout << "[DEBUG] --- spawnWave called for round " << round << "\n";
    std::cout << "Spawned round " << round << " with " << count << " enemies.\n";
}

void Game::loadHighScore() {
    std::ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> highScore;
        inFile.close();
    }
}

void Game::saveHighScore() {
    if (round > highScore) {
        std::ofstream outFile("highscore.txt");
        if (outFile.is_open()) {
            outFile << round;
            outFile.close();
            std::cout << "[DEBUG] New high score saved: " << round << "\n";
        }
    }
}



