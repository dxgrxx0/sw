#pragma once
#ifndef WIZARD_TOWER_H
#define WIZARD_TOWER_H

#include "SubTower.h"
#include "Projectile.h"
#include <vector>
#include"Utility.h"

class WizardTower : public SubTower {
private:
    std::vector<Projectile> projectiles;
    sf::Texture projectileTexture;
    float splashRadius; // 범위 공격 반경

    // AOE 범위 표시를 위한 원
    sf::CircleShape aoeIndicator;
    bool showingAOE;
    sf::Vector2f currentAOEPosition;
    sf::Vector2f towerCenter;
public:
    WizardTower(sf::Vector2f position)
        : SubTower(position, 400.0f, 1.5f, 15.0f), // 사거리, 공격속도, 공격력 조정
        splashRadius(100.0f),showingAOE(false), towerCenter(1220,220) // 스플래시 범위 설정
    {
        texture.loadFromFile("wizardtower.png");
        sprite.setTexture(texture);
        sprite.setPosition(towerCenter);

        sprite.setScale(0.3f, 0.3f);

        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        projectileTexture.loadFromFile("MagicBall.png");
        sf::Vector2u textureSize = projectileTexture.getSize();
        sprite.setPosition(position);
        sprite.setOrigin(textureSize.x / 2.f, textureSize.y / 2.f);
        // AOE 표시기 설정
        aoeIndicator.setRadius(splashRadius);
        aoeIndicator.setOrigin(splashRadius, splashRadius);
        aoeIndicator.setFillColor(sf::Color(255, 0, 255, 64));  // 반투명 보라색
        aoeIndicator.setOutlineColor(sf::Color(255, 0, 255, 128));
        aoeIndicator.setOutlineThickness(2.0f);
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
            // 가장 가까운 몬스터 찾기
            float minDistance = range;
            Monster* targetMonster = nullptr;

            for (auto& monster : monsters) {
                float distance = calculateDistance(position, monster->getPosition());
                if (distance < minDistance) {
                    minDistance = distance;
                    targetMonster = monster.get();
                }
            }

            // 타겟이 있으면 공격
            if (targetMonster) {
                showingAOE = true;
                currentAOEPosition = targetMonster->getPosition();
                aoeIndicator.setPosition(currentAOEPosition);

                projectiles.emplace_back(projectileTexture, position, targetMonster->getPosition(), 300.0f, attackDamage);
                projectiles.back().setScale(0.2f, 0.2f);  // 투사체 크기를 0.2배로 축소
                attackClock.restart();
            }
        }
        updateProjectiles(deltaTime, monsters);
    }

    void updateProjectiles(float deltaTime, std::vector<std::unique_ptr<Monster>>& monsters) {
            for (auto it = projectiles.begin(); it != projectiles.end();) {
            it->update(deltaTime);
            bool projectileDestroyed = false;

            // 몬스터와의 충돌 검사
            for (auto& monster : monsters) {
                if (it->checkCollision(*monster)) {
                    // 스플래시 데미지 적용
                    for (auto& splashMonster : monsters) {
                        float distanceToExplosion = calculateDistance(it->getPosition(), splashMonster->getPosition());
                        if (distanceToExplosion <= splashRadius) {
                            float damageMultiplier = 1.0f - (distanceToExplosion / splashRadius);
                            float splashDamage = attackDamage * damageMultiplier;
                            splashMonster->takeDamage(splashDamage);
                        }
                    }
                    showingAOE = false;  // 데미지 적용 후 범위 표시 제거
                    projectileDestroyed = true;
                    break;
                }
            }

            if (projectileDestroyed || it->isDestroyed()) {
                it = projectiles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void draw(sf::RenderTarget& target) override {
        // AOE 범위 먼저 그리기 (투사체 아래에 표시)
        if (showingAOE) {
            target.draw(aoeIndicator);
        }

        target.draw(sprite);

        for (auto& projectile : projectiles) {
            projectile.draw(target);
        }
    }
};

#endif // WIZARD_TOWER_H