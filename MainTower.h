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
    void upgrade(std::vector<SubTower>& subTowers); // 서브 타워 강화
    void draw(sf::RenderWindow& window) const;
    const sf::Sprite& getSprite() const; // 스프라이트를 반환하는 메서드 추가

private:
    float healAmountPerSecond; // 회복 속도
    sf::Sprite sprite;
    sf::Texture texture;
    float health;
};
