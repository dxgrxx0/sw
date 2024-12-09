#include "UpgradeManager.h"
#include <iostream>


UpgradeManager::UpgradeManager() {
    generateUpgradeOptions();
}

/*
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
}*/


std::vector<std::string> UpgradeManager::getUpgradeDescriptions() const {
    std::vector<std::string> descriptions;

    for (const auto& option : upgradeOptions) {
        descriptions.push_back(option.description + " (Level " +
            std::to_string(option.currentLevel) + "/" +
            std::to_string(option.maxLevel) + ")");
    }

    return descriptions;
}

std::vector<std::string> UpgradeManager::getUpgradeImagePaths() const {
    std::vector<std::string> imagePaths;

    for (const auto& option : upgradeOptions) {
        imagePaths.push_back(option.imagePath);
    }

    return imagePaths;
}

void UpgradeManager::generateUpgradeOptions() {
    upgradeOptions.clear();

    // 업그레이드 옵션 추가
    upgradeOptions.emplace_back("Reduce Cooldown\n", "C:\\Temp\\cooldown.png", []() {
        std::cout << "Cooldown reduced!" << std::endl;
        });


    upgradeOptions.emplace_back("Increase Health\n", "C:\\Temp\\health.png", []() {
        std::cout << "Health increased!" << std::endl;
        });

    upgradeOptions.emplace_back("Increase Attack Power\n", "C:\\Temp\\power.png", []() {
        std::cout << "Attack power increased!" << std::endl;
        });
    

    
}


void UpgradeManager::applyUpgrade(int index) {
    if (index >= 0 && index < upgradeOptions.size()) {
        upgradeOptions[index].applyUpgrade();
    }
}

//
//std::vector<std::string> UpgradeManager::getUpgradeDescriptions() const {
//    std::vector<std::string> descriptions;
//    for (const auto& option : currentOptions) {
//        descriptions.push_back(option->description + " (Level " +
//            std::to_string(option->currentLevel) + "/" +
//            std::to_string(option->maxLevel) + ")");
//    }
//    return descriptions;
//}