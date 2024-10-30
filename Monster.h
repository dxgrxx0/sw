#ifndef MONSTER_H
#define MONSTER_H

#include <SFML/Graphics.hpp>

class Monster {
public:
    // ������
    Monster(float x, float y, float speed);

    // ���� ������Ʈ �Լ� (��ǥ ��ġ�� �̵�)
    void update(sf::Vector2f targetPosition, float deltaTime);

    // ���͸� �����쿡 �׸��� �Լ�
    void draw(sf::RenderTarget& target)const;

    // ������ ���� ��ġ ��ȯ �Լ�
    sf::Vector2f getPosition();

    // Ư�� ��ġ ��ó�� �ִ��� Ȯ���ϴ� �Լ� (�̴ϸ� Ȱ�� ��)
    bool isNear(sf::Vector2f position, float radius) const;

private:
    sf::RectangleShape shape; // ������ ���
    float movementSpeed; // ������ �̵� �ӵ�
};

#endif // MONSTER_H
