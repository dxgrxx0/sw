#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Monster.h"

class Projectile {
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float damage;

public:
    Projectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f targetPosition, float speed, float damage);
    virtual ~Projectile() = default; // 소멸자를 가상으로 설정

    virtual void update(float deltaTime); // 가상 메서드로 선언
    virtual void draw(sf::RenderTarget& target);
    virtual bool checkCollision(Monster& monster);

    bool isToBeDestroyed() const { return toBeDestroyed; }
    virtual bool isOutofBound(); // 기본 구현 제공
    sf::Vector2f getPosition() const { return position; }
    void setScale(float scaleX, float scaleY) {
        sprite.setScale(scaleX, scaleY);
    }
    sf::Sprite sprite;
    bool toBeDestroyed = false;

};

#endif // PROJECTILE_H
