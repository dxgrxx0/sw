#include "UpgradeManager.h"
#include <iostream>
UpgradeManager::UpgradeManager(Character* character, MainTower* mainTower)
    : character(character), mainTower(mainTower), rng(std::random_device{}()) {}

void UpgradeManager::generateUpgradeOptions() {
    currentOptions.clear();
    currentOptions.emplace_back(std::move(UpgradeOption("Increase Attack Power", [this]() { character->increaseAttackPower(10); })));
    currentOptions.emplace_back(std::move(UpgradeOption("Increase Health", [this]() { character->increaseMaxHealth(20); })));
    currentOptions.emplace_back(std::move(UpgradeOption("Reduce Skill Cooldown", [this]() { character->reduceCooldown(0.1f); })));
    //currentOptions.push_back(UpgradeOption("Increase Tower Durability", [this]() { mainTower->increaseDurability(30); }));
    //currentOptions.push_back(UpgradeOption("Enhance Tower Healing", [this]() { mainTower->increaseHealing(5); }));

    std::shuffle(currentOptions.begin(), currentOptions.end(), rng);
    if (currentOptions.size() > 3) {
        currentOptions.resize(3); // 최대 3개의 옵션만 제공
    }
}



void UpgradeManager::applyUpgrade(int choice) {
    if (choice >= 0 && choice < currentOptions.size()) {
        bool upgraded = currentOptions[choice].upgrade(); // 선택된 업그레이드 시도
        if (!upgraded) {
            std::cout << "Already at max level for this upgrade." << std::endl;
        }
    }
}


std::vector<std::string> UpgradeManager::getUpgradeDescriptions() const {
    std::vector<std::string> descriptions;
    for (const auto& option : currentOptions) {
        descriptions.push_back(option.description + " (Level " +
            std::to_string(option.currentLevel) + "/" +
            std::to_string(option.maxLevel) + ")");
    }
    return descriptions;
}
