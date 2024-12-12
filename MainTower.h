// MainTower.h
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
class Character;
class SubTower; // 전방 선언

class MainTower {
public:
    MainTower(const sf::Vector2f& position);
    void healNearbyCharacter(float deltaTime, Character& character);
    void upgrade(); 
    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const; // 스프라이트를 반환하는 메서드 추가
    sf::Vector2f getPosition();
    void takeDamage(float attackPower);
    float getMaxHealth();
    float getHealth();
private:
    float healAmountPerSecond; // 회복 속도
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float health;
    float defense;
    float maxHealth;
};
