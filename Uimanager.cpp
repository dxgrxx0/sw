#include "UIManager.h"

UIManager::UIManager(sf::Font& font,Character* character, sf::RenderWindow& window):character(character),window(window) {
    // ĳ���� ü�¹� �ʱ�ȭ
    characterHealthBarBackground.setSize(sf::Vector2f(48, 10));
    characterHealthBarBackground.setFillColor(sf::Color(50, 50, 50));
    characterHealthBarBackground.setOutlineThickness(1);
    characterHealthBarBackground.setOutlineColor(sf::Color::White);
    characterHealthBarBackground.setPosition(character->getPosition().x-24, character->getPosition().y - 55);
    characterHealthBarForeground.setFillColor(sf::Color::Green);  // ����� ����
    characterHealthBarForeground.setPosition(characterHealthBarBackground.getPosition().x + 1, characterHealthBarBackground.getPosition().y + 1);


    levelBarBackground.setSize(sf::Vector2f(window.getSize().x/2, 30));
    levelBarBackground.setFillColor(sf::Color(50, 50, 50));
    levelBarBackground.setOutlineThickness(2);
    levelBarBackground.setOutlineColor(sf::Color(101, 67, 33));

    levelText.setFont(font);
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(0, 0);

    currentTime = 0;
    elapsedTime = 0;

    // Ÿ�̸� �ؽ�Ʈ �ʱ�ȭ
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(800, 20);

}

// ĳ���� ü�¹� ������Ʈ
void UIManager::updateCharacterHealth() {
    float healthPercentage = character->getHealth() / character->getMaxHealth();
    characterHealthBarBackground.setPosition(character->getPosition().x - 24, character->getPosition().y - 55);
    characterHealthBarForeground.setSize(sf::Vector2f(46 * healthPercentage, 8));
    characterHealthBarForeground.setPosition(characterHealthBarBackground.getPosition().x + 1, characterHealthBarBackground.getPosition().y + 1);
}
void UIManager::updateLevelBar(int level,float experience,float experienceToNextLevel) {
    levelBarBackground.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, 5)));
    float levelPercentage = experience / experienceToNextLevel;
    levelBarForeground.setSize(sf::Vector2f((window.getSize().x/2) * levelPercentage, 20));
    levelBarForeground.setFillColor(sf::Color(135, 206, 250));
    levelBarForeground.setPosition(levelBarBackground.getPosition().x + 5, levelBarBackground.getPosition().y + 5);
    levelText.setString("Level: " + std::to_string(level));

    float textWidth = levelText.getLocalBounds().width;
    levelText.setPosition(levelBarBackground.getPosition().x - textWidth - 15, levelBarBackground.getPosition().y + 5);
}

// Ÿ�� ������ �� ������Ʈ
void UIManager::updateTowerDurability(float currentDurability, float maxDurability) {
    float durabilityPercentage = currentDurability / maxDurability;
    towerDurabilityBar.setSize(sf::Vector2f(100.0f * durabilityPercentage, 10.0f));
}

// Ÿ�̸� ������Ʈ
void UIManager::updateTimer(float dt) {
    elapsedTime += dt;
    if (currentTime != (int)elapsedTime) {
        currentTime = (int)elapsedTime;
        int minutes = currentTime / 60;
        int seconds = currentTime % 60;
        timerText.setString("Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds));
    }
    float textWidth = timerText.getLocalBounds().width;
    timerText.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2 - textWidth / 2, 50)));
}


// UI ��Ҹ� �׸���
void UIManager::draw(sf::RenderWindow& window) {
    window.draw(characterHealthBarBackground);
    window.draw(characterHealthBarForeground);
    window.draw(levelBarBackground);
    window.draw(levelBarForeground);
    window.draw(levelText);
    window.draw(timerText);
}
