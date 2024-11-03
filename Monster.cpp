#include "Monster.h"
#include <cmath>

// 생성자
Monster::Monster(float x, float y, float speed)
    : movementSpeed(speed),damageTaken(0.0f), isTakingDamage(false), damageDisplayDuration(0.3f), damageDisplayTime(0.0f) {
    shape.setSize(sf::Vector2f(30.0f, 30.0f));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(x, y);
	healthPoint = 100.0f;
}

// update 함수 구현
void Monster::update(sf::Vector2f targetPosition, float deltaTime) {
    sf::Vector2f direction = targetPosition - shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (isTakingDamage) {
        damageDisplayTime += deltaTime;
        if (damageDisplayTime >= damageDisplayDuration) {
            isTakingDamage = false; // 피해 표시 종료
            damageTaken = 0.0f; // 피해량 초기화
        }
    }
    if (length != 0) {
        direction /= length;  // 방향 정규화
        shape.move(direction * movementSpeed * deltaTime);  // 몬스터 이동
    }
}

// draw 함수 구현
void Monster::draw(sf::RenderTarget& target)const {
    target.draw(shape);
    if (isTakingDamage) {
        sf::Font font;
        font.loadFromFile("arial.ttf"); // 폰트 로드 (폰트 파일이 필요합니다)
        sf::Text damageText;
        damageText.setFont(font);
        damageText.setString(std::to_string(static_cast<int>(damageTaken))); // 피해량을 문자열로 변환
        damageText.setCharacterSize(12);
        damageText.setFillColor(sf::Color::White);
        damageText.setPosition(shape.getPosition().x, shape.getPosition().y - 30); // 위치 조정

        target.draw(damageText); // 피해량 텍스트 그리기
    }
}

// getPosition 함수 구현
sf::Vector2f Monster::getPosition() {
    return shape.getPosition();
}
// 특정 위치 근처에 있는지 확인
bool Monster::isNear(sf::Vector2f position, float radius) const {
    sf::Vector2f distanceVec = position - shape.getPosition();
    float distance = std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);
    return distance <= radius;
}
void Monster::takeDamage(float attackDamage) {
    healthPoint -= attackDamage;
    if (attackDamage>0) {
        // 피해량 저장
        damageTaken = attackDamage;
        isTakingDamage = true;
        damageDisplayTime = 0.0f; // 초기화

    }
}
float Monster:: getHealthPoint()const {
    return healthPoint;
}