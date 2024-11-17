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

    void basicAttack(std::vector<std::unique_ptr<Monster>>& monsters);

    sf::Vector2f getPosition();
    bool getAttackApplied();
    bool getIsSwinging();
    float getAttackRange();
    float getAttackDamage();
    float getHealth();
    float getMaxHealth();
    void takeDamage(float damageAmount);
    void setAttackApplied(bool applied);
    void heal(float healAmount);
    bool isMonsterInAttackRange(const sf::Vector2f& characterPosition, const sf::Vector2f& monsterPosition,
        float attackRange, float attackAngle, float characterRotation);
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect currentFrame;
    int frameWidth, frameHeight;
    int currentFrameIndex;
    int totalFrames;
    bool isSwinging;
    bool attackApplied;
    sf::Sprite slashSprite; // 슬래시 이미지를 위한 스프라이트
    sf::Texture slashTexture; // 슬래시 텍스처
    float animationSpeed;
    float timeSinceLastFrame;
    void startSwinging();
    float facingDirection; // 캐릭터가 바라보는 방향 (각도)
    //능력치
    float movementSpeed;
    float attackRange;
    float attackDamage;
    float attackCoolDown;
    float defense;
    float maxHealth;    //최대체력
    float health;//현재체력
};