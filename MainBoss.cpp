#include "MainBoss.h"
#include "Character.h"
#include "MainTower.h"
#include <iostream>

MainBoss::MainBoss(float x, float y, float speed, MonsterType type) : Monster(x, y, speed, type) {
   
    switch (type) {
    case MonsterType::Main_Boss: // 메인보스
        textureName = "MainBoss";
        movementSpeed = 80.0f;
        healthPoint = 5000.0f;
        attackPower = 100.0f;
        defense = 50.0f;
       
        break;
    default:
        break;

    }
    texture = ResourceManager::getInstance().getTexture(textureName);
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(x, y);
    sprite.setOrigin(500, 500);
    std::vector<std::string> penNames = { "RedPen", "OrangePen", "YellowPen", "GreenPen", "BluePen","NavyPen", "PurplePen" };

    for (size_t i = 0; i < 7; ++i) {
        penTextures[i] = ResourceManager::getInstance().getTexture(penNames[i]);
		penSprites[i].setTexture(penTextures[i]);
    }

}
void MainBoss::update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos, float deltaTime, Character& character, MainTower& mainTower) {
    Monster::update(CharacterPos, MainTowerPos, deltaTime, character, mainTower);
    
}
void MainBoss::draw(sf::RenderTarget& target) {
    Monster::draw(target);
}
