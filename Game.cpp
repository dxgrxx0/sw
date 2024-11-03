#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game() :
    window(sf::VideoMode(1600, 1400), "Warrior and Monsters"),
    warrior("knightbg.png", 700, 700, 2.0f, 100.0f),
    minimap(600, 600, 0.4f),
    mainView(sf::FloatRect(0, 0, 1600, 1400)),
    towerView(sf::FloatRect(500, 400, 600, 600)),
    spawnInterval(1.0f),
    monsterSpeed(50.0f)
    ,Timer(750, 30)
{
    if (!mainTowerTexture.loadFromFile("tower.PNG")) {
        throw std::runtime_error("Failed to load tower image");
    }
    towerSprite.setTexture(mainTowerTexture);
    towerSprite.setPosition(650, 500);
    minimap.setPosition(3, 3);  // 기본 미니맵 위치 설정
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
    float deltaTime = clock.restart().asSeconds();
    if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
        spawnMonster();
        spawnClock.restart();
    }
    warrior.handleInput(deltaTime);
    warrior.updateAnimation(deltaTime);
    sf::Vector2f warriorPosition = warrior.getPosition();
    for (auto& monster : monsters) {
        monster.update(warriorPosition, deltaTime);
    }
    mainView.setCenter(warriorPosition);
    minimap.update(towerView, towerSprite, monsters, warrior);
    minimap.setPosition(mainView.getCenter().x - mainView.getSize().x / 2 + 3,
        mainView.getCenter().y - mainView.getSize().y / 2 + 3);

    Timer.update(mainView.getCenter().x+20, mainView.getCenter().y - 550);
}

void Game::render() {
    window.clear();
    window.setView(mainView);
    window.draw(towerSprite);
    for (auto& monster : monsters) {
        monster.draw(window);
    }
    warrior.draw(window);

    // 미니맵 그리기
    minimap.draw(window);

    Timer.draw(window);

    window.display();
}

void Game::spawnMonster() {
    float x = static_cast<float>(std::rand() % window.getSize().x);
    float y = static_cast<float>(std::rand() % window.getSize().y);
    monsters.emplace_back(x, y, monsterSpeed);
}
