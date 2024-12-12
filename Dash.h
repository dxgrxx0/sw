#ifndef DASH_H
#define DASH_H

#include "BaseSkill.h"
#include "Character.h"
#include <iostream>
class Dash : public BaseSkill {
private:
    Character* character;
    float duration;    // 지속 시간
    float elapsedTime; // 경과 시간
    int direction;
	sf::Vector2f dashVector;
public:
    Dash(Character* character)
        : BaseSkill("Dash", sf::Keyboard::E, 2.0f), character(character), duration(0.5), elapsedTime(0),direction(0) {}

    void applyEffect() override {
        isActive = true;
        std::cout << name << "dash start" << std::endl;
        elapsedTime = 0.0f;
		direction = character->getDirection();//0:right,1:down,2:left,3:up
        switch (direction) {
        case 0:
			dashVector = sf::Vector2f(1, 0);
			break;
		case 1:
			dashVector = sf::Vector2f(0, 1);
			break;
		case 2:
			dashVector = sf::Vector2f(-1, 0);
			break;
		case 3:
			dashVector = sf::Vector2f(0, -1);
			break;
        }
    }
    void update(float deltaTime) override {
        cooldownTime += deltaTime;
		if (isActive) {
			elapsedTime += deltaTime;
			sf::Vector2f pos = character->getPosition();
			if(pos.x<3200&&pos.x>-1600&&pos.y<3000&&pos.y>-2000)character->setPosition(character->getPosition() + dashVector * 500.0f * deltaTime);
            if (elapsedTime >= duration) {
                isActive = false;
                std::cout << name << "dash effect ended." << std::endl;
            }
        }
    }
    void upgrade() override {
		cooldown -= 0.2f;
    }
};

#endif // BULKUP_H
