#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Character.h"
#include "Monster.h"

#include "Minimap.h"
#include "Utility.h"
#include "MainTower.h"
#include "UiManager.h"
#include "WaveManager.h"
class Game {
public:
    Game();
    void run();
    void addExp(float exp);
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
    int level;
    float experience;
    float experienceToNextLevel;

    sf::Font font;
    UIManager uiManager;
    MainTower mainTower;
    WaveManager waveManager;

};

#endif // GAME_H
