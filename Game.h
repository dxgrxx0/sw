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
#include "UpgradeManager.h"
#include "UpgradeUI.h"
#include "SkillManager.h"
#include "SubTowerManager.h"
#include "ScreenUi.h"
class Game {
public:
    Game();
    void run();
    void addExp(float exp);
private:
    void handleEvents();
    void update();
    void render();
    void onLevelUp();

    sf::RenderWindow window;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

    sf::Texture bossbackgroundTexture;
    sf::Sprite bossbackgroundSprite;

    Character warrior;
    sf::Texture mainTowerTexture;
    sf::Sprite towerSprite;
    sf::View mainView, towerView;
    MiniMap minimap;
    sf::Clock clock, spawnClock;
    //std::vector<Monster> monsters;
    std::vector<std::unique_ptr<Monster>> monsters;
    float spawnInterval;
    float monsterSpeed;
    int level;
    float experience;
    float experienceToNextLevel;
    float deltaTime;
    sf::Font font;
    UIManager uiManager;
    MainTower mainTower;
    WaveManager waveManager;
    UpgradeManager upgradeManager;
    SkillManager skillManager;
    UpgradeUI upgradeUI;
	SubTowerManager subTowerManager;
    //screen 변수들
    ScreenUi screenUI;
    bool gameStarted;
    bool isGameOver;
    bool isVictory;  
    bool mainBossDefeated;
    bool mainBossSpawned;

};

#endif // GAME_H
