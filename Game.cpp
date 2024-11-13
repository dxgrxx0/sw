#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game() :
    window(sf::VideoMode(1600, 1000), "Warrior and Monsters"),
    warrior("knightbg.png", 700, 700, 3.0f, 100.0f),
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
    upgradeManager(&warrior, &mainTower),
    upgradeUI(font, sf::Vector2f(window.getSize()))
{
    minimap.setPosition(3, 3);  // 기본 미니맵 위치 설정
    font.loadFromFile("arial.ttf");
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
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
    if (upgradeUI.getIsVisible()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int choice = upgradeUI.handleClick(mousePos);
            if (choice != -1) {
                upgradeManager.applyUpgrade(choice); // 선택된 업그레이드 적용
                upgradeUI.hide(); // UI 숨기기 (업그레이드 완료)
            }
        }
        return; // 업그레이드 UI가 활성화된 동안 다른 게임 업데이트 중단
    }

    float deltaTime = clock.restart().asSeconds();
    /*if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
        spawnMonster();
        spawnClock.restart();
    }*/
    warrior.handleInput(deltaTime);
    warrior.updateAnimation(deltaTime);


    if (warrior.getIsSwinging() && !warrior.getAttackApplied()) {
        warrior.basicAttack(monsters);

        // 체력이 0 이하인 몬스터들을 한 번에 제거
        monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
            [this](const std::unique_ptr<Monster>& monster) {
                if (monster->getHealthPoint() <= 0) {
                    addExp(10);
                }
                return monster->getHealthPoint() <= 0;
            }),
            monsters.end());

        warrior.setAttackApplied(true);  // 공격 적용 완료 플래그 설정
    }
    if (experience > experienceToNextLevel) {
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
}

void Game::render() {
    window.clear();
    if (upgradeUI.getIsVisible()) {
        window.setView(window.getDefaultView());  // 기본 뷰로 변경
        upgradeUI.draw(window); // UI가 활성화된 경우에만 그리기
    }
    else {
        window.setView(mainView);
        window.draw(towerSprite);

        mainTower.draw(window);

        waveManager.drawMonsters(window);
        warrior.draw(window);

        // 미니맵 그리기
        minimap.draw(window);
        uiManager.draw(window);// UI 그리기
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
    upgradeManager.generateUpgradeOptions(); // 업그레이드 옵션 생성
    std::vector<std::string> options = upgradeManager.getUpgradeDescriptions();
    upgradeUI.showOptions(options); // UI에 업그레이드 옵션 표시
}
