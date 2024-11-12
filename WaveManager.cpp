#include "WaveManager.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

WaveManager::WaveManager(Character* heroine, MainTower* mainTower, std::vector<std::unique_ptr<Monster>>* monsters, float mapWidth, float mapHeight)
    : heroine(heroine), mainTower(mainTower), monsters(monsters){
    maxDistance = std::sqrt(mapWidth * mapWidth + mapHeight * mapHeight);
    spawnInterval = maxSpawnInterval; // �ʱ� ���� ������ �ִ밪���� ����
}

float WaveManager::calculateSpawnInterval() {
    float distance = calculateDistance(heroine->getPosition(), mainTower->getPosition());
    // �Ÿ� ������ ����� ���� ������ �������� ���
    float distanceRatio = distance / maxDistance;
    if (distanceRatio > 1)distanceRatio = 1;
    return maxSpawnInterval - distanceRatio * (maxSpawnInterval - minSpawnInterval);
}

void WaveManager::update(float deltaTime) {
    // ���� Ÿ���� ������ ������ �Ÿ��� ������� ���� ������ ������Ʈ
    spawnInterval = calculateSpawnInterval();
    timeSinceLastSpawn += deltaTime;

    // ���� ���ݿ� ���� ���� ����
    if (timeSinceLastSpawn >= spawnInterval) {
        spawnMonsterAtSpecificDistance();
        timeSinceLastSpawn = 0.0f;
    }

    // ���� ������Ʈ
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

        sf::Vector2f midpoint = (heroinePos + towerPos) / 2.0f;
        spawnPos = sf::Vector2f(
            midpoint.x + 1000 * std::cos(radianAngle),
            midpoint.y + 1000 * std::sin(radianAngle)
        );

        if (std::abs(calculateDistance(spawnPos, heroinePos) - 1000) < 1.0f &&
            std::abs(calculateDistance(spawnPos, towerPos) - 1000) < 1.0f) {
            validPosition = true;
        }
        validPosition = true;
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
