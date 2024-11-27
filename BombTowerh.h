#pragma once
#ifndef BOMB_TOWER_H
#define BOMB_TOWER_H

#include "SubTower.h"
#include "Projectile.h"
#include <vector>
#include <SFML/Graphics.hpp> // SFML 관련 라이브러리

class BombTower : public SubTower {
private:
    std::vector<Projectile> bombProjectiles;
    sf::Texture projectileTexture; // 기본 텍스처

    float explosionRadius; //범위반경
public:
    BombTower(sf::Vector2f position)
        : SubTower(position, 300.0f, 0.5f, 30.0f), explosionRadius(100.0f) {
        texture.loadFromFile("BombTower.png");
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        projectileTexture.loadFromFile("Bomb.png"); // 기본 텍스처
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
            for (auto& monster : monsters) {
                if (isInRange(monster->getPosition())) {
                    bombProjectiles.emplace_back(projectileTexture, sprite.getPosition(), monster->getPosition(), 200.0f, attackDamage);
                    attackClock.restart();
                    break;
                }
            }
        }
        updateProjectiles(deltaTime, monsters);
    }

    void updateProjectiles(float deltaTime, std::vector<std::unique_ptr<Monster>>& monsters) {
        for (auto it = bombProjectiles.begin(); it != bombProjectiles.end();) {
            it->update(deltaTime);

            // 충돌 검사
            bool collided = false;
            for (auto& monster : monsters) {
                if (it->checkCollision(*monster)) {
                    // 충돌한 지점의 위치를 별도로 저장
                    sf::Vector2f collisionPosition = it->getPosition();
                    // 범위 피해 적용
                    applyAreaDamage(collisionPosition, monsters);
                    collided = true;
                    break;
                }
            }

            // 충돌했거나 화면 밖으로 나가면 투사체 제거
            if (collided || it->isOutofBound()) {
                it = bombProjectiles.erase(it);
            }
            else {
                ++it;
            }
        }
    }


    void applyAreaDamage(const sf::Vector2f& explosionCenter, std::vector<std::unique_ptr<Monster>>& monsters) {
        for (auto& monster : monsters) {
            // 몬스터와 폭발 중심 사이의 거리 계산
            sf::Vector2f monsterPos = monster->getPosition();
            float distance = std::sqrt(std::pow(monsterPos.x - explosionCenter.x, 2) +
                std::pow(monsterPos.y - explosionCenter.y, 2));

            // 폭발 반경 내의 몬스터에게 피해 적용
            if (distance <= explosionRadius) {
                monster->takeDamage(attackDamage);
            }
        }
    }


    void draw(sf::RenderTarget& target) override {
        target.draw(sprite);
        for (auto& projectile : bombProjectiles) {
            projectile.draw(target);
        }
    }
};

#endif // BOMB_TOWER_H