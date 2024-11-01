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

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::IntRect currentFrame;
    int frameWidth, frameHeight;
    int currentFrameIndex;
    int totalFrames;
    bool isSwinging;
    float movementSpeed;
    float animationSpeed;
    float timeSinceLastFrame;
    void startSwinging();
    int hanseo_number_git_tmp;
    int hanseo_number_git_tmp1;
    int hanseo_number_git_tmp2;
    int hanseo_number_git_tmp3;
    int hanseo_number_git_tmp4;
    int hanseo_number_git_tmp5;
    int hanseo_number_git_tmp6;

};
