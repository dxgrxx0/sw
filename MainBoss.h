#pragma once
#include "Monster.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Character;
class MainTower;

class MainBoss : public Monster {
public:
    MainBoss(float x, float y, float speed, MonsterType type);

    void update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos,
        float deltaTime, Character& character, MainTower& mainTower);
    void draw(sf::RenderTarget& target) override;

private:
    sf::Texture penTextures[7];                // 펜 텍스처 배열
    sf::Sprite penSprites[7];                  // 펜 스프라이트 배열
    sf::VertexArray drawingVertices{ sf::Quads }; // 두께가 있는 선 데이터
    std::vector<sf::Vector2f> drawingPath;     // 경로 데이터
    size_t currentPathIndex = 0;               // 현재 경로 인덱스
    float drawingDuration;                     // 전체 그림 시간 (초)
    float pointsPerSecond;                     // 초당 이동 점 수
    float elapsedDrawingTime;                  // 그림 진행 시간
	bool isDrawing = false;                    // 그리기 중인지 여부
	bool isThrowing = false;				   // 던지는 중인지 여부
	sf::Vector2f throwDirection;               // 던질 방향
	float throwSpeed = 100.0f;                  // 던지는 속도
    sf::Vector2f drawingOffset;
    bool initializeDrawingFromImage(const std::string& imagePath, float penThickness); // 이미지 기반 경로 초기화
};
