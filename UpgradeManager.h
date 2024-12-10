#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include <vector>
#include <random>
#include "UpgradeOption.h"
#include "Character.h"
#include "MainTower.h"
#include "SkillManager.h"
class UpgradeManager {
private:
    Character* character;
    MainTower* mainTower;
	SkillManager* skillManager;
    std::map<std::string, UpgradeOption> upgradeOptions; // 업그레이드 상태 저장
    std::vector<UpgradeOption*> currentOptions;// 현재 표시할 옵션
    std::mt19937 rng;
    int& gameLevel;
public:
    UpgradeManager(Character* character, MainTower* mainTower,SkillManager* skillManager,int& level);

    void generateUpgradeOptions();                  // 업그레이드 옵션 생성
    void applyUpgrade(int choice);                  // 선택된 업그레이드 옵션 적용
    std::vector<std::string> getUpgradeDescriptions() const; // 업그레이드 설명 반환
	std::vector<std::string> getUpgradeImagePaths() const; // 업그레이드 이미지 경로 반환
};
#endif // UPGRADEMANAGER_H