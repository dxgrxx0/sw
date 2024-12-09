#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include <vector>
#include <random>
#include "UpgradeOption.h"
#include "Character.h"
#include "MainTower.h"
#include <map>
#include <memory>
#include "Monster.h"
#include"SubTowerManager.h"
class UpgradeManager {
private:
    Character* character;
    MainTower* mainTower;
    std::vector<std::unique_ptr<Monster>>& monsters; 
    std::map<std::string, UpgradeOption> upgradeOptions; // 업그레이드 상태 저장
    std::vector<UpgradeOption*> currentOptions;// 현재 표시할 옵션
    std::mt19937 rng;
    std::string getTowerName(const std::function<std::unique_ptr<SubTower>()>& creator);

    int* currentLevel;  // level 포인터 추가
    int defaultLevel;
    SubTowerManager* subTowerManager;
public:
    UpgradeManager(Character* character, MainTower* mainTower, std::vector<std::unique_ptr<Monster>>& monsters,int *currentLevel);

    void generateUpgradeOptions();                  // 업그레이드 옵션 생성
    void applyUpgrade(int choice);                  // 선택된 업그레이드 옵션 적용
    std::vector<std::string> getUpgradeDescriptions() const; // 업그레이드 설명 반환

    void generateSubTowerUpgradeOptions();
};
#endif // UPGRADEMANAGER_H