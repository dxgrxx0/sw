// WaveManager.h

#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Monster.h"
#include "Character.h"
#include "MainTower.h"


class WaveManager {
private:
    float maxDistance;
    Character* heroine;
    MainTower* mainTower;
    std::vector<std::unique_ptr<Monster>>* monsters; // 포인터로 변경

    // 스폰 속도 관련 변수
    float spawnInterval;           // 현재 스폰 간격
    float timeSinceLastSpawn = 0.0f;
    float minSpawnInterval = 0.5f; // 스폰 속도 상한 (예: 0.2초)
    float maxSpawnInterval = 2.0f; // 스폰 속도 하한 (예: 2초)

    bool midBossSpawned;           // Mid-Boss 스폰 여부
    bool mainBossSpawned;          // Main-Boss 스폰 여부

    sf::Clock gameClock;           // 게임 시간 트래킹용

    sf::Clock blinkClock; //깜빡임 효과


public:
    WaveManager(Character* heroine, MainTower* mainTower, std::vector<std::unique_ptr<Monster>>* monsters, float mapWidth, float mapHeight);
    bool loadResources();
    void update(float deltaTime);           // 매 프레임마다 호출, 스폰 및 업데이트
    void drawMonsters(sf::RenderTarget& target); // 몬스터 그리기
    bool isBossSpawned() const {
        return mainBossSpawned;
    }
private:
    void spawnMonsterAtSpecificDistance();   // 특정 거리에서 몬스터 스폰
    void spawnBoss(MonsterType bossType);    // Mid-Boss와 Main-Boss 스폰
    float calculateDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
    float calculateSpawnInterval();          // 타워와 히로인의 거리에 따라 스폰 간격 계산

    std::string bossMessage; // 현재 보스 소환 메시지
    sf::Clock messageTimer;  // 메시지 표시 타이머
    bool displayBossMessage; // 메시지 표시 여부
    sf::Font font;           // 메시지에 사용할 폰트
    sf::Text text;           // 메시지 텍스트
   
};

#endif // WAVEMANAGER_H