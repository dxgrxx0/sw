#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Character.h"
#include "Monster.h"

int main() {
    // 창 설정
    sf::RenderWindow window(sf::VideoMode(1600, 1400), "Warrior and Monsters");

    // 캐릭터 생성
    Character warrior("knightbg.png", 400, 300, 2.0f, 100.0f);
    sf::RectangleShape tower(sf::Vector2f(100.0f, 100.0f));
    tower.setFillColor(sf::Color::Red);
    tower.setPosition(window.getSize().x / 2 - tower.getSize().x / 2, window.getSize().y / 2 - tower.getSize().y / 2);

    // 타이머 및 몬스터 관리
    sf::Clock clock;
    sf::Clock spawnClock;
    std::vector<Monster> monsters;
    float spawnInterval = 1.0f;
    float monsterSpeed = 50.0f;
    float detectionRadius = 150.0f;

    std::srand(static_cast<unsigned int>(std::time(0)));

    // 게임 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // 몬스터 스폰 로직
        if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
            float x = static_cast<float>(std::rand() % window.getSize().x);
            float y = static_cast<float>(std::rand() % window.getSize().y);

            sf::Vector2f towerPosition = tower.getPosition();
            if (std::sqrt(std::pow(x - towerPosition.x, 2) + std::pow(y - towerPosition.y, 2)) > 150.0f) {
                monsters.emplace_back(x, y, monsterSpeed);
                spawnClock.restart();
            }
        }

        // 입력 처리
        warrior.handleInput(deltaTime);

        // 애니메이션 업데이트
        warrior.updateAnimation(deltaTime);

        // 몬스터 업데이트
        sf::Vector2f warriorPosition = warrior.getPosition();
        for (auto& monster : monsters) {
            monster.update(warriorPosition, deltaTime);
        }

        // 화면 그리기
        window.clear();
        window.draw(tower);
        warrior.draw(window);
        for (auto& monster : monsters) {
            monster.draw(window);
        }
        window.display();
    }

    return 0;
}
