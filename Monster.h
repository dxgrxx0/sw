#ifndef MONSTER_H

#define MONSTER_H

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Utility.h"
class Character;
class MainTower;
enum class MonsterType {
    Basic,
    Speed,
    Attack,
    Defense,
    Mid_Boss,
    Main_Boss
};
class Monster {
public:
    Monster(float x, float y, int waveLevel,MonsterType type);

    virtual void update(const sf::Vector2f& CharacterPos,const sf::Vector2f& MainTowerPos, float deltaTime,Character& character,MainTower& mainTower);

    virtual void draw(sf::RenderTarget& target);

    sf::Vector2f getPosition();

    bool isNear(sf::Vector2f position, float radius) const;

    
    void takeDamage(float attackDamage);
    float getHealthPoint()const;

    MonsterType getBossMonsterType() const {
        if (textureName == "MainBoss") {
            return MonsterType::Main_Boss;
        }
        else
            return MonsterType::Mid_Boss;
    }
    void upgrade();
    
protected:
    float movementSpeed; 
    float healthPoint;
    float damageTaken; // 최근 피해량
    bool isTakingDamage; // 피해를 표시해야 하는지 여부
    float damageDisplayDuration; // 피해 표시 지속 시간
    float damageDisplayTime; // 피해 표시 경과 시간
    float attackPower; //공격력
    float defense;  //방어력
    float attackRange;
    sf::Clock attackTimer; // 공격 타이머 추가
    float attackCooldown = 1.0f; // 1초 주기
    MonsterType monsterType;
    std::string textureName;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Font font;
    sf::Text damageText;
};

#endif // MONSTER_H
