#ifndef UPGRADEOPTION_H
#define UPGRADEOPTION_H

#include <string>
#include <functional>

struct UpgradeOption {
    std::string description;                  // ���׷��̵� ����
    std::function<void()> applyUpgrade;       // ���׷��̵� ȿ�� ���� �Լ�
    int currentLevel;                         // ���� ����
    const int maxLevel = 3;                   // �ִ� ����

    // �⺻ ������
    UpgradeOption() : currentLevel(0) {}

    // ������
    UpgradeOption(const std::string& desc, std::function<void()> func)
        : description(desc), applyUpgrade(func), currentLevel(0) {}

    // ���� ������
    UpgradeOption(const UpgradeOption& other)
        : description(other.description)
        , applyUpgrade(other.applyUpgrade)
        , currentLevel(other.currentLevel) {}

    // �̵� ������
    UpgradeOption(UpgradeOption&& other) noexcept
        : description(std::move(other.description))
        , applyUpgrade(std::move(other.applyUpgrade))
        , currentLevel(other.currentLevel) {}

    // ���� �Ҵ� ������
    UpgradeOption& operator=(const UpgradeOption& other) {
        if (this != &other) {
            description = other.description;
            applyUpgrade = other.applyUpgrade;
            currentLevel = other.currentLevel;
        }
        return *this;
    }

    // �̵� �Ҵ� ������
    UpgradeOption& operator=(UpgradeOption&& other) noexcept {
        if (this != &other) {
            description = std::move(other.description);
            applyUpgrade = std::move(other.applyUpgrade);
            currentLevel = other.currentLevel;
        }
        return *this;
    }

    // ���׷��̵� ���� �Լ�
    bool upgrade() {
        if (currentLevel < maxLevel) {
            applyUpgrade();
            ++currentLevel;
            return true;
        }
        return false; // �ִ� ������ ������ ��� ���׷��̵� �Ұ�
    }
};

#endif // UPGRADEOPTION_H