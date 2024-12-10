#ifndef TRAINING_TOWER_H
#define TRAINING_TOWER_H
#include "SubTower.h"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Knight {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    sf::Vector2f targetPosition; // 목표 위치 추가
    float speed;
    float attackRange;
    float attackDamage;
    float attackSpeed;
    float maxHealth;
    float currentHealth;
    sf::Clock attackClock;
    sf::Clock animationClock;
    sf::Clock stateClock;      // 상태 전환을 위한 클럭 추가

    int currentFrame;
    int frameWidth;
    int frameHeight;
    float animationTime;
    enum State { IDLE, WALKING, ATTACKING } currentState;
    int facingLeft = 0;
    bool isAttacking;          // 공격 상태 플래그 추가
    Monster* currentTarget;     // 현재 타겟 몬스터 추가

public:
    Knight(sf::Vector2f startPos, sf::Vector2f mainTowerPos)
        : position(startPos)
        , targetPosition(mainTowerPos)
        , speed(100.0f)
        , attackRange(50.0f)
        , attackDamage(10.0f)
        , attackSpeed(1.0f)
        , maxHealth(100.0f)
        , currentHealth(100.0f)
        , currentFrame(0)
        , frameWidth(32)
        , frameHeight(32)
        , animationTime(0.1f)
        , currentState(IDLE)
        , facingLeft(false)
        , isAttacking(false)
        , currentTarget(nullptr)
    {
        if (!texture.loadFromFile("knightbg.png")) {
        }
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        sprite.setPosition(position);
        sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
        sprite.setScale(2.0f, 2.0f);
    }

    void updateAnimation() {
        float currentTime = animationClock.getElapsedTime().asSeconds();
        if (currentTime >= animationTime) {
            int row;
            int framesInRow;

            switch (currentState) {
            case IDLE:
                row = facingLeft;
                framesInRow = 5;
                break;
            case WALKING:
                row = facingLeft;
                framesInRow = 5;
                break;
            case ATTACKING:
                row = facingLeft + 4;
                framesInRow = 5;
                if (currentFrame >= framesInRow - 1) {
                    currentFrame = 0;  // 공격 애니메이션 완료 후 리셋
                    if (!isAttacking) {
                        currentState = IDLE;  // 공격이 끝나면 IDLE 상태로
                    }
                }
                break;
            }

            currentFrame = (currentFrame + 1) % framesInRow;

            // 이전 프레임 위치 저장
            sf::Vector2f prevPos = sprite.getPosition();

            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight));

            // 위치 유지
            sprite.setPosition(prevPos);

            animationClock.restart();
        }
    }

    void engage(Monster* monster, float deltaTime) {
        if (!monster) return;

        currentTarget = monster;
        sf::Vector2f direction = monster->getPosition() - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (direction.x < 0 && abs(direction.x) > abs(direction.y))facingLeft = 2;
        else if (direction.x > 0 && abs(direction.x) > abs(direction.y))facingLeft = 3;
        else if (direction.y < 0 && abs(direction.x) < abs(direction.y))facingLeft = 1;
        else if (direction.y > 0 && abs(direction.x) < abs(direction.y))facingLeft = 0;


        if (distance > attackRange) {
            // 몬스터를 향해 이동
            direction /= distance;
            position += direction * speed * deltaTime;
            sprite.setPosition(position);
            currentState = WALKING;
            isAttacking = false;
        }
        else {
            // 공격 범위 내에서 공격
            currentState = ATTACKING;
            isAttacking = true;
            if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed) {
                monster->takeDamage(attackDamage);
                attackClock.restart();
            }
        }
        updateAnimation();
    }

    void patrol(sf::Vector2f towerPos, float deltaTime) {
        sf::Vector2f direction = towerPos - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 50.0f) {  // 타워 주변 50 반경 내에서 순찰
            direction /= distance;
            position += direction * speed * deltaTime;
            sprite.setPosition(position);

            if (direction.x < 0 && abs(direction.x) > abs(direction.y))facingLeft = 2;
            else if (direction.x > 0 && abs(direction.x) > abs(direction.y))facingLeft = 3;
            else if (direction.y < 0 && abs(direction.x) < abs(direction.y))facingLeft = 1;
            else if (direction.y > 0 && abs(direction.x) < abs(direction.y))facingLeft = 0;

            currentState = WALKING;
        }
        else {
            // 타워 주변에서 가만히 있기
            currentState = IDLE;
        }
        isAttacking = false;
        updateAnimation();
    }

    void takeDamage(float damage) {
        currentHealth -= damage;
        if (currentHealth < 0) currentHealth = 0;
    }

    bool isDead() const {
        return currentHealth <= 0;
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    float getRange() const {
        return attackRange;
    }

    void draw(sf::RenderTarget& target) {
        target.draw(sprite);

        // 체력바 그리기
        /*
        sf::RectangleShape healthBar(sf::Vector2f(30.0f, 5.0f));
        healthBar.setPosition(position.x - 15.0f, position.y - 25.0f);
        healthBar.setFillColor(sf::Color::Red);
        target.draw(healthBar);

        sf::RectangleShape currentHealthBar(sf::Vector2f(30.0f * (currentHealth / maxHealth), 5.0f));
        currentHealthBar.setPosition(position.x - 15.0f, position.y - 25.0f);
        currentHealthBar.setFillColor(sf::Color::Green);
        target.draw(currentHealthBar);*/
    }
};


