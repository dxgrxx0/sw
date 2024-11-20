#ifndef SUBTOWER_H
#define SUBTOWER_H

#include <SFML/Graphics.hpp>
#include "Monster.h"

class SubTower {
protected:
    sf::Vector2f position;   // Ÿ�� ��ġ
    float range;             // ���� ����
    float attackSpeed;       // ���� �ӵ� (�ʴ� �߻� ���� Ƚ��)
    float attackDamage;      // ���ݷ�
    sf::Clock attackClock;   // ���� �ֱ� ����
    sf::CircleShape rangeIndicator; // �ð��� ���� ǥ��
	sf::Texture texture;
	sf::Sprite sprite;
public:
    SubTower(sf::Vector2f position, float range, float attackSpeed, float attackDamage);

    virtual void attack(std::vector<std::unique_ptr<Monster>>& monsters,float deltaTime); // ���� ����
    virtual void draw(sf::RenderTarget& target);                         // Ÿ�� �׸���
    sf::Vector2f getPosition() const;
    bool isInRange(const sf::Vector2f& targetPosition) const;
    virtual ~SubTower() = default;
};

#endif // SUBTOWER_H
