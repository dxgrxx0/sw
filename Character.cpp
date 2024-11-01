#include "Character.h"
#include <iostream>
#include <cmath>  // If needed for calculations

Character::Character(const std::string& textureFile, float x, float y, float scale, float speed)
    : movementSpeed(speed), animationSpeed(0.1f), timeSinceLastFrame(0.0f),
    currentFrameIndex(0), isSwinging(false), frameWidth(32), frameHeight(32), totalFrames(5) {
    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Failed to load texture" << std::endl;
        throw std::runtime_error("Failed to load texture");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(scale, scale);
    currentFrame = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(currentFrame);
}

void Character::handleInput(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isSwinging) {
        currentFrame.top = 32;
        sprite.move(0, -movementSpeed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isSwinging) {
        currentFrame.top = 0;
        sprite.move(0, movementSpeed * deltaTime);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !isSwinging) {
        currentFrame.top = 64;
        sprite.move(-movementSpeed * deltaTime, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !isSwinging) {
        currentFrame.top = 96;
        sprite.move(movementSpeed * deltaTime, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isSwinging) {
        startSwinging();
    }
    
}

void Character::updateAnimation(float deltaTime) {
    timeSinceLastFrame += deltaTime;

    if (timeSinceLastFrame >= animationSpeed) {
        currentFrame.left = currentFrameIndex * frameWidth;
        sprite.setTextureRect(currentFrame);

        if (isSwinging) {
            currentFrameIndex++;
            if (currentFrameIndex >= totalFrames) {
                currentFrameIndex = 0;
                isSwinging = false;
                currentFrame.top -= 128;
            }
        }
        else {
            currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
        }
        timeSinceLastFrame = 0.0f;
    }
}

void Character::draw(sf::RenderTarget& target) {
    target.draw(sprite);
}

sf::Vector2f Character::getPosition() {
    return sprite.getPosition();
}

void Character::startSwinging() {
    currentFrame.top += 128;
    isSwinging = true;
    currentFrameIndex = 0;
}
