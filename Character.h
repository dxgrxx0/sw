#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Monster.h"
#include "Utility.h"
class Character {
public:
    Character(const std::string& textureFile, float x, float y, float scale, float speed);
    void handleInput(float deltaTime);
    void updateAnimation(float deltaTime);
    void draw(sf::RenderTarget& target);
    void basicAttack(std::vector<Monster>& monsters);
    sf::Vector2f getPosition();
    bool getAttackApplied();
    bool getIsSwinging();
    float getAttackRange();
    float getAttackDamage();
	void setAttackApplied(bool applied);
    void heal(float healAmount);
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect currentFrame;
    int frameWidth, frameHeight;
    int currentFrameIndex;
    int totalFrames;
    bool isSwinging;
    bool attackApplied;

    float animationSpeed;
    float timeSinceLastFrame;
    void startSwinging();

    //능력치
    float movementSpeed;
    float attackRange;
    float attackDamage;
    float attackCoolDown;
    float defense;
    float maxHealth;    //최대체력
    float health;//현재체력
};
