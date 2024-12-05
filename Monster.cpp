#include "Monster.h"
#include <cmath>
#include "Character.h"
#include "MainTower.h"
#include <iostream>
// 생성자
Monster::Monster(float x, float y, float speed, MonsterType type)
    : movementSpeed(speed), damageTaken(0.0f), isTakingDamage(false), damageDisplayDuration(0.3f), damageDisplayTime(0.0f), attackPower(0), defense(0)
    , attackRange(50), monsterType(type)
{
    font = ResourceManager::getInstance().getFont("Arial");
    damageText.setFont(font);
    healthPoint = 100.0f;
    switch (type) {
    case MonsterType::Speed:  //이속 3배,체력 1/2배
        textureName = ("SpeedMonster");
        movementSpeed = 150.0f;
        healthPoint = 50.0f;
        attackPower = 10.0f;
        defense = 10.0f;
        break;
    case MonsterType::Attack: //공격력 3배
        textureName = ("AttackMonster");
        movementSpeed = 100.0f;
        healthPoint = 100.0f;
        defense = 10.0f;
        attackPower = 30.0f; // 추가 공격력

        break;
    case MonsterType::Defense: //방어력3배,체력2배
        textureName = ("DefenseMonster");
        movementSpeed = 50.0f;
        healthPoint = 200.0f;
        attackPower = 10.0f;
        defense = 30.0f; // 방어력 추가

        break;
    case MonsterType::Basic:
		textureName = ("BasicMonster");
		movementSpeed = 100.0f;
		healthPoint = 100.0f;
		attackPower = 10.0f;
		defense = 10.0f;
		break;
    default:
        break;

    }
    if (type != MonsterType::Mid_Boss && type != MonsterType::Main_Boss) {
        texture = ResourceManager::getInstance().getTexture(textureName);
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(x, y);
        sprite.setOrigin(500, 500);
    }
}


// update 함수 구현
void Monster::update(const sf::Vector2f& heroinePosition, const sf::Vector2f& towerPosition, float deltaTime, Character& character, MainTower& mainTower) {
    sf::Vector2f targetPosition;
    float distanceToHeroine = calculateDistance(sprite.getPosition(), heroinePosition);
    float distanceToTower = calculateDistance(sprite.getPosition(), towerPosition);

    if (distanceToHeroine < distanceToTower) {
        targetPosition = heroinePosition;
    }
    else {
        targetPosition = towerPosition;
    }

    sf::Vector2f direction = targetPosition - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    // 특정 거리 이내로 접근했을 때 피해를 입힘
    if (length <= attackRange) {
        if (attackTimer.getElapsedTime().asSeconds() >= attackCooldown) {
            if (targetPosition == towerPosition) {
                mainTower.takeDamage(attackPower);
                //printf("Tower takeDamage!");
            }
            else {
                character.takeDamage(attackPower);
                //printf("character takeDamage!");
            } // 캐릭터에 피해 입힘
            attackTimer.restart(); // 타이머 초기화
        }
    }
    else if (length >= attackRange) {
        direction /= length; // 방향 정규화
        sprite.move(direction * movementSpeed * deltaTime); // 몬스터 이동
    }
    if (isTakingDamage) {
        damageDisplayTime += deltaTime;
		sprite.setColor(sf::Color::Red); // 빨간색으로 변경
        if (damageDisplayTime >= damageDisplayDuration) {
            isTakingDamage = false; // 피해 표시 종료
            damageTaken = 0.0f; // 피해량 초기화
			sprite.setColor(sf::Color::White); // 색상 원래대로 변경
        }
    }
}

// draw 함수 구현
void Monster::draw(sf::RenderTarget& target) {
    target.draw(sprite);

    if (isTakingDamage) {

        damageText.setString(std::to_string(static_cast<int>(damageTaken))); // 피해량을 문자열로 변환
        damageText.setCharacterSize(30);
        damageText.setFillColor(sf::Color::White);
        damageText.setPosition(sprite.getPosition().x, sprite.getPosition().y - 70); // 위치 조정
        target.draw(damageText); // 피해량 텍스트 그리기
    }

}

// getPosition 함수 구현
sf::Vector2f Monster::getPosition() {
    return sprite.getPosition();
}

// 특정 위치 근처에 있는지 확인
bool Monster::isNear(sf::Vector2f position, float radius) const {
    sf::Vector2f distanceVec = position - sprite.getPosition();
    float distance = std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);
    return distance <= radius;
}

void Monster::takeDamage(float attackDamage) {
    attackDamage = attackDamage * (100 - defense) / 100;
    healthPoint -= attackDamage;
    if (attackDamage > 0) {
        // 피해량 저장
        damageTaken = attackDamage;
        isTakingDamage = true;
        damageDisplayTime = 0.0f; // 초기화

    }
}
float Monster::getHealthPoint()const {
    return healthPoint;
}