#ifndef BOMB_TOWER_H
#define BOMB_TOWER_H
#include "SubTower.h"
#include "Projectile.h"
#include <vector>

// 폭발 효과를 위한 간단한 구조체
struct ExplosionEffect {
    sf::Sprite sprite;
    sf::Clock timer;
    float duration;

    ExplosionEffect(const sf::Texture& texture, sf::Vector2f pos, float explosionDuration) {
        sprite.setTexture(texture);
        sprite.setPosition(pos);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        sprite.setScale(0.3f, 0.3f);
        duration = explosionDuration;
        timer.restart();
    }

    bool isExpired() const {
        return timer.getElapsedTime().asSeconds() >= duration;
    }
};

class BombTower : public SubTower {
private:
    std::vector<Projectile> projectiles;
    sf::Texture projectileTexture;
    sf::Texture explosionTexture;
    std::vector<ExplosionEffect> activeExplosions;

    float splashRadius = 100.0f;
    float explosionDuration = 0.2f;

public:
    BombTower(sf::Vector2f position)
        : SubTower(position, 400.0f, 1.5f, 15.0f) {
        texture.loadFromFile("BombTower.png");
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);

        projectileTexture.loadFromFile("Bomb.png");

        explosionTexture.loadFromFile("Bomb_explosion.png");
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
            for (auto& monster : monsters) {
                if (isInRange(monster->getPosition())) {
                    // 타겟 몬스터의 위치로 투사체 발사

                    projectiles.emplace_back(projectileTexture, position, monster->getPosition(), 200.0f, attackDamage);
                    projectiles.back().setScale(0.1f, 0.1f); //투사체 크기 축소
                    attackClock.restart();
                    break;
                }
            }
        }
        updateProjectiles(deltaTime, monsters);
    }

    void updateProjectiles(float deltaTime, std::vector<std::unique_ptr<Monster>>& monsters) {
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            it->update(deltaTime);
            bool projectileDestroyed = false;

            if (it->isOutofBound()) {
                it = projectiles.erase(it);
                continue;
            }

            for (auto& monster : monsters) {
                if (it->checkCollision(*monster)) {
                    // 충돌한 몬스터의 위치를 폭발 중심으로 사용
                    sf::Vector2f explosionCenter = monster->getPosition();

                    // 스플래시 데미지 적용
                    for (auto& splashMonster : monsters) {
                        float distanceToExplosion = calculateDistance(explosionCenter, splashMonster->getPosition());
                        if (distanceToExplosion <= splashRadius) {
                            float damageMultiplier = 1.0f - (distanceToExplosion / splashRadius);
                            float splashDamage = attackDamage * damageMultiplier;
                            splashMonster->takeDamage(splashDamage);
                        }
                    }

                    // 폭발 효과 추가 (몬스터 위치)
                    activeExplosions.emplace_back(explosionTexture, explosionCenter, explosionDuration);

                    projectileDestroyed = true;
                    break;
                }
            }

            if (projectileDestroyed) {
                it = projectiles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void draw(sf::RenderTarget& target) override {
        target.draw(sprite);

        // 투사체 그리기
        for (auto& projectile : projectiles) {
            projectile.draw(target);
        }

        // 폭발 효과 그리기
        for (auto& explosion : activeExplosions) {
            float elapsedTime = explosion.timer.getElapsedTime().asSeconds();
            float alpha = std::max(0.0f, 1.0f - (elapsedTime / explosionDuration));
            explosion.sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));
            target.draw(explosion.sprite);
        }

        // 만료된 폭발 효과 제거
        activeExplosions.erase(
            std::remove_if(activeExplosions.begin(), activeExplosions.end(),
                [](const ExplosionEffect& explosion) { return explosion.isExpired(); }),
            activeExplosions.end()
        );
    }
};
#endif