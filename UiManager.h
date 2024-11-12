#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Character.h"
class UIManager {
public:
    // UI ��ҵ�
    //sf::RectangleShape characterHealthBar;
    sf::RectangleShape towerDurabilityBar;
    sf::Text waveNotificationText;
    sf::Text experienceText;
    sf::Text upgradeSelectionText; // ���׷��̵� ���� â

    // ������ �� �ʱ�ȭ
    UIManager(sf::Font& font,Character* character, sf::RenderWindow& window);

    // UI ������Ʈ �޼���
    void updateCharacterHealth();
    void updateTowerDurability(float currentDurability, float maxDurability);
    void updateTimer(float dt);
    void updateWaveNotification(int currentWave);
    void updateExperience(int experience);
    void showUpgradeOptions(const std::string& options);
    void updateLevelBar(int level, float experience, float experienceToNextLevel);
    // UI ��Ҹ� �������ϴ� �޼���
    void draw(sf::RenderWindow& window);
private:
    float elapsedTime;
    int currentTime;
    Character* character;
    sf::RenderWindow& window;
    sf::RectangleShape characterHealthBarBackground;
    sf::RectangleShape characterHealthBarForeground;
    sf::Text levelText;
    sf::RectangleShape levelBarBackground;
    sf::RectangleShape levelBarForeground;
    sf::Text timerText;
    sf::RectangleShape mainTowerHealthBarBackground;
    sf::RectangleShape mainTowerHealthBarForeground;
    sf::Text towerText;
};

#endif // UIMANAGER_H
