#include "SubTower.h"
#include <cmath>

SubTower::SubTower(sf::Vector2f position, float range, float attackSpeed, float attackDamage)
    : position(position), range(range), attackSpeed(attackSpeed), attackDamage(attackDamage) {
    rangeIndicator.setRadius(range);
    rangeIndicator.setFillColor(sf::Color(0, 0, 0, 50)); // 투명한 범위 표시
    rangeIndicator.setOutlineColor(sf::Color::White);
    rangeIndicator.setOutlineThickness(1);
    rangeIndicator.setPosition(position.x - range, position.y - range);
}

bool SubTower::isInRange(const sf::Vector2f& targetPosition) const {
    float dx = targetPosition.x - position.x;
    float dy = targetPosition.y - position.y;
    return std::sqrt(dx * dx + dy * dy) <= range;
}

void SubTower::attack(std::vector<std::unique_ptr<Monster>>& monsters,float deltaTime) {
    if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
        for (auto& monster : monsters) {
            if (isInRange(monster->getPosition())) {
                monster->takeDamage(attackDamage); // 몬스터에 피해 적용
                attackClock.restart();
                break;
            }
        }
    }
}

void SubTower::draw(sf::RenderTarget& target) {
    sf::RectangleShape towerShape(sf::Vector2f(20, 20));
    towerShape.setFillColor(sf::Color::Blue);
    towerShape.setPosition(position.x - 10, position.y - 10);
    target.draw(towerShape);

    // 범위 표시
    target.draw(rangeIndicator);
}

sf::Vector2f SubTower::getPosition() const {
    return position;
}
