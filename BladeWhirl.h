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
    float range;  //스킬이 적중할 수 있는 거리
    float damage;
    float activeDuration; //스킬의 지속 시간
    float elapsedTime; 

    // 검 휘두르기 관련 변수들
    float currentAngle;  //회전 각도
    float startAngle;  
    float endAngle;   
    float swingSpeed;  //회전 속도
    float handleLength;// 손잡이 길이
    float bladeLength; //검의 칼날 길이
    float handleFixed; // 검 손잡이와 캐릭터 위치 사이의 고정 거리
    float lastDamageAngle;

    // 검의 시각적 표현
    sf::RectangleShape handleSprite;      // 손잡이
    sf::RectangleShape guardSprite;       // 가드(십자가)
    sf::ConvexShape bladeSprite;          // 칼날
    sf::RectangleShape bladeCenterLine;   // 칼날 중앙선
    
public:
    BladeWhirl(Character* character, std::vector<std::unique_ptr<Monster>>& monsters)
        : BaseSkill("Blade Whirl", sf::Keyboard::Q, 2.0f), // 쿨 2초
        character(character),
        monsters(monsters),
        range(150.0f),
        damage(25.0f),
        activeDuration(0.5f),
        elapsedTime(0.0f),
        currentAngle(-45.0f),
        startAngle(-45.0f),
        endAngle(225.0f),
        swingSpeed(720.0f),
        handleLength(40.0f),
        bladeLength(100.0f),
        handleFixed(20.0f),
        lastDamageAngle(-45.0f)
    {
        // 손잡이 설정
        handleSprite.setSize(sf::Vector2f(handleLength, 8.0f));
        handleSprite.setFillColor(sf::Color(139, 69, 19));  // 갈색
        handleSprite.setOrigin(0, 4.0f);

        // 가드(십자가) 설정
        guardSprite.setSize(sf::Vector2f(30.0f, 6.0f));
        guardSprite.setFillColor(sf::Color(192, 192, 192)); // 은색
        guardSprite.setOrigin(15.0f, 3.0f);

        // 칼날 설정
        bladeSprite.setPointCount(5);
        bladeSprite.setPoint(0, sf::Vector2f(0, -8));
        bladeSprite.setPoint(1, sf::Vector2f(bladeLength * 0.9f, -6));
        bladeSprite.setPoint(2, sf::Vector2f(bladeLength, 0));
        bladeSprite.setPoint(3, sf::Vector2f(bladeLength * 0.9f, 6));
        bladeSprite.setPoint(4, sf::Vector2f(0, 8));
        bladeSprite.setFillColor(sf::Color(211, 211, 211));
        bladeSprite.setOrigin(0, 0);

        // 칼날 중앙선 설정
        bladeCenterLine.setSize(sf::Vector2f(bladeLength * 0.85f, 2.0f));
        bladeCenterLine.setFillColor(sf::Color(160, 160, 160));
        bladeCenterLine.setOrigin(0, 1.0f);


    }


    void applyEffect() override {
        if (elapsedTime >= cooldown) {
            isActive = true;
            elapsedTime = 0.0f;
            currentAngle = startAngle;
            lastDamageAngle = startAngle;
            std::cout << "Blade Whirl activated!" << std::endl;
            applyDamageInArc();
        }
        else {
            std::cout << "Blade Whirl is on cooldown! Time remaining: "
                << cooldown - elapsedTime << "s" << std::endl;
        }
    }


    void update(float deltaTime) override {
        elapsedTime += deltaTime;

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
            target.draw(handleSprite);
            target.draw(guardSprite);
            target.draw(bladeSprite);
            target.draw(bladeCenterLine);
        }
    }

private:
    void updateWeaponPosition() {
        sf::Vector2f characterPos = character->getPosition();
        float angleRad = currentAngle * 3.14159f / 180.0f;

        float handleX = characterPos.x + std::cos(angleRad) * handleFixed;
        float handleY = characterPos.y + std::sin(angleRad) * handleFixed;

        handleSprite.setPosition(handleX, handleY);
        handleSprite.setRotation(currentAngle);

        float guardX = handleX + std::cos(angleRad) * handleLength;
        float guardY = handleY + std::sin(angleRad) * handleLength;

        guardSprite.setPosition(guardX, guardY);
        guardSprite.setRotation(currentAngle);

        bladeSprite.setPosition(guardX, guardY);
        bladeSprite.setRotation(currentAngle);

        bladeCenterLine.setPosition(guardX, guardY);
        bladeCenterLine.setRotation(currentAngle);
    }

    void applyDamageInArc() {
        sf::Vector2f bladePos = bladeSprite.getPosition();
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
