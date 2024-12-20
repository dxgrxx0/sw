#ifndef BULKUP_H
#define BULKUP_H

#include "BaseSkill.h"
#include "Character.h"
#include <iostream>
class BulkUp : public BaseSkill {
private:
    Character* character;
    float duration;    // 지속 시간
    float elapsedTime; // 경과 시간
	float powerBoost, speedBoost, rangeBoost,scaleBoost;
    float reducePower, reduceSpeed, reduceRange, reduceScale;
public:
    BulkUp(Character* character)
        : BaseSkill("Bulk Up", sf::Keyboard::W, 10.0f), character(character),duration(5),elapsedTime(0),powerBoost(20),
        speedBoost(50),rangeBoost(100),scaleBoost(2),reducePower(0),reduceSpeed(0),reduceRange(0),reduceScale(0) {}

    void applyEffect() override {
        isActive = true;
        std::cout << name << "w start" << std::endl;
        elapsedTime = 0.0f;

        reducePower = powerBoost;
        reduceSpeed = speedBoost;
        reduceRange = rangeBoost;
        reduceScale = scaleBoost;

        character->increaseAttackPower(powerBoost);
        character->increaseSpeed(speedBoost);
        character->increaseAttackRange(rangeBoost);
        character->setScale(scaleBoost);
    }
    void update(float deltaTime) override {
        cooldownTime += deltaTime;
        if (isActive) {
            elapsedTime += deltaTime;
            if (elapsedTime >= duration) {
                isActive = false;
                std::cout << name << " effect ended." << std::endl;
                // 히로인의 스탯 원상 복구
                character->increaseAttackPower(-reducePower);
                character->increaseSpeed(-reduceSpeed);
                character->increaseAttackRange(-reduceRange);
                character->setScale(1);

                reducePower = 0;
                reduceSpeed = 0;
                reduceRange = 0;
                reduceScale = 0;
            }
        }
    }
    void upgrade() {
		cooldown -= 1.0f;
        powerBoost += 10;
		speedBoost += 30;
		rangeBoost += 100;
        scaleBoost += 1;
    }
};

#endif // BULKUP_H
