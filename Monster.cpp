#include "Monster.h"
#include <cmath>

// 생성자
Monster::Monster(float x, float y, float speed)
    : movementSpeed(speed) {
    shape.setSize(sf::Vector2f(30.0f, 30.0f));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(x, y);
}

// update 함수 구현
void Monster::update(sf::Vector2f targetPosition, float deltaTime) {
    sf::Vector2f direction = targetPosition - shape.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) {
        direction /= length;  // 방향 정규화
        shape.move(direction * movementSpeed * deltaTime);  // 몬스터 이동
    }
}

// draw 함수 구현
void Monster::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// getPosition 함수 구현
sf::Vector2f Monster::getPosition() {
    return shape.getPosition();
}
