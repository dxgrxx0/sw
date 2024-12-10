#include "UpgradeManager.h"
#include <iostream>
UpgradeManager::UpgradeManager(Character* character, MainTower* mainTower,SkillManager* skillManager,int& level)
    : character(character), mainTower(mainTower),skillManager(skillManager), gameLevel(level), rng(std::random_device{}()) {
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
    if (gameLevel >= 2) {
        addOption("Upgrade BladeWhirl", [this]() { skillManager->upgradeSkill("BladeWhirl"); }, "UpgradePlayerSpeed.png");
    }
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