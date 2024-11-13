#ifndef UPGRADEMANAGER_H
#define UPGRADEMANAGER_H

#include <vector>
#include <random>
#include "UpgradeOption.h"
#include "Character.h"
#include "MainTower.h"

class UpgradeManager {
private:
    Character* character;
    MainTower* mainTower;
    std::map<std::string, UpgradeOption> upgradeOptions; // ���׷��̵� ���� ����
    std::vector<UpgradeOption> currentOptions;// ���� ǥ���� �ɼ�
    std::mt19937 rng;

public:
    UpgradeManager(Character* character, MainTower* mainTower);

    void generateUpgradeOptions();                  // ���׷��̵� �ɼ� ����
    void applyUpgrade(int choice);                  // ���õ� ���׷��̵� �ɼ� ����
    std::vector<std::string> getUpgradeDescriptions() const; // ���׷��̵� ���� ��ȯ
};
#endif // UPGRADEMANAGER_H