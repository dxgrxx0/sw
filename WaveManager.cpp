#include "WaveManager.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

WaveManager::WaveManager(Character* heroine, MainTower* mainTower, std::vector<std::unique_ptr<Monster>>* monsters, float mapWidth, float mapHeight)
    : heroine(heroine), mainTower(mainTower), monsters(monsters),gameClock(0){
    maxDistance = std::sqrt(mapWidth * mapWidth + mapHeight * mapHeight);
    spawnInterval = maxSpawnInterval; // 초기 스폰 간격은 최대값으로 설정
}

float WaveManager::calculateSpawnInterval() {
    float distance = calculateDistance(heroine->getPosition(), mainTower->getPosition());
    // 거리 비율을 사용해 스폰 간격을 동적으로 계산
    float distanceRatio = distance / maxDistance;
    if (distanceRatio > 1)distanceRatio = 1;
    return (maxSpawnInterval - distanceRatio * (maxSpawnInterval - minSpawnInterval));
}

void WaveManager::update(float deltaTime) {
    gameClock += deltaTime;


    // Mid-Boss 스폰 (5분에 등장)
    if (gameClock >= 150.0f && !midBossSpawned) {
        spawnBoss(MonsterType::Mid_Boss);
        midBossSpawned = true;
    } //(10.0f -> 300.0f)

    // Main-Boss 스폰 (10분에 등장)
    if (gameClock >= 300.0f && !mainBossSpawned) {
        spawnBoss(MonsterType::Main_Boss);
        mainBossSpawned = true;
    } //(20.0f -> 600.0f)

    if (!mainBossSpawned) { // Main-Boss가 등장하면 일반 몬스터 스폰 중지
        spawnInterval = calculateSpawnInterval();
        timeSinceLastSpawn += deltaTime;

        if (timeSinceLastSpawn >= spawnInterval) {
            spawnMonsterAtSpecificDistance();
            timeSinceLastSpawn = 0.0f;
        }
    }
    if (mainBossSpawned && mainBoss->isSpawnMidBoss() == true) {
        sf::Vector2f spawnPos = mainBoss->getPosition();
        auto boss = std::make_unique<MidBoss>(spawnPos.x, spawnPos.y, 50.0f, MonsterType::Mid_Boss);
        monsters->push_back(std::move(boss));
		mainBoss->setSpawnMidBoss(false);
        
    }
    // 몬스터 업데이트
    for (auto& monster : *monsters) {
        monster->update(heroine->getPosition(), mainTower->getPosition(), deltaTime,*heroine,*mainTower);
    }
    //if (heroine->getIsSwinging())std::cout << spawnInterval;
}

void WaveManager::spawnMonsterAtSpecificDistance() {
    sf::Vector2f heroinePos = heroine->getPosition();
    sf::Vector2f towerPos = mainTower->getPosition();

    sf::Vector2f spawnPos;
    bool validPosition = false;

    while (!validPosition) {
        float angle = static_cast<float>(rand()) / RAND_MAX * 360;
        float radianAngle = angle * 3.141592f / 180;

		int random = std::rand() % 2;
        sf::Vector2f randomPos;
		random % 2 == 0 ? randomPos = heroinePos : randomPos = towerPos;
		spawnPos = sf::Vector2f(
			randomPos.x + 1000 * std::cos(radianAngle),
			randomPos.y + 1000 * std::sin(radianAngle)
		);
		if (std::abs(calculateDistance(spawnPos, heroinePos)) > 1000 &&
			std::abs(calculateDistance(spawnPos, towerPos)) > 1000) {
			validPosition = true;
		}
        /*
        sf::Vector2f midpoint = (heroinePos + towerPos) / 2.0f;
        spawnPos = sf::Vector2f(
            midpoint.x + 1000 * std::cos(radianAngle),
            midpoint.y + 1000 * std::sin(radianAngle)
        );

        if (std::abs(calculateDistance(spawnPos, heroinePos) - 1000) < 1.0f &&
            std::abs(calculateDistance(spawnPos, towerPos) - 1000) < 1.0f) {
            validPosition = true;
        }*/
        else validPosition = false; printf("XXX\n");
    }
    MonsterType type = static_cast<MonsterType>(std::rand() % 4);
    auto monster = std::make_unique<Monster>(spawnPos.x, spawnPos.y, 50.0f, type);
    monsters->push_back(std::move(monster));
    std::cout << "Moster spawned!" << std::endl;
}

float WaveManager::calculateDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
    return std::sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y));
}

void WaveManager::drawMonsters(sf::RenderTarget& target) {
    for (const auto& monster : *monsters) {
        //target.draw(monster.getSprite());
        monster->draw(target);
    }
}
void WaveManager::spawnBoss(MonsterType bossType) {

    if (bossType != MonsterType::Mid_Boss && bossType != MonsterType::Main_Boss) {
        return;
    }

    sf::Vector2f spawnPos = { 1000.0f, 1000.0f }; // 보스는 스폰 위치
    float speed = (bossType == MonsterType::Mid_Boss) ? 70.0f : 50.0f;
    
    if (bossType == MonsterType::Mid_Boss) { 
        auto boss = std::make_unique<MidBoss>(spawnPos.x, spawnPos.y, speed, bossType); 
        monsters->push_back(std::move(boss));
    }
    else if(bossType==MonsterType::Main_Boss) { 
        auto boss = std::make_unique<MainBoss>(spawnPos.x, spawnPos.y, speed, bossType); 
        mainBoss = boss.get();
        monsters->clear();
        monsters->push_back(std::move(boss));
    }


}