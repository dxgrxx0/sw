#pragma once
#include <SFML/Graphics.hpp>

class Monster {
public:
    Monster(float x, float y, float speed);
    void update(sf::Vector2f targetPosition, float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition();

private:
    sf::RectangleShape shape;
    float movementSpeed;
};
