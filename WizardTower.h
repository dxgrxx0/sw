#ifndef WIZARD_TOWER_H
#define WIZARD_TOWER_H

#include "SubTower.h"
#include <vector>

struct LightningEffect {
    sf::Sprite sprite;
    sf::Clock timer;
    Monster* target;

    LightningEffect(const sf::Texture& texture, Monster* target, float duration)
        : target(target) {
        sprite.setTexture(texture);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height); // 중심점을 중앙으로
        sprite.setPosition(target->getPosition().x, target->getPosition().y); // 몬스터 머리 위에 번개 위치
        sprite.setScale(0.5f, 0.5f); // 크기 조정
        timer.restart();
    }

    bool isExpired(float duration) const {
        return timer.getElapsedTime().asSeconds() >= duration;
    }
};

class WizardTower : public SubTower {
private:
    sf::Texture lightningTexture; // 번개 텍스처
    std::vector<LightningEffect> activeLightning; // 활성화된 번개 효과
    float lightningDuration; // 번개 효과 지속 시간
    sf::Texture wizardTexture;
	sf::Sprite wizardSprite;
	sf::IntRect wizardRect;
    float wizardFrameIndex;
	float elapsedTime;
public:
    WizardTower(sf::Vector2f position)
		: SubTower(position, 500.0f, 1.0f, 10.0f), lightningDuration(0.2f) {//위치, 공격범위, 공격속도, 공격력
        texture.loadFromFile("WizardTower.png");
        sprite.setTexture(texture);
        sprite.setPosition(position);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
		lightningTexture.loadFromFile("lightning.png");
		wizardTexture.loadFromFile("wizard.png");
		wizardSprite.setTexture(wizardTexture);
		wizardRect = sf::IntRect(0, 0, 90, 96);
		wizardSprite.setTextureRect(wizardRect);
        wizardFrameIndex = 0;
        elapsedTime = 0;
		wizardSprite.setOrigin(wizardSprite.getGlobalBounds().width / 2, wizardSprite.getGlobalBounds().height / 2);
		wizardSprite.setPosition(position.x, position.y - 90);
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
			elapsedTime += deltaTime;
			if (elapsedTime >= 0.2f) {
                wizardFrameIndex++;
				elapsedTime = 0;
				wizardRect.left = wizardRect.width * int(wizardFrameIndex);
				wizardSprite.setTextureRect(wizardRect);
			}
            if (wizardFrameIndex == 4) { 
                attackClock.restart(); 
				wizardFrameIndex = 0;
            }
        }
        if (wizardFrameIndex == 2) {
            int attackCount = 0;
            for (auto& monster : monsters) {
                if (isInRange(monster->getPosition())) {
                    activeLightning.emplace_back(lightningTexture, monster.get(), lightningDuration);
                    //monster->takeDamage(attackDamage);
                    monster->takeDamage(attackDamage);
                    attackCount++;
                    if (attackCount >= 5) break;

                }
            }
        }
    }

    void draw(sf::RenderTarget& target) override {
        target.draw(sprite);
		target.draw(wizardSprite);
        for (auto& lightning : activeLightning) {
            target.draw(lightning.sprite);
        }
        // 번개 효과 만료 처리
        activeLightning.erase(
            std::remove_if(activeLightning.begin(), activeLightning.end(),
                [this](const LightningEffect& lightning) {
                    return lightning.isExpired(lightningDuration);
                }),
            activeLightning.end()
        );
    }
};

#endif // WIZARD_TOWER_H
