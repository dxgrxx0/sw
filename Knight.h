#ifndef KNIGHT_H
#define KNIGHT_H

#include <SFML/Graphics.hpp>
#include "Monster.h"
#include <iostream>

// Knight.h
class Knight {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float healthPoint;
    float movementSpeed;
    float attackPower;
    float attackRange;
    float attackCooldown;
    sf::Clock attackTimer;
    sf::Vector2f initialPosition;
    sf::Vector2f mainTowerPos;
    float patrolRadius;
    float currentPatrolAngle;

    // 데미지 효과 관련 변수
    float damageTaken;
    float damageDisplayTime;
    bool showDamageText;

    // 애니메이션 관련 변수 추가
    sf::IntRect frameRect;
    float animationTimer;
    float frameDuration;
    int currentFrame;
    int frameCount;
    enum class AnimationState {
        Idle,
        Walk,
        Attack,
        Death
    };
    AnimationState currentState;
    bool facingLeft;

    // 스프라이트 프레임 크기
    static const int FRAME_WIDTH = 64;  // 스프라이트 한 프레임의 너비
    static const int FRAME_HEIGHT = 64; // 스프라이트 한 프레임의 높이

    void updateAnimation(float deltaTime) {
        animationTimer += deltaTime;
        if (animationTimer >= frameDuration) {
            animationTimer = 0;
            currentFrame++;

            // 각 상태별 프레임 개수에 따라 순환
            switch (currentState) {
            case AnimationState::Idle:
                if (currentFrame >= 8) currentFrame = 0;  // 8프레임 순환
                break;
            case AnimationState::Walk:
                if (currentFrame >= 8) currentFrame = 0;  // 8프레임 순환
                break;
            case AnimationState::Attack:
                if (currentFrame >= 6) {
                    currentFrame = 0;
                    currentState = AnimationState::Idle;  // 공격 애니메이션 완료 후 대기
                }
                break;
            case AnimationState::Death:
                if (currentFrame >= 6) currentFrame = 5;  // 죽음 애니메이션은 마지막 프레임에서 정지
                break;
            }

            // 프레임 직사각형 업데이트
            int row;
            switch (currentState) {
            case AnimationState::Idle: row = 0; break;
            case AnimationState::Walk: row = 1; break;
            case AnimationState::Attack: row = 2; break;
            case AnimationState::Death: row = 3; break;
            default: row = 0;
            }

            frameRect = sf::IntRect(currentFrame * FRAME_WIDTH, row * FRAME_HEIGHT,
                FRAME_WIDTH, FRAME_HEIGHT);
            sprite.setTextureRect(frameRect);
        }
    }

public:
    Knight(sf::Vector2f position, sf::Vector2f mainTowerPos)
        : movementSpeed(100.0f), healthPoint(100.0f), attackPower(20.0f)
        , attackRange(50.0f), attackCooldown(1.0f), initialPosition(position)
        , mainTowerPos(mainTowerPos), patrolRadius(100.0f), currentPatrolAngle(0.0f)
        , damageTaken(0.0f), damageDisplayTime(0.0f), showDamageText(false)
        , animationTimer(0.0f), frameDuration(0.1f), currentFrame(0)
        , currentState(AnimationState::Idle), facingLeft(false)
    {
        if (!texture.loadFromFile("knightbg.png")) {
            std::cout << "Failed to load knight texture!" << std::endl;
        }
        sprite.setTexture(texture);
        frameRect = sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
        sprite.setTextureRect(frameRect);
        sprite.setPosition(position);
        sprite.setScale(1.0f, 1.0f);
        sprite.setOrigin(FRAME_WIDTH / 2.0f, FRAME_HEIGHT / 2.0f);
    }

    void takeDamage(float damage) {
        healthPoint -= damage;
        if (damage > 0) {
            damageTaken = damage;
            damageDisplayTime = 0.0f;
            showDamageText = true;
        }
        if (healthPoint <= 0) {
            currentState = AnimationState::Death;
            currentFrame = 0;
        }
    }

    void update(float deltaTime) {
        if (showDamageText) {
            damageDisplayTime += deltaTime;
            if (damageDisplayTime >= 0.5f) {
                showDamageText = false;
                damageTaken = 0.0f;
            }
        }

        updateAnimation(deltaTime);
    }

    void engage(Monster* monster, float deltaTime) {
        if (!monster || currentState == AnimationState::Death) return;

        sf::Vector2f monsterPos = monster->getPosition();
        sf::Vector2f currentPos = sprite.getPosition();
        sf::Vector2f direction = monsterPos - currentPos;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // 방향 설정
        facingLeft = direction.x < 0;
        sprite.setScale(facingLeft ? -1.0f : 1.0f, 1.0f);

        if (distance > attackRange) {
            currentState = AnimationState::Walk;
            direction /= distance;
            sprite.move(direction * movementSpeed * deltaTime);
        }
        else {
            if (attackTimer.getElapsedTime().asSeconds() >= attackCooldown) {
                currentState = AnimationState::Attack;
                currentFrame = 0;
                monster->takeDamage(attackPower);
                attackTimer.restart();
            }
        }

        update(deltaTime);
    }

    void patrol(const sf::Vector2f& towerPos, float deltaTime) {
        if (currentState == AnimationState::Death) return;

        currentPatrolAngle += deltaTime * 0.5f;
        float x = towerPos.x + std::cos(currentPatrolAngle) * patrolRadius;
        float y = towerPos.y + std::sin(currentPatrolAngle) * patrolRadius;

        sf::Vector2f targetPos(x, y);
        sf::Vector2f currentPos = sprite.getPosition();
        sf::Vector2f direction = targetPos - currentPos;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // 방향 설정
        facingLeft = direction.x < 0;
        sprite.setScale(facingLeft ? -1.0f : 1.0f, 1.0f);

        if (distance > 1.0f) {
            currentState = AnimationState::Walk;
            direction /= distance;
            sprite.move(direction * movementSpeed * deltaTime);
        }
        else {
            currentState = AnimationState::Idle;
        }

        update(deltaTime);
    }

    void draw(sf::RenderTarget& target) const {
        target.draw(sprite);

        if (showDamageText) {
            sf::Font font;
            if (font.loadFromFile("arial.ttf")) {
                sf::Text damageText;
                damageText.setFont(font);
                damageText.setString(std::to_string(static_cast<int>(damageTaken)));
                damageText.setCharacterSize(15);
                damageText.setFillColor(sf::Color::Red);
                damageText.setPosition(sprite.getPosition().x, sprite.getPosition().y - 30);
                target.draw(damageText);
            }
        }
    }

    bool isDead() const {
        return healthPoint <= 0.0f;
    }

    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }

    void setTexture(const sf::Texture& tex) {
        sprite.setTexture(tex);
    }
};

#endif // KNIGHT_H
