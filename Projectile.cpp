#include "Projectile.h"
#include <cmath>
#include "Utility.h"

Projectile::Projectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f targetPosition, float speed, float damage)
    : position(position), speed(speed), damage(damage) {
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f);

    sf::Vector2f direction = targetPosition - position;
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0) {
        velocity = direction / magnitude * speed;
    }
    else {
        velocity = sf::Vector2f(0, 0);
    }

    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;
    sprite.setRotation(angle + 45); // 90도 보정
}

void Projectile::update(float deltaTime) {
    position += velocity * deltaTime;
    sprite.setPosition(position);
}

void Projectile::draw(sf::RenderTarget& target) {
    target.draw(sprite);
}

bool Projectile::checkCollision(Monster& monster) {
    // 몬스터의 중심 위치와 크기 가져오기
    sf::Vector2f monsterPosition = monster.getPosition();
    float monsterRadius = 50; // 몬스터 반지름

    // 투사체의 중심 위치와 반지름 계산
    sf::Vector2f projectilePosition = sprite.getPosition();
    float projectileRadius = sprite.getGlobalBounds().width / 2.0f; // 투사체의 반지름

    // 두 개체 간 거리 계산
    float distance = std::sqrt(
        std::pow(projectilePosition.x - monsterPosition.x, 2) +
        std::pow(projectilePosition.y - monsterPosition.y, 2)
    );

    // 충돌 여부 판단 (거리 <= 두 개체 반지름의 합)
    if (distance <= monsterRadius + projectileRadius) {
        monster.takeDamage(damage); // 몬스터에 데미지 적용
        toBeDestroyed = true;      // 투사체를 제거할 준비
        return true;
    }
    return false;
}

bool Projectile::isOutofBound() {
    if (position.x < -1000 || position.x > 3600 || position.y < -1000 || position.y > 2000) {
        toBeDestroyed = true;
        return true;
    }
    return false;
}
