#pragma once
#include "Monster.h"
#include <SFML/Graphics.hpp>

class MidBoss : public Monster {
private:
    //mid,main boss skill variable
    sf::Clock skillCooldown;
    sf::Clock rangedAttackCooldown;
    float skillDuration;
    bool isSkillActive;
    float originalSpeed;
    float originalDefense;
    float originalAttackPower;
    float originalattackRange;

    void removeSkillEffects();

    bool isCloneActive;
    std::vector<sf::Sprite> cloneSprites;
    float cloneDistance;
    float cloneRotationAngle;
    float cloneRotationSpeed;



    //원거리 공격 로직 추가
    struct Projectile {
        sf::CircleShape shape;
        sf::Vector2f velocity;
        float damage;
        bool active;

        Projectile(const sf::Vector2f& pos, const sf::Vector2f& vel, float dmg)
            : velocity(vel), damage(dmg), active(true) {
            shape.setRadius(10.f);
            shape.setFillColor(sf::Color::Red);
            shape.setPosition(pos);
            shape.setOrigin(10.f, 10.f);
        }
    };

    std::vector<Projectile> projectiles;
    float projectileSpeed = 400.0f;
    sf::Clock rangedAttackTimer;

    //세번째 스킬:순간이동
    float alpha;                  // 투명도 (0~255)
    float oscillationTime;        // 진동 시간
    bool isThirdSkillActive;      // 세 번째 스킬 활성 상태
    sf::Clock thirdSkillTimer;    // 스킬 타이머

    // 원형 이동 관련 변수
    bool circularAttacking;
    float circularAngle = 0.0f;          // 원형 이동의 현재 각도
    float circularSpeed = 540.0f;         // 원형 이동 속도 (각도/초)
    float circularRadius = 200.0f;       // 원형 이동 반지름
    sf::Clock circularTimer;             // 원형 이동 지속 시간 타이머
    float circularDuration = 1.5f;       // 원형 이동 지속 시간 (초)

    void initializeCircularPath(int radius);
    // 흔적 관련 변수

    sf::VertexArray trailVertices;       // 흔적을 그리기 위한 정점 배열
    sf::Vector2f playerPosition;


    //폭발 관련 변수
    sf::Texture bossExplodeTexture;
    sf::Sprite bossExplodeSprite;
    sf::CircleShape bossExplodeCircle;
    float bossExplodeFrameClock;
    int bossExplodeCurrentFrame;
    bool drawingBossExplode;
	bool hitByExplosion;
    float maxHealth;
	sf::RectangleShape healthBar;
	sf::RectangleShape healthBarBack;
public:
    MidBoss(float x, float y, float speed, MonsterType type);
    void update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos, float deltaTime, Character& character, MainTower& mainTower) override;
    void draw(sf::RenderTarget& target) override;

    void Fir_useSkill(Character& character, MainTower& mainTower);
    void Sec_useSkill(Character& character, MainTower& mainTower);
    void Third_useSkill(Character& chracter);
    void CircularAttack(Character& character);
    void createClones();
    void updateClones(float deltaTime);
    void removeClones();
    void updateThirdSkill(float deltaTime, Character& character);
    void updateCircularAttack(float deltaTime, sf::Vector2f playerPosition);
    void drawTrails(sf::RenderTarget& target);
    //원거리 공격 로직 추가
    void updateProjectiles(float deltaTime, Character& character, MainTower& mainTower);
    void shootProjectile(const sf::Vector2f& targetPos);
    void drawProjectiles(sf::RenderTarget& target) const;
};
