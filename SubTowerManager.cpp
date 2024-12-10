#include "SubTowerManager.h"

void SubTowerManager::addTower(std::unique_ptr<SubTower> tower) {

    if (towers.size() < maxTowers) {
        std::string type = tower->type; // 소유권 이전 전에 type 접근
        subTowers[type] = tower.get(); // raw 포인터 저장
        towers.push_back(std::move(tower)); // 소유권 이동
    }
}

void SubTowerManager::updateTowers(std::vector<std::unique_ptr<Monster>>& monsters,float deltaTime) {
    for (auto& tower : towers) {
        tower->attack(monsters,deltaTime);
    }
}

void SubTowerManager::drawTowers(sf::RenderTarget& target) {
    for (auto& tower : towers) {
        tower->draw(target);
    }
}

bool SubTowerManager::hasTower(const std::string& type) const {
    return subTowers.find(type) != subTowers.end();
}
void SubTowerManager::upgradeTower(const std::string& type) {
	if (hasTower(type)) {
		subTowers[type]->upgrade();
	}
}
bool SubTowerManager::isCorrectPos(sf::Vector2f pos) {
    for (auto& tower : towers) {
        if (calculateDistance(tower->getPosition(), pos) < 300)return false;
    }
    return true;
}