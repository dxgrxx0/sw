#include "UpgradeManager.h"
#include <iostream>
UpgradeManager::UpgradeManager(Character* character, MainTower* mainTower,SkillManager* skillManager,SubTowerManager* subTowerManager,int& level)
    : character(character), mainTower(mainTower),skillManager(skillManager),subTowerManager(subTowerManager), gameLevel(level), rng(std::random_device{}()) {
}
void UpgradeManager::createSubTowerOptions() {
    currentOptions.clear();
	auto addOption = [&](const std::string& desc, std::function<std::string()> func, const std::string& imgPath) {
		if (upgradeOptions.find(desc) == upgradeOptions.end()) {
			upgradeOptions[desc] = UpgradeOption(desc, imgPath, func);
		}
		if (upgradeOptions[desc].currentLevel < upgradeOptions[desc].maxLevel) {
			currentOptions.push_back(&upgradeOptions[desc]); // 포인터 추가
		}
		};
    if (!(subTowerManager->hasTower("WizardTower")))addOption("Build WizardTower", [this]() { return "WizardTower"; }, "UpgradeWizardTower.png");
    if (!(subTowerManager->hasTower("ArrowTower")))addOption("Build ArrowTower", [this]() { return "ArrowTower"; }, "UpgradeArrowTower.png");
    if (!(subTowerManager->hasTower("TrainingTower")))addOption("Build TrainingTower", [this]() { return "TrainingTower"; }, "UpgradeTrainingTower.png");
    if (!(subTowerManager->hasTower("CannonTower")))addOption("Build CannonTower", [this]() { return "CannonTower"; }, "UpgradeCannonTower.png");
    if (!(subTowerManager->hasTower("BombTower")))addOption("Build BombTower", [this]() { return "BombTower"; }, "UpgradeBombTower.png");
	std::shuffle(currentOptions.begin(), currentOptions.end(), rng);

	if (currentOptions.size() > 3) {
		currentOptions.resize(3);
	}
	else if (currentOptions.size() < 3) {
		currentOptions.resize(currentOptions.size());
	}
}
std::string UpgradeManager::getTowerName(int choice) const {
	if (choice >= 0 && choice < currentOptions.size()) {
		return currentOptions[choice]->getTowerName();
	}
	return "";
}
void UpgradeManager::generateUpgradeOptions() {
    currentOptions.clear();

    auto addOption = [&](const std::string& desc, std::function<void()> func,const std::string& imgPath) {
        if (upgradeOptions.find(desc) == upgradeOptions.end()) {
            upgradeOptions[desc] = UpgradeOption(desc,imgPath, func);
        }
        if (upgradeOptions[desc].currentLevel < upgradeOptions[desc].maxLevel) {
            currentOptions.push_back(&upgradeOptions[desc]); // 포인터 추가
        }
    };

    addOption("Increase Attack Power", [this]() { character->increaseAttackPower(10); },"UpgradeAttackPower.png");
    addOption("Increase Health", [this]() { character->increaseMaxHealth(20); },"UpgradeHealth.png");
    addOption("Reduce Skill Cooldown", [this]() { character->reduceCooldown(0.1f); },"UpgradeBasicCooldown.png");
    addOption("Increase Heroine Speed", [this]() {character->increaseSpeed(30.0f); },"UpgradePlayerSpeed.png");
    addOption("Upgrade MainTower", [this]() { mainTower->upgrade(); }, "UpgradeMainTower.png");
    if (gameLevel >= 3) {
        addOption("Upgrade BladeWhirl", [this]() { skillManager->upgradeSkill("BladeWhirl"); }, "UpgradeBladeWhirl.png");
    }
    if (gameLevel >= 6) {
		addOption("Upgrade BulkUp", [this]() { skillManager->upgradeSkill("BulkUp"); }, "UpgradeBulkUp.png");
    }
    if (gameLevel >= 9) {
		addOption("Upgrade Dash", [this]() { skillManager->upgradeSkill("Dash"); }, "UpgradeDash.png");
    }
	if (subTowerManager->hasTower("WizardTower"))addOption("Upgrade WizardTower", [this]() { subTowerManager->upgradeTower("WizardTower"); }, "UpgradeWizardTower.png");
	if (subTowerManager->hasTower("ArrowTower"))addOption("Upgrade ArrowTower", [this]() { subTowerManager->upgradeTower("ArrowTower"); }, "UpgradeArrowTower.png");
	if (subTowerManager->hasTower("TrainingTower"))addOption("Upgrade TrainingTower", [this]() { subTowerManager->upgradeTower("TrainingTower"); }, "UpgradeTrainingTower.png");
	if (subTowerManager->hasTower("CannonTower"))addOption("Upgrade CannonTower", [this]() { subTowerManager->upgradeTower("CannonTower"); }, "UpgradeCannonTower.png");
	if (subTowerManager->hasTower("BombTower"))addOption("Upgrade BombTower", [this]() { subTowerManager->upgradeTower("BombTower"); }, "UpgradeBombTower.png");
    std::shuffle(currentOptions.begin(), currentOptions.end(), rng);

    if (currentOptions.size() > 3) {
        currentOptions.resize(3);
    }
    else if (currentOptions.size() < 3) {
        currentOptions.resize(currentOptions.size());
    }
}



void UpgradeManager::applyUpgrade(int choice) {
    if (choice >= 0 && choice < currentOptions.size()) {
        bool upgraded = currentOptions[choice]->upgrade(); // 선택된 업그레이드 시도
        if (!upgraded) {
            std::cout << "Already at max level for this upgrade." << std::endl;
        }
    }
}


std::vector<std::string> UpgradeManager::getUpgradeDescriptions() const {
    std::vector<std::string> descriptions;
    for (const auto& option : currentOptions) {
        descriptions.push_back(option->description + " \n(Level " +
            std::to_string(option->currentLevel) + "/" +
            std::to_string(option->maxLevel) + ")");
    }
    return descriptions;
}
std::vector<std::string> UpgradeManager::getUpgradeImagePaths() const {
    std::vector<std::string> imagePaths;

    for (const auto& option : currentOptions) {
        imagePaths.push_back(option->imagePath);
    }

    return imagePaths;
}