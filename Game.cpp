#include "Game.h"
#include <cstdlib>
#include <ctime>

Game::Game() :
    window(sf::VideoMode(1600, 1000), "Warrior and Monsters"),
    warrior("knightbg.png", 700, 700, 3.0f, 100.0f),
    minimap(600, 600, 0.4f),
    mainView(sf::FloatRect(0, 0, 1600, 1000)),
    towerView(sf::FloatRect(500, 400, 600, 600)),
    spawnInterval(1.0f),
    monsterSpeed(50.0f),
    mainTower(sf::Vector2f(650.0f, 500.0f)) // MainTower 위치 설정
{
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


    if (warrior.getIsSwinging() && !warrior.getAttackApplied()) {
        warrior.basicAttack(monsters);

        // 체력이 0 이하인 몬스터들을 한 번에 제거
        monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
            [](const Monster& monster) {
                return monster.getHealthPoint() <= 0;
            }),
            monsters.end());

        warrior.setAttackApplied(true);  // 공격 적용 완료 플래그 설정
    }



    sf::Vector2f warriorPosition = warrior.getPosition();
    for (auto& monster : monsters) {
        monster.update(warriorPosition, deltaTime);
    }
    mainView.setCenter(warriorPosition);

    minimap.update(towerView, mainTower, monsters, warrior);

    minimap.setPosition(mainView.getCenter().x - mainView.getSize().x / 2 + 3,
        mainView.getCenter().y - mainView.getSize().y / 2 + 3);

}

void Game::render() {
    window.clear();
    window.setView(mainView);
    window.draw(towerSprite);

    mainTower.draw(window);

    for (auto& monster : monsters) {
        monster.draw(window);
    }
    warrior.draw(window);

    // 미니맵 그리기
    minimap.draw(window);


    window.display();
}

void Game::spawnMonster() {
    float x = static_cast<float>(std::rand() % window.getSize().x);
    float y = static_cast<float>(std::rand() % window.getSize().y);
    monsters.emplace_back(x, y, monsterSpeed);
}
