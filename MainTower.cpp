#include "MainTower.h"
//#include "SubTower.h"
#include "Character.h"
MainTower::MainTower(const sf::Vector2f& position)
    : healAmountPerSecond(7.0f),health(800.f),position(position),defense(0),maxHealth(800.0f) {
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
    if (calculateDistance(sprite.getPosition(), character.getPosition()) < 200.0f) {
        character.heal(healAmountPerSecond * deltaTime);
    }
    if(health<maxHealth)health += healAmountPerSecond/3.0f * deltaTime;
}
const sf::Sprite& MainTower::getSprite() const {
    return sprite; // 스프라이트 반환
}

void MainTower::upgrade() {
	maxHealth += 400.0f;
    healAmountPerSecond += 3.0f;
	health += 400.0f;
}
sf::Vector2f MainTower::getPosition() {
    return sprite.getPosition();
}
void MainTower::takeDamage(float attackPower) {
    float damageAmount = attackPower * (100 - defense) / 100;
    health -= damageAmount;
    if (health < 0)health = 0;
}
float MainTower::getHealth() {
    return health;
}
float MainTower::getMaxHealth() {
    return maxHealth;
}