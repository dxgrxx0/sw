#ifndef MONSTER_H
#define MONSTER_H

#include <SFML/Graphics.hpp>

class Monster {
public:
    // 생성자
    Monster(float x, float y, float speed);

    // 몬스터 업데이트 함수 (목표 위치로 이동)
    void update(sf::Vector2f targetPosition, float deltaTime);

    // 몬스터를 윈도우에 그리는 함수
    void draw(sf::RenderTarget& target);

    // 몬스터의 현재 위치 반환 함수
    sf::Vector2f getPosition();

    // 특정 위치 근처에 있는지 확인하는 함수 (미니맵 활용 등)
    bool isNear(sf::Vector2f position, float radius) const;

private:
    sf::RectangleShape shape; // 몬스터의 모양
    float movementSpeed; // 몬스터의 이동 속도
};

#endif // MONSTER_H
