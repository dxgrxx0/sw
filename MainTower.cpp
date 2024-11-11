#include "MainTower.h"
//#include "SubTower.h"
#include "Character.h"
#include <iostream>
MainTower::MainTower(const sf::Vector2f& position)
    : healAmountPerSecond(5.0f),health(200.f),position(position) {
    if (!texture.loadFromFile("tower.png")) {
        throw std::runtime_error("Failed to load tower texture");
    }
    sprite.setTexture(texture); // 스프라이트에 텍스처 적용
    sprite.setPosition(position); // 타워 위치 설정
    sprite.setOrigin(sprite.getGlobalBounds().width /2,sprite.getGlobalBounds().height / 2); // 원점을 중앙으로 설정
}
void MainTower::draw(sf::RenderWindow& window) const {
    window.draw(sprite); // 타워 스프라이트 그리기
}
void MainTower::healNearbyCharacter(float deltaTime, Character& character) {
    // 일정 범위 내에 있는지 확인 후 캐릭터 회복
    if (calculateDistance(sprite.getPosition(), character.getPosition()) < 100.0f) {
        character.heal(healAmountPerSecond * deltaTime);
		std::cout << "heal" << std::endl;
    }
}
const sf::Sprite& MainTower::getSprite() const {
    return sprite; // 스프라이트 반환
}
/*
void MainTower::upgrade(std::vector<SubTower>& subTowers) {
    if (level < 5) {
        level++;
        healAmountPerSecond += 2.0f; // 회복 속도 증가
        for (auto& subTower : subTowers) {
            //subTower.increaseAttack(5.0f); // 서브 타워 공격력 강화
        }
    }
}*/
sf::Vector2f MainTower::getPosition() {
    return sprite.getPosition();
}