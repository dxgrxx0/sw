#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Character {
public:
    Character(const std::string& textureFile, float x, float y, float scale, float speed);
    void handleInput(float deltaTime);
    void updateAnimation(float deltaTime);
    void draw(sf::RenderTarget& target);
    sf::Vector2f getPosition();
    bool getAttackApplied();


private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect currentFrame;
    int frameWidth, frameHeight;
    int currentFrameIndex;
    int totalFrames;
    bool isSwinging;
    bool attackApplied;
    float movementSpeed;
    float animationSpeed;
    float timeSinceLastFrame;
    void startSwinging();
    float attackRange;
    float attackDamage;

};
