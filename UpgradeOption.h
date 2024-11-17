#ifndef UPGRADEOPTION_H
#define UPGRADEOPTION_H

#include <string>
#include <functional>

struct UpgradeOption {
    std::string description;                  // 업그레이드 설명
    std::function<void()> applyUpgrade;       // 업그레이드 효과 적용 함수
    int currentLevel;                         // 현재 레벨
    const int maxLevel = 3;                   // 최대 레벨

    // 기본 생성자
    UpgradeOption() : currentLevel(0) {}

    // 생성자
    UpgradeOption(const std::string& desc, std::function<void()> func)
        : description(desc), applyUpgrade(func), currentLevel(0) {}

    // 복사 생성자
    UpgradeOption(const UpgradeOption& other)
        : description(other.description)
        , applyUpgrade(other.applyUpgrade)
        , currentLevel(other.currentLevel) {}

    // 이동 생성자
    UpgradeOption(UpgradeOption&& other) noexcept
        : description(std::move(other.description))
        , applyUpgrade(std::move(other.applyUpgrade))
        , currentLevel(other.currentLevel) {}

    // 복사 할당 연산자
    UpgradeOption& operator=(const UpgradeOption& other) {
        if (this != &other) {
            description = other.description;
            applyUpgrade = other.applyUpgrade;
            currentLevel = other.currentLevel;
        }
        return *this;
    }

    // 이동 할당 연산자
    UpgradeOption& operator=(UpgradeOption&& other) noexcept {
        if (this != &other) {
            description = std::move(other.description);
            applyUpgrade = std::move(other.applyUpgrade);
            currentLevel = other.currentLevel;
        }
        return *this;
    }

    // 업그레이드 적용 함수
    bool upgrade() {
        if (currentLevel < maxLevel) {
            applyUpgrade();
            ++currentLevel;
            return true;
        }
        return false; // 최대 레벨에 도달한 경우 업그레이드 불가
    }
};

#endif // UPGRADEOPTION_H