#pragma once
#include "Monster.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct DrawingSkillInstance {
    sf::Sprite penSprite;
    sf::VertexArray drawingVertices;
    std::vector<sf::Vector2f> drawingPath;
    size_t currentPathIndex = 0;
    float drawingDuration = 3.0f;
    float pointsPerSecond = 0.0f;
    float elapsedDrawingTime = 0.0f;

    bool isDrawing = true;
    bool isThrowing = false;

    sf::Vector2f throwDirection;
    float throwSpeed = 500.0f;
    sf::Vector2f drawingOffset;

    std::string imagePath;

    sf::FloatRect boundingBox; // 바운딩 박스
};

class MainBoss : public Monster {
public:
    MainBoss(float x, float y, float speed, MonsterType type);

    void update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos,
        float deltaTime, Character& character, MainTower& mainTower) override;
    void draw(sf::RenderTarget& target) override;
	bool isSpawnMidBoss() const { return spawnMidBoss; }
	void setSpawnMidBoss(bool spawnMidBoss) { this->spawnMidBoss = spawnMidBoss; }
    
private:
    bool initializeDrawingFromImage(DrawingSkillInstance& instance, const std::string& imagePath, int imageIndex);
    void castSkill();
    bool getIsDrawing();
    float skillCooldown = 10.0f; // 5초 주기
    float skillTimer = 0.0f;

    std::vector<std::string> penNames = { "RedPen",  "YellowPen","PurplePen", "pencil","OrangePen", "GreenPen", "BluePen", "NavyPen"};
    std::vector<std::string> imagePaths = { "MainBossRedSkill.png","MainBossYellowSkill.png","MainBossSkillPurple.png","MainBossPencilSkill.png"}; // 실제 존재하는 이미지 파일명으로 변경

    std::vector<sf::Texture> penTextures;
    std::vector<sf::Sprite> penSprites;

    std::vector<DrawingSkillInstance> skillInstances;


    std::vector<Projectile> projectiles; // 보스가 발사한 투사체 목록
    float basicAttackCooldown = 1.5f;    // 기본 공격 쿨다운
    float basicAttackTimer = 0.0f;       // 기본 공격 타이머

    void performBasicAttack(const sf::Vector2f& targetPos); // 기본 공격 함수

	bool spawnMidBoss = false; // MidBoss 스폰 여부

	const float dashDuration = 0.5f; // 대쉬 지속 시간
	float dashTimeCheck = 0.0f;         // 대쉬 타이머
    float dashCooldown = 0.0f;
	bool isDashing = false;         // 대쉬 중인지 여부
	sf::Vector2f dashDirection;     // 대쉬 방향


    float maxHealth;
    sf::RectangleShape healthBar;
	sf::RectangleShape healthBarBack;
};
