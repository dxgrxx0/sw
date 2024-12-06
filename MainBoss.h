#pragma once
#include "Monster.h"
#include <SFML/Graphics.hpp>

class MainBoss : public Monster {
private:
    sf::Texture penTextures[7];
	sf::Sprite penSprites[7];
public:
    MainBoss(float x, float y, float speed, MonsterType type);
    void update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos, float deltaTime, Character& character, MainTower& mainTower) override;
    void draw(sf::RenderTarget& target) override;

};
