#ifndef SUBTOWERMANAGER_H
#define SUBTOWERMANAGER_H

#include "SubTower.h"
#include <vector>

class SubTowerManager {
private:
    std::map<std::string, std::unique_ptr<SubTower>> subTowers; // 스킬 이름으로 관리
    std::vector<std::unique_ptr<SubTower>> towers;
    int maxTowers = 5; // 최대 타워 개수

public:
    void addTower(std::unique_ptr<SubTower> tower);
    void updateTowers(std::vector<std::unique_ptr<Monster>>& monsters,float deltaTime);
    void drawTowers(sf::RenderTarget& target);
    bool hasTower(const std::string& type) const;
};

#endif // SUBTOWERMANAGER_H
