#include "BombProjectile.h"
#include <cmath>
#include <iostream>

BombProjectile::BombProjectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f targetPosition, float speed, float damage)
    : Projectile(texture, position, targetPosition, speed, damage) {
    // 필요하면 초기화 추가
}

void BombProjectile::update(float deltaTime) {
    if (!exploded) {
        position += velocity * deltaTime;
        sprite.setPosition(position);
    }
    else {
        // 폭발 후 지속 시간 확인
        if (explosionTimer.getElapsedTime().asSeconds() > explosionDuration) {
            toBeDestroyed = true;
        }
    }
}

void BombProjectile::draw(sf::RenderTarget& target) {
    target.draw(sprite);
    // 폭발 효과 추가 가능
}

bool BombProjectile::checkCollision(Monster& monster) {
    // 투사체와 몬스터 중심 간 거리 계산
    sf::Vector2f monsterPosition = monster.getPosition(); // 몬스터 위치
    float monsterRadius = 50;            // 몬스터 크기 (반지름 기준)

    float distance = std::sqrt(
        std::pow(sprite.getPosition().x - monsterPosition.x, 2) +
        std::pow(sprite.getPosition().y - monsterPosition.y, 2)
    );

    // 충돌 조건: 거리 <= 몬스터 반지름 + 투사체 반지름
    float projectileRadius = sprite.getGlobalBounds().width / 2.0f; // 투사체 반지름
    if (distance <= monsterRadius + projectileRadius) {
        triggerExplosion();
        return true;
    }
    return false;
}


void BombProjectile::triggerExplosion() {
    exploded = true;
    explosionTimer.restart();
    std::cout << "BombProjectile exploded!" << std::endl;
    // 폭발 효과 로직 추가 가능 (범위 내 몬스터 데미지 등)
}
