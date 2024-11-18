#ifndef TELEPORT_H
#define TELEPORT_H

#include "BaseSkill.h"
#include "Character.h"
#include "MainTower.h"

class Teleport : public BaseSkill {
private:
    Character* character;
    MainTower* mainTower;

public:
    Teleport(Character* character, MainTower* mainTower)
        : BaseSkill("Teleport", sf::Keyboard::E, 15.0f), character(character), mainTower(mainTower) {}

    void applyEffect() override {
        character->setPosition(mainTower->getPosition()); // 타워 위치로 이동
    }
};

#endif // TELEPORT_H
