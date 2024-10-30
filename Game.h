#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Character.h"
#include "Monster.h"
#include "MiniMap.h"

class Game {
public:
    Game();
    void run();

private:
    void handleEvents();
    void update();
    void render();
    void spawnMonster();

    sf::RenderWindow window;
    Character warrior;
    sf::Texture mainTowerTexture;
    sf::Sprite towerSprite;
    sf::View mainView, towerView;
    MiniMap minimap;
    sf::Clock clock, spawnClock;
    std::vector<Monster> monsters;
    float spawnInterval;
    float monsterSpeed;
};

#endif // GAME_H
