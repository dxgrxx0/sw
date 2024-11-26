#ifndef BOMB_PROJECTILE_H
#define BOMB_PROJECTILE_H

#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

class BombProjectile : public Projectile {
private:
    bool exploded = false;
    sf::Clock explosionTimer;
    float explosionRadius = 100.0f; // 폭발 반경
    float explosionDuration = 1.0f; // 폭발 지속 시간

public:
    BombProjectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f targetPosition, float speed, float damage);

    void update(float deltaTime) override; // Projectile의 update 재정의
    void draw(sf::RenderTarget& target) override;
    bool checkCollision(Monster& monster) override;

    void triggerExplosion(); // 폭발 상태로 전환
};

#endif // BOMB_PROJECTILE_H
