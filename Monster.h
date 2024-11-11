#ifndef MONSTER_H

#define MONSTER_H

#include <SFML/Graphics.hpp>

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
    Monster(float x, float y, float speed, MonsterType type);

    void update(sf::Vector2f targetPosition, float deltaTime, Character& character);

    void draw(sf::RenderTarget& target)const;

    sf::Vector2f getPosition();
   
    bool isNear(sf::Vector2f position, float radius) const;

    void takeDamage(float attackDamage);
    float getHealthPoint()const;
private:
    sf::RectangleShape shape; // ������ ���
    float movementSpeed; // ������ �̵� �ӵ�
    float healthPoint;
    float damageTaken; // 최근 피해량
    bool isTakingDamage; // 피해를 표시해야 하는지 여부
    float damageDisplayDuration; // 피해 표시 지속 시간
    float damageDisplayTime; // 피해 표시 경과 시간
    float attackPower; //공격력
    float defense;  //방어력

    sf::Clock attackTimer; // 공격 타이머 추가
    float attackCooldown = 1.0f; // 1초 주기

    sf::Sprite sprite; 
    sf::Texture texture; 
};

#endif // MONSTER_H