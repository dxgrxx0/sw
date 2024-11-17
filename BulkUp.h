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

public:
    BulkUp(Character* character)
        : BaseSkill("Bulk Up", sf::Keyboard::W, 10.0f), character(character),duration(5),elapsedTime(0) {}

    void applyEffect() override {
        isActive = true;
        std::cout << name << "w start" << std::endl;
        elapsedTime = 0.0f;
        character->increaseAttackPower(20);
        character->increaseSpeed(50);
        character->increaseAttackRange(200);
        character->setScale(5);
    }
    void update(float deltaTime) override {
        if (isActive) {
            elapsedTime += deltaTime;
            if (elapsedTime >= duration) {
                isActive = false;
                std::cout << name << " effect ended." << std::endl;
                // 히로인의 스탯 원상 복구
                character->increaseAttackPower(-20);
                character->increaseSpeed(-50);
                character->increaseAttackRange(-200);
                character->setScale(3);
            }
        }
    }
};

#endif // BULKUP_H
