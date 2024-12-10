#include "SubTowerManager.h"

void SubTowerManager::addTower(std::unique_ptr<SubTower> tower) {

    if (towers.size() < maxTowers) {
        towers.push_back(std::move(tower));
		subTowers[tower->type] = std::move(tower);
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