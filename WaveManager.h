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
    std::vector<std::unique_ptr<Monster>>* monsters; // �����ͷ� ����

    // ���� �ӵ� ���� ����
    float spawnInterval;           // ���� ���� ����
    float timeSinceLastSpawn = 0.0f;
    float minSpawnInterval = 0.5f; // ���� �ӵ� ���� (��: 0.2��)
    float maxSpawnInterval = 2.0f; // ���� �ӵ� ���� (��: 2��)

public:
    WaveManager(Character* heroine, MainTower* mainTower, std::vector<std::unique_ptr<Monster>>* monsters, float mapWidth, float mapHeight);

    void update(float deltaTime);           // �� �����Ӹ��� ȣ��, ���� �� ������Ʈ
    void drawMonsters(sf::RenderTarget& target); // ���� �׸���

private:
    void spawnMonsterAtSpecificDistance();   // Ư�� �Ÿ����� ���� ����
    float calculateDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2);
    float calculateSpawnInterval();          // Ÿ���� �������� �Ÿ��� ���� ���� ���� ���
};

#endif // WAVEMANAGER_H
