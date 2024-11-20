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

    sf::Vector2f getPosition() const { return position; }

    void setScale(float scaleX, float scaleY) {
        sprite.setScale(scaleX, scaleY);
    }

    void centerOrigin() {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
};

#endif // PROJECTILE_H
