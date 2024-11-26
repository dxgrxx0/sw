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

public:
    BombTower(sf::Vector2f position)
        : SubTower(position, 300.0f, 0.5f, 30.0f) {
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

            // 몬스터와의 충돌 검사
            bool collided = false;
            for (auto& monster : monsters) {
                if (it->checkCollision(*monster)) {
                    monster->takeDamage(attackDamage); // 몬스터에게 피해 적용
                    collided = true;
                    break;
                }
            }

            if (collided) {
                it = bombProjectiles.erase(it); // 충돌 시 투사체 제거
            }
            else {
                ++it;
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
