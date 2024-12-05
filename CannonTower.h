#pragma once
#ifndef CANNON_TOWER_H
#define CANNON_TOWER_H

#include "SubTower.h"
#include "Projectile.h"
#include <vector>

class CannonTower : public SubTower {
private:
    std::vector<Projectile> projectiles;
    sf::Texture projectileTexture; // 투사체에 사용할 텍스처
    Monster* target;

public:
    CannonTower(sf::Vector2f position)
        : SubTower(position, 500.0f, 1.0f, 30.0f) {
        texture.loadFromFile("cannontower.png");
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        projectileTexture.loadFromFile("cannon.png");
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
            for (auto& monster : monsters) {
                if (isInRange(monster->getPosition())) {
                    projectiles.emplace_back(projectileTexture, position, monster->getPosition(), 400.0f, attackDamage);
                    //monster->takeDamage(attackDamage);
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
			it->sprite.rotate(deltaTime*360.0f); // 투사체 회전
            // 몬스터와의 충돌 검사
            bool collided = false;
            for (auto& monster : monsters) {
                if (it->checkCollision(*monster)) {
                    collided = true;
                    break;
                }
            }

            // 충돌하거나 화면 밖으로 나간 투사체 제거
            if (collided || it->isToBeDestroyed() || it->isOutofBound()) {
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
    }
};

#endif // ARROW_TOWER_H