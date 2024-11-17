#ifndef BLADEWHIRL_H
#define BLADEWHIRL_H

#include "BaseSkill.h"
#include "Character.h"

class BladeWhirl : public BaseSkill {
private:
    Character* character;

public:
    BladeWhirl(Character* character)
        : BaseSkill("Blade Whirl", sf::Keyboard::Q, 5.0f), character(character) {}

    void applyEffect() override {
        //character->performAreaAttack(150.0f, 50.0f); // 범위 공격
    }
};

#endif // BLADEWHIRL_H
