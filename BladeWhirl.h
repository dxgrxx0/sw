#ifndef BLADE_WHIRL_H
#define BLADE_WHIRL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "BaseSkill.h"
#include "Character.h"
#include "Monster.h"

class BladeWhirl : public BaseSkill {
private:
    Character* character;
    std::vector<std::unique_ptr<Monster>>& monsters;
    float range;
    float damage;
    float elapsedTime;

    float currentAngle;
    float startAngle;
    float endAngle;
    float swingSpeed;
    float handleFixed;
    float lastDamageAngle;

    sf::Texture swordTexture;
    sf::Sprite swordSprite;

public:
    BladeWhirl(Character* character, std::vector<std::unique_ptr<Monster>>& monsters)
        : BaseSkill("Blade Whirl", sf::Keyboard::Q, 2.0f),
        character(character),
        monsters(monsters),
        range(330.0f),
        damage(44.0f),
        elapsedTime(0.0f),
        currentAngle(0.0f),
        startAngle(0.0f),
        endAngle(360.0f),
        swingSpeed(540.0f),
        handleFixed(20.0f),
        lastDamageAngle(-45.0f) {
        if (!swordTexture.loadFromFile("QSlash.png")) {
            // 이미지 로드 실패 처리
        }
        swordSprite.setTexture(swordTexture);
        //swordSprite.setScale(0.3f, 0.3f);
    }

    void applyEffect() override {
        
        elapsedTime = 0.0f;
        currentAngle = startAngle;
        lastDamageAngle = startAngle;
        std::cout << "Blade Whirl activated!" << std::endl;
        applyDamageInArc();
        
        /*else {
            std::cout << "Blade Whirl is on cooldown! Time remaining: "
                << cooldown - elapsedTime << "s" << std::endl;
        }*/
    }

    void update(float deltaTime) override {
        elapsedTime += deltaTime;
        cooldownTime += deltaTime;
        if (isActive) {
            if (currentAngle >= endAngle) {
                isActive = false;
            }
            else {
                currentAngle += swingSpeed * deltaTime;
                updateWeaponPosition();

                float damageCheckInterval = 45.0f;
                if (currentAngle - lastDamageAngle >= damageCheckInterval) {
                    applyDamageInArc();
                    lastDamageAngle = currentAngle;
                }
            }
        }
    }

    void draw(sf::RenderTarget& target) {
        if (isActive) {
            target.draw(swordSprite);
        }
    }
    void upgrade() override {
		damage += 20.0f;
		cooldown -= 0.2f;
    }

private:
    void updateWeaponPosition() {
        sf::Vector2f characterPos = character->getPosition();
        float angleRad = currentAngle * 3.14159f / 180.0f;

        float handleX = characterPos.x + std::cos(angleRad) * handleFixed;
        float handleY = characterPos.y + std::sin(angleRad) * handleFixed;

        swordSprite.setPosition(handleX, handleY);
        swordSprite.setRotation(currentAngle);
    }

    void applyDamageInArc() {
        sf::Vector2f bladePos = swordSprite.getPosition();
        float damageArcAngle = 60.0f;

        for (auto& monster : monsters) {
            sf::Vector2f monsterPos = monster->getPosition();
            float distance = calculateDistance(bladePos, monsterPos);

            if (distance <= range) {
                sf::Vector2f directionToMonster = monsterPos - character->getPosition();
                float angleToMonster = std::atan2(directionToMonster.y, directionToMonster.x) * 180.0f / 3.14159f;

                if (angleToMonster < 0) angleToMonster += 360.0f;
                float currentNormalizedAngle = std::fmod(currentAngle + 360.0f, 360.0f);

                float angleDiff = std::abs(angleToMonster - currentNormalizedAngle);
                if (angleDiff <= damageArcAngle / 2.0f || angleDiff >= (360.0f - damageArcAngle / 2.0f)) {
                    monster->takeDamage(damage);
                }
            }
        }
    }

    float calculateDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
        sf::Vector2f diff = pos1 - pos2;
        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }
};

#endif // BLADE_WHIRL_H