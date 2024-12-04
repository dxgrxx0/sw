#ifndef SUBTOWERMANAGER_H
#define SUBTOWERMANAGER_H

#include "SubTower.h"
#include <vector>

class SubTowerManager {
private:
    std::vector<std::unique_ptr<SubTower>> towers;
    int maxTowers = 5; // 최대 타워 개수

public:
    void addTower(std::unique_ptr<SubTower> tower);
    void updateTowers(std::vector<std::unique_ptr<Monster>>& monsters,float deltaTime);
    void drawTowers(sf::RenderTarget& target);
};

#endif // SUBTOWERMANAGER_H
