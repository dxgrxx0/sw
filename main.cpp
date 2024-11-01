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

    // 메인 카메라 뷰 설정
    sf::View mainView(sf::FloatRect(0, 0, 1600, 1400));

    // 캐릭터 생성
    Character warrior("knightbg.png", 400, 300, 2.0f, 100.0f);
    sf::RectangleShape tower(sf::Vector2f(100.0f, 100.0f));
    sf::Texture mainTower;
    if (!mainTower.loadFromFile("tower.PNG")) {
        return -1; // 이미지 로드 실패
    }

    // 스프라이트 객체 생성
    sf::Sprite towerSprite;
    towerSprite.setTexture(mainTower);
    towerSprite.setPosition(800 - 50, 700 - 50); // 중앙 위치 설정

    //미니맵용
    // CCTV 뷰 설정
    sf::View towerView(sf::FloatRect(600, 500, 600, 600));

    sf::RenderTexture minimapTexture;
    minimapTexture.create(600, 600);

    // 타이머 및 몬스터 관리
    sf::Clock clock;
    sf::Clock spawnClock;
    std::vector<Monster> monsters;
    float spawnInterval = 1.0f;
    float monsterSpeed = 50.0f;

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
            monsters.emplace_back(x, y, monsterSpeed);
            spawnClock.restart();
        }

        // 입력 처리 및 애니메이션 업데이트
        warrior.handleInput(deltaTime);
        warrior.updateAnimation(deltaTime);

        // 몬스터 업데이트
        sf::Vector2f warriorPosition = warrior.getPosition();
        for (auto& monster : monsters) {
            monster.update(warriorPosition, deltaTime);
        }

        // 메인 뷰 렌더링
        window.clear();
        mainView.setCenter(warriorPosition);

        window.setView(mainView);
        window.draw(towerSprite);
        for (auto& monster : monsters) {
            monster.draw(window);
        }
        warrior.draw(window);

		// 미니맵 렌더링
        minimapTexture.clear();
        minimapTexture.setView(towerView);
        minimapTexture.draw(towerSprite);
        for (auto& monster : monsters) {
            if (monster.isNear(sf::Vector2f(750,650), 500)) { // 타워 주변의 몬스터만 그리기
                monster.draw(minimapTexture);
            }
        }
        warrior.draw(minimapTexture);
        minimapTexture.display();
        // RenderTexture를 미니맵 스프라이트로 변환 후 화면에 축소하여 표시
        sf::Sprite minimapSprite(minimapTexture.getTexture());
        minimapSprite.setScale(0.2f, 0.2f);  // 미니맵 크기로 축소
        minimapSprite.setPosition(mainView.getCenter().x - 800, mainView.getCenter().y -700);  // 메인 뷰 좌측 하단에 위치

        window.setView(mainView);  // 메인 뷰로 돌아와서 미니맵 그리기
        window.draw(minimapSprite);
        window.display();
    }

    return 0;
}