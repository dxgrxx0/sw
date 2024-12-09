#include "UpgradeManager.h"
#include"BladeWhirl.h"
#include"BulkUp.h"
#include <iostream>
#include"SubTower.h"
#include "ArrowTower.h"
#include "WizardTower.h"
#include "TrainingTower.h"
#include "BombTower.h"
#include "CannonTower.h"

UpgradeManager::UpgradeManager(Character* character, MainTower* mainTower, std::vector<std::unique_ptr<Monster>>& monsters,int *currentLevel)
    : character(character), monsters(monsters), mainTower(mainTower), rng(std::random_device{}()), currentLevel(currentLevel ? currentLevel : &defaultLevel)
{
    character->addSkill(std::make_unique<BladeWhirl>(character, monsters));
    character->addSkill(std::make_unique<BulkUp>(character));
    defaultLevel = 1;
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
void UpgradeManager::generateUpgradeOptions() {
    currentOptions.clear();

    auto addOption = [&](const std::string& desc, std::function<void()> func, int requiredLevel) {
        // 현재 레벨이 요구 레벨보다 높거나 같을 때만 옵션 추가
        printf("level = %d\n", *currentLevel);

        if (*currentLevel >= requiredLevel) {
            if (upgradeOptions.find(desc) == upgradeOptions.end()) {
                upgradeOptions[desc] = UpgradeOption(desc, func);
            }
            if (upgradeOptions[desc].currentLevel < upgradeOptions[desc].maxLevel) {
                currentOptions.push_back(&upgradeOptions[desc]);
            }
        }
    };
    addOption("Increase Attack Power", [this]() { character->increaseAttackPower(10); },2);
    addOption("Increase Health", [this]() { character->increaseMaxHealth(20); },2);
    addOption("Reduce Skill Cooldown", [this]() { character->reduceCooldown(0.1f); },2);
    addOption("Increase Heroine Speed", [this]() {character->increaseSpeed(30.0f); },2);
    
    
    // 스킬 업그레이드
    addOption("BladeWhirl Upgrade", [this]() {
        BladeWhirl* bladeWhirl = character->getSkill<BladeWhirl>();
        if (bladeWhirl) {
            std::cout << "Before Upgrade - Base: " << bladeWhirl->getBaseDamage()
                << ", Additional: " << bladeWhirl->getAdditionalDamage() << std::endl;
            bladeWhirl->upgradeDamage(30.0f);  // 데미지 30 증가
            bladeWhirl->reduceSkillCooldown(0.2f);  // 쿨타임 0.2초 감소

            std::cout << "After Upgrade - Base: " << bladeWhirl->getBaseDamage()
                << ", Additional: " << bladeWhirl->getAdditionalDamage()
                << ", Total: " << bladeWhirl->getCurrentDamage() << std::endl;
        } 
        else {
            std::cout << "BladeWhirl Not Found!" << std::endl;
        }
        },3);

    addOption("BulkUp Upgrade", [this]() {
        BulkUp* bulkUp = character->getSkill<BulkUp>();
        if (bulkUp) {
            bulkUp->increaseDuration(10.0f);  // 지속 시간 1초 증가
            bulkUp->coolTimeDown(0.2f);  // 쿨 다운
        }
        },4);
    
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
        descriptions.push_back(option->description + " (Level " +
            std::to_string(option->currentLevel) + "/" +
            std::to_string(option->maxLevel) + ")");
    }
    return descriptions;
}

std::string UpgradeManager::getTowerName(const std::function<std::unique_ptr<SubTower>()>& creator) {
    auto tower = creator();
    if (dynamic_cast<WizardTower*>(tower.get())) return "Wizard";
    if (dynamic_cast<BombTower*>(tower.get())) return "Bomb";
    if (dynamic_cast<TrainingTower*>(tower.get())) return "Training";
    if (dynamic_cast<ArrowTower*>(tower.get())) return "Arrow";
    if (dynamic_cast<CannonTower*>(tower.get())) return "Cannon";
    return "Unknown";
}

void UpgradeManager::generateSubTowerUpgradeOptions() {
    currentOptions.clear();

    // 사용 가능한 타워 타입 정의
    std::vector<std::function<std::unique_ptr<SubTower>()>> towerCreators = {
        []() { return std::make_unique<WizardTower>(sf::Vector2f(650, 326)); },
        []() { return std::make_unique<BombTower>(sf::Vector2f(950, 326)); },
        []() { return std::make_unique<TrainingTower>(sf::Vector2f(250, 846)); },
        []() { return std::make_unique<ArrowTower>(sf::Vector2f(250, 846)); },
        []() { return std::make_unique<CannonTower>(sf::Vector2f(1050, 846)); }
    };

    // 랜덤하게 타워 선택 옵션 생성
    std::shuffle(towerCreators.begin(), towerCreators.end(), rng);

    // 최대 3개의 타워 선택 옵션 생성
    for (int i = 0; i < std::min(3, static_cast<int>(towerCreators.size())); ++i) {
        std::string towerName = getTowerName(towerCreators[i]);
        currentOptions.emplace_back(new UpgradeOption(
            "Build " + towerName + " Tower",
            [this, creator = towerCreators[i]]() {
                subTowerManager->addTower(creator());
            }
        ));
    }
}