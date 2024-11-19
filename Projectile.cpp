#include "Projectile.h"
#include "Utility.h"
#include <cmath>

Projectile::Projectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f targetPosition, float speed, float damage)
    : position(position), speed(speed), damage(damage) {
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f); // 필요에 따라 크기 조정

    // 방향 벡터 계산
    sf::Vector2f direction = targetPosition - position;
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (magnitude != 0) {
        velocity = direction / magnitude * speed;
    }
    else {
        velocity = sf::Vector2f(0, 0);
    }

    // 투사체 회전 (몬스터 방향으로)
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;
    sprite.setRotation(angle+45); // 90도 보정 (위쪽 방향이 0도인 경우)
}

void Projectile::update(float deltaTime) {
    position += velocity * deltaTime;
    sprite.setPosition(position);
}

void Projectile::draw(sf::RenderTarget& target) {
    target.draw(sprite);
}

bool Projectile::checkCollision(Monster& monster) {
    // 간단한 충돌 검사 (바운딩 박스 사용)
    if (calculateDistance(sprite.getPosition(),monster.getPosition())<50) {
        monster.takeDamage(damage);
        toBeDestroyed = true;
        return true;
    }
    return false;
}
