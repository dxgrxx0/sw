#include "Game.h"
#include "BladeWhirl.h"
#include "BulkUp.h"
#include "Teleport.h"
#include "ArrowTower.h"
#include "WizardTower.h"
#include "TrainingTower.h"
#include "BombTower.h"
#include "Dash.h"
#include "CannonTower.h"
#include "BombTower.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <chrono>
#include "UpgradeManager.h"

Game::Game() :
    window(sf::VideoMode(1600, 1000), "Warrior and Monsters"),
    warrior("knight.png", 600, 500, 1.0f, 100.0f),
    uiManager(font, &warrior, window),
    minimap(600, 600, 0.4f),
    mainView(sf::FloatRect(0, 0, 1600, 1000)),
    mainTower(sf::Vector2f(650.0f, 500.0f)), // MainTower 위치 설정
    towerView(sf::FloatRect(150, 0, 1000, 1000)),
    spawnInterval(1.0f),
    monsterSpeed(50.0f),
    experience(0),
    level(1),
    experienceToNextLevel(100),
    waveManager(&warrior, &mainTower, &monsters, 1600, 1000),
    upgradeManager(),
    upgradeUI(font, sf::Vector2f(window.getSize())),
    screenUI(sf::Vector2f(window.getSize())),  // Add ScreenUI initialization
    isGameOver(false),
    mainBossDefeated(false),
    isVictory(false)
{
	loadResources();
    minimap.setPosition(3, 3);  // 기본 미니맵 위치 설정
    font.loadFromFile("arial.ttf");
	backgroundTexture.loadFromFile("background.png");
    bossbackgroundTexture.loadFromFile("Bossbackground.png");
	backgroundSprite.setTexture(backgroundTexture);
    screenUI.loadResources("StartUi.png", "PixelOperator8.ttf");
    cacheBackground();
    gameStarted = false;
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        // Check if game has started
        if (!gameStarted) {
            // Update and render only the start screen
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            screenUI.update(mousePos);
            window.clear();
            screenUI.draw(window);
            window.display();
            // Check for start button click
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (screenUI.handleClick(mousePos)) {
                    gameStarted = true;
                    clock.restart();
                }
            }
            continue;  // Skip main game update and render
        }      
        update();
        // Normal game update and render
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}
void Game::update() {
    if (!gameStarted || isGameOver || isVictory) {
        return;
    }
    // Game Over 조건
    if (mainTower.getHealth() <= 0 || warrior.getHealth() <= 0) {
        //isGameOver = true;
       // screenUI.setGameOver(true);
    }
    else if (mainBossDefeated) {
        isVictory = true;
        screenUI.setVictory(true);  // ScreenUI에 victory 상태 설정
        return;
    }

    if (upgradeUI.getIsVisible()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int choice = upgradeUI.handleClick(mousePos);
            if (choice != -1) {
                upgradeManager.applyUpgrade(choice); // 선택된 업그레이드 적용
                upgradeUI.hide(); // UI 숨기기 (업그레이드 완료)
                clock.restart();
            }
        }
        return; // 업그레이드 UI가 활성화된 동안 다른 게임 업데이트 중단
    }
    deltaTime = clock.restart().asSeconds();
    warrior.handleInput(deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        skillManager.activateSkill(sf::Keyboard::Q); // Q스킬 활성화
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        skillManager.activateSkill(sf::Keyboard::W); // W스킬 활성화
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        skillManager.activateSkill(sf::Keyboard::E); // E스킬 활성화
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        skillManager.activateSkill(sf::Keyboard::R); // R스킬 활성화
    }
    warrior.updateAnimation(deltaTime);


    if (warrior.getIsSwinging() && !warrior.getAttackApplied()) {
        warrior.basicAttack(monsters);

        warrior.setAttackApplied(true);  // 공격 적용 완료 플래그 설정
    }
    // 체력이 0 이하인 몬스터들을 한 번에 제거
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
        [this](const std::unique_ptr<Monster>& monster) {
            if (monster->getHealthPoint() <= 0) {
                if (monster->getBossMonsterType() == MonsterType::Main_Boss) {
                    mainBossDefeated = true;  // 메인 보스 처치 표시
                    printf("Main Boss has been defeated!\n");
                }
                addExp(100);
            }
            return monster->getHealthPoint() <= 0;
        }),
        monsters.end());
    if (experience >= experienceToNextLevel) {
        onLevelUp();
    }

    sf::Vector2f warriorPosition = warrior.getPosition();
    /*for (auto& monster : monsters) {
        monster.update(warriorPosition, deltaTime);
    }*/
    waveManager.update(deltaTime);
    mainView.setCenter(warriorPosition);

    minimap.update(towerView, mainTower, monsters, warrior);

    minimap.setPosition(mainView.getCenter().x - mainView.getSize().x / 2 + 3,
        mainView.getCenter().y + mainView.getSize().y / 2 - 243);
    uiManager.updateCharacterHealth();
    uiManager.updateLevelBar(level, experience, experienceToNextLevel);
    uiManager.updateTimer(deltaTime);
    uiManager.updateTowerDurability(mainTower.getHealth(), mainTower.getMaxHealth());
    mainTower.healNearbyCharacter(deltaTime, warrior);
    skillManager.updateSkills(deltaTime);
    subTowerManager.updateTowers(monsters,deltaTime);
    
}

