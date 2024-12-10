#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include <vector>
#include <random>
#include "UpgradeOption.h"
#include "Character.h"
#include "MainTower.h"
#include "SkillManager.h"
#include "SubTowerManager.h"
class UpgradeManager {
private:
    Character* character;
    MainTower* mainTower;
	SkillManager* skillManager;
    SubTowerManager* subTowerManager;
    std::map<std::string, UpgradeOption> upgradeOptions; // 업그레이드 상태 저장
    std::vector<UpgradeOption*> currentOptions;// 현재 표시할 옵션
    std::mt19937 rng;
    int& gameLevel;
public:
    UpgradeManager(Character* character, MainTower* mainTower,SkillManager* skillManager,SubTowerManager* subTowerManager,int& level);
	void createSubTowerOptions();                   // 서브 타워 옵션 생성
    void generateUpgradeOptions();                  // 업그레이드 옵션 생성
    void applyUpgrade(int choice);                  // 선택된 업그레이드 옵션 적용
	std::string getTowerName(int choice) const;     // 선택된 타워 이름 반환
    std::vector<std::string> getUpgradeDescriptions() const; // 업그레이드 설명 반환
	std::vector<std::string> getUpgradeImagePaths() const; // 업그레이드 이미지 경로 반환
};
#endif // UPGRADEMANAGER_H