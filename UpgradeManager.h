#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include <vector>
#include <random>
#include "UpgradeOption.h"
#include "Character.h"
#include "MainTower.h"

class UpgradeManager {
private:
    //Character* character;
    //MainTower* mainTower;
    //std::map<std::string, UpgradeOption> upgradeOptions; // 업그레이드 상태 저장
    std::vector<UpgradeOption> upgradeOptions; // 생성된 업그레이드 옵션들

    std::mt19937 rng;

public:
    
    UpgradeManager();
    void generateUpgradeOptions();
    //const std::vector<UpgradeOption>& getUpgradeOptions() const;
    void applyUpgrade(int index);
    std::vector<std::string> getUpgradeDescriptions() const;   // 옵션 설명 반환
    std::vector<std::string> getUpgradeImagePaths() const;     // 이미지 경로 반환


};
#endif // UPGRADEMANAGER_H