#ifndef SUBTOWER_H
#define SUBTOWER_H

#include <SFML/Graphics.hpp>
#include "Monster.h"

class SubTower {
protected:
    sf::Vector2f position;   // 타워 위치
    float range;             // 공격 범위
    float attackSpeed;       // 공격 속도
    float attackDamage;      // 공격력
    sf::Clock attackClock;   // 공격 주기 관리
    sf::CircleShape rangeIndicator; // 시각적 범위 표시
	sf::Texture texture;
	sf::Sprite sprite;

public:
    SubTower(sf::Vector2f position, float range, float attackSpeed, float attackDamage);
    std::string type;
    virtual void attack(std::vector<std::unique_ptr<Monster>>& monsters,float deltaTime); // 공격 로직
    virtual void draw(sf::RenderTarget& target);                         // 타워 그리기
    sf::Vector2f getPosition() const;
    bool isInRange(const sf::Vector2f& targetPosition) const;
    
    virtual ~SubTower() = default;
};

#endif // SUBTOWER_H