class TrainingTower : public SubTower {
private:
    std::vector<std::unique_ptr<Knight>> knights;
    int maxKnights;
    float spawnInterval;
    sf::Clock spawnClock;
    sf::Vector2f mainTowerPos;

public:
    TrainingTower(sf::Vector2f position)
        : SubTower(position, 400.0f, 0.2f, 0.0f)
        , maxKnights(3)
        , spawnInterval(5.0f)
        , mainTowerPos(sf::Vector2f(650.0f, 500.0f))
    {
        if (!texture.loadFromFile("TrainingTower.png")) {
        }
		sprite.setTexture(texture);
		sprite.setPosition(position);
		sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);

        rangeIndicator.setPosition(position);
        rangeIndicator.setRadius(range);
        rangeIndicator.setOrigin(range, range);
        rangeIndicator.setFillColor(sf::Color(0, 255, 0, 10));
        rangeIndicator.setOutlineColor(sf::Color(0, 255, 0, 100));
        rangeIndicator.setOutlineThickness(2.0f);
        type = "TrainingTower";
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed && knights.size() < maxKnights) {
            spawnKnight();
            attackClock.restart();
        }
        updateKnights(deltaTime, monsters);
    }

    void draw(sf::RenderTarget& target) override {
        target.draw(sprite);
        for (const auto& knight : knights) {
            knight->draw(target);
        }
    }

private:
    void spawnKnight() {
        knights.push_back(std::make_unique<Knight>(position, mainTowerPos));
    }

    void updateKnights(float deltaTime, std::vector<std::unique_ptr<Monster>>& monsters) {
        for (auto it = knights.begin(); it != knights.end();) {
            if ((*it)->isDead()) {
                it = knights.erase(it);
                continue;
            }

            // 가장 가까운 몬스터 찾기
            Monster* closestMonster = nullptr;
            float closestDistance = std::numeric_limits<float>::max();

            for (const auto& monster : monsters) {
                float distance = getDistance((*it)->getPosition(), monster->getPosition());
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestMonster = monster.get();
                }
                
            }

            // 타워 범위 내에 몬스터가 있으면 공격, 없으면 순찰
            if (closestMonster && isInRange(closestMonster->getPosition())) {
                (*it)->engage(closestMonster, deltaTime);
            }
            else {
                (*it)->patrol(position, deltaTime);  // 메인타워가 아닌 훈련타워 주변을 순찰
            }

            ++it;
        }
    }

    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};



#endif // TRAINING_TOWER_H