#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Character.h"
class UIManager {
public:
    // UI 요소들
    //sf::RectangleShape characterHealthBar;
    sf::RectangleShape towerDurabilityBar;
    sf::Text waveNotificationText;
    sf::Text experienceText;
    sf::Text upgradeSelectionText; // 업그레이드 선택 창

    // 생성자 및 초기화
    UIManager(sf::Font& font,Character* character, sf::RenderWindow& window);

    // UI 업데이트 메서드
    void updateCharacterHealth();
    void updateTowerDurability(float currentDurability, float maxDurability);
    void updateTimer(float dt);
    void updateWaveNotification(int currentWave);
    void updateExperience(int experience);
    void showUpgradeOptions(const std::string& options);
    void updateLevelBar(int level, float experience, float experienceToNextLevel);
    // UI 요소를 렌더링하는 메서드
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
