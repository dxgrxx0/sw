#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Monster.h"

class Projectile {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    float damage;
    bool toBeDestroyed = false;

    
public:
    Projectile(const sf::Texture& texture, sf::Vector2f position, sf::Vector2f targetPosition, float speed, float damage);

    void update(float deltaTime);
    void draw(sf::RenderTarget& target);

    bool checkCollision(Monster& monster);
    bool isDestroyed() const { return toBeDestroyed; }
    bool isOutofBound();

    sf::Vector2f getPosition() const {
        return position;
    }
};

#endif // PROJECTILE_H
