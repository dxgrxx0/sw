#ifndef MONSTER_H

#define MONSTER_H

#include <SFML/Graphics.hpp>
class Character;
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
    // ������
    Monster(float x, float y, float speed,MonsterType type);

    // ���� ������Ʈ �Լ� (��ǥ ��ġ�� �̵�)
    void update(const sf::Vector2f& CharacterPos,const sf::Vector2f& MainTowerPos, float deltaTime,Character& character);

    // ���͸� �����쿡 �׸��� �Լ�
    void draw(sf::RenderTarget& target)const;

    // ������ ���� ��ġ ��ȯ �Լ�
    sf::Vector2f getPosition();

    // Ư�� ��ġ ��ó�� �ִ��� Ȯ���ϴ� �Լ� (�̴ϸ� Ȱ�� ��)
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
};

#endif // MONSTER_H
