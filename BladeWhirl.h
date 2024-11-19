#ifndef BLADEWHIRL_H
#define BLADEWHIRL_H

#include "BaseSkill.h"
#include "Character.h"
#include "Monster.h"
#include "Utility.h"
class BladeWhirl : public BaseSkill {
private:
    Character* character;
	std::vector<std::unique_ptr<Monster>>& monsters;
    float range;
    float damage;

public:
    BladeWhirl(Character* character,std::vector<std::unique_ptr<Monster>>& monsters)
        : BaseSkill("Blade Whirl", sf::Keyboard::Q, 2.0f), character(character),monsters(monsters),range(200),damage(100) {}

    void applyEffect() override {
        for (auto& monster : monsters) {
			if (range >= calculateDistance(character->getPosition(), monster->getPosition())) {
				monster->takeDamage(damage);
			}
        }
    }
};

#endif // BLADEWHIRL_H
