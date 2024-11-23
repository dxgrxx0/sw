#ifndef MONSTER_H

#define MONSTER_H

#include <SFML/Graphics.hpp>
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
    // ������
    Monster(float x, float y, float speed, MonsterType type);

    // ���� ������Ʈ �Լ� (��ǥ ��ġ�� �̵�)
    void update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos, float deltaTime, Character& character, MainTower& mainTower);

    // ���͸� �����쿡 �׸��� �Լ�
    void draw(sf::RenderTarget& target)const;

    // ������ ���� ��ġ ��ȯ �Լ�
    sf::Vector2f getPosition();

    // Ư�� ��ġ ��ó�� �ִ��� Ȯ���ϴ� �Լ� (�̴ϸ� Ȱ�� ��)
    bool isNear(sf::Vector2f position, float radius) const;


    void takeDamage(float attackDamage);
    float getHealthPoint()const;


    void Fir_useSkill(Character& character, MainTower& mainTower);
    void Sec_useSkill(Character& character, MainTower& mainTower);

    void createClones();
    void updateClones(float deltaTime);
    void removeClones();


    //원거리 공격 로직 추가
    void updateProjectiles(float deltaTime, Character& character, MainTower& mainTower);
    void shootProjectile(const sf::Vector2f& targetPos);
    void drawProjectiles(sf::RenderTarget& target) const;


    MonsterType getBossMonsterType() const {
        if (texturePath == "mainboss.PNG") {
            return MonsterType::Main_Boss;
        }
        else   
            return MonsterType::Mid_Boss;
        
       
    }
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
    float attackRange;
    sf::Clock attackTimer; // 공격 타이머 추가
    float attackCooldown = 1.0f; // 1초 주기

    std::string texturePath;
    sf::Texture texture;
    sf::Sprite sprite;

    //mid,main boss skill variable
    sf::Clock skillCooldown;
    sf::Clock rangedAttackCooldown;
    float skillDuration;
    bool isSkillActive;
    float originalSpeed;
    float originalDefense;
    float originalAttackPower;
    float originalattackRange;

    //void applySkillEffects();
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
};

#endif // MONSTER_H