void Game::render() {
    window.clear();
    if (isGameOver) {
        window.setView(window.getDefaultView());
        screenUI.draw(window);
    }
    else if (isVictory) {
        window.setView(window.getDefaultView());
        screenUI.draw(window);
    }
    /*else if (upgradeUI.getIsVisible()) {
        window.setView(window.getDefaultView());  // 기본 뷰로 변경
        upgradeUI.draw(window); // UI가 활성화된 경우에만 그리기
    }*/
    else {
        if (waveManager.isBossSpawned()&&isBackgroundCached==false) {
            backgroundSprite.setTexture(bossbackgroundTexture);
			cacheBackground();
			isBackgroundCached = true;
        }
        window.draw(cachedBackgroundSprite);
        

        window.setView(mainView);
        window.draw(towerSprite);

        mainTower.draw(window);
        subTowerManager.drawTowers(window);
        waveManager.drawMonsters(window);
        warrior.draw(window);
        if (skillManager.hasSkill("BladeWhirl")) {
            BaseSkill* skill = skillManager.getSkill("BladeWhirl");
            // dynamic_cast로 BladeWhirl로 안전하게 변환
            BladeWhirl* bladeWhirl = dynamic_cast<BladeWhirl*>(skill);
            if (bladeWhirl) {
                bladeWhirl->draw(window);
            }
        }

        // 미니맵 그리기
        minimap.draw(window);
        uiManager.draw(window);// UI 그리기
        uiManager.updateSkillCoolTime(skillManager);
		
    }
    if (upgradeUI.getIsVisible()) {
        window.setView(window.getDefaultView());  // 기본 뷰로 변경
        upgradeUI.draw(window); // UI가 활성화된 경우에만 그리기
    }
    window.display();
}
/*
void Game::spawnMonster() {
    float x = static_cast<float>(std::rand() % window.getSize().x);
    float y = static_cast<float>(std::rand() % window.getSize().y);
    monsters.emplace_back(x, y, monsterSpeed);
}*/
void Game::addExp(float exp) {
    experience += exp;
}
void Game::onLevelUp() {
    experience -= experienceToNextLevel;
    experienceToNextLevel *= 1.5f;
    level += 1;
    if (level == 2) {
        skillManager.unlockSkill("BladeWhirl");
        skillManager.addSkill("BladeWhirl", std::make_unique<BladeWhirl>(&warrior, monsters));
        subTowerManager.addTower(std::make_unique<WizardTower>(sf::Vector2f(350,326)));
        subTowerManager.addTower(std::make_unique<BombTower>(sf::Vector2f(950, 326)));
        subTowerManager.addTower(std::make_unique<TrainingTower>(sf::Vector2f(650, 846)));

        subTowerManager.addTower(std::make_unique<ArrowTower>(sf::Vector2f(250, 846)));
        subTowerManager.addTower(std::make_unique<CannonTower>(sf::Vector2f(1050, 846)));
    }
    if (level == 3) {
        skillManager.unlockSkill("BulkUp");
        skillManager.addSkill("BulkUp", std::make_unique<BulkUp>(&warrior));
    }
    if (level == 4) {
        skillManager.unlockSkill("Dash");
		skillManager.addSkill("Dash", std::make_unique<Dash>(&warrior));
    }
    if (level == 5) {
        skillManager.unlockSkill("Teleport");
        skillManager.addSkill("Teleport", std::make_unique<Teleport>(&warrior,&mainTower));
    }
    upgradeManager.generateUpgradeOptions(); // 업그레이드 옵션 생성
    // 설명과 이미지 경로 각각 가져오기
    std::vector<std::string> descriptions = upgradeManager.getUpgradeDescriptions();
    std::vector<std::string> imagePaths = upgradeManager.getUpgradeImagePaths();

    // UI에 옵션 표시
    upgradeUI.showOptions(descriptions, imagePaths);
}
void Game::loadResources() {
    ResourceManager& rm = ResourceManager::getInstance();
    rm.loadTexture("ArrowTower", "ArrowTower.png");
    rm.loadTexture("WizardTower", "WizardTower.png");
    rm.loadTexture("TrainingTower", "TrainingTower.png");
    rm.loadTexture("BombTower", "BombTower.png");
    rm.loadTexture("SpeedMonster", "speedMonster.png");
    rm.loadTexture("AttackMonster", "attackMonster.png");
    rm.loadTexture("BasicMonster", "basicMonster.png");
    rm.loadTexture("DefenseMonster", "defenseMonster.png");
    rm.loadTexture("MainBoss", "mainboss.png");
    rm.loadTexture("MidBoss", "midboss.png");
    rm.loadTexture("BossExplode", "BossExplode.png");
	rm.loadTexture("GreenBomb", "greenBomb.png");
	rm.loadTexture("GreenBombExplode", "greenBombExplode.png");
	rm.loadTexture("PinkBomb", "pinkBomb.png");
	rm.loadTexture("PinkBombExplode", "pinkBombExplode.png");
	rm.loadTexture("YellowBomb", "yellowBomb.png");
	rm.loadTexture("YellowBombExplode", "yellowBombExplode.png");
	rm.loadTexture("BlueBomb", "blueBomb.png");
	rm.loadTexture("BlueBombExplode", "blueBombExplode.png");
    rm.loadTexture("RedPen", "RedPen.png");
	rm.loadTexture("OrangePen", "OrangePen.png");
	rm.loadTexture("YellowPen", "YellowPen.png");
	rm.loadTexture("GreenPen", "GreenPen.png");
	rm.loadTexture("BluePen", "BluePen.png");
	rm.loadTexture("NavyPen", "NavyPen.png");
	rm.loadTexture("PurplePen", "PurplePen.png");
    rm.loadFont("Arial", "arial.ttf");


    rm.loadTexture("health", "health.png");
    rm.loadTexture("power", "power.png");
    rm.loadTexture("range", "range.png");
    
}
void Game::cacheBackground() {
	backgroundCache.create(4800, 3000);
	backgroundCache.clear();
    int horizontalTiles =17;  // 타일 갯수 계산
    int verticalTiles = 16;
    for (int i = 0; i < horizontalTiles; ++i) {
        for (int j = 0; j < verticalTiles; ++j) {
            backgroundSprite.setPosition(i * 300, j * 200);
            backgroundCache.draw(backgroundSprite);
        }
    }
    backgroundCache.display(); // 렌더링 완료

    // RenderTexture의 텍스처를 Sprite에 설정
    cachedBackgroundSprite.setTexture(backgroundCache.getTexture());
	cachedBackgroundSprite.setPosition(-1600, -1000);
}
