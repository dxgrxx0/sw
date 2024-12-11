#include "UIManager.h"

UIManager::UIManager(sf::Font& font, Character* character, sf::RenderWindow& window)
    : character(character), window(window) {
    // 캐릭터 체력바 초기화
    characterHealthBarBackground.setSize(sf::Vector2f(48, 10));
    characterHealthBarBackground.setFillColor(sf::Color(50, 50, 50));
    characterHealthBarBackground.setOutlineThickness(1);
    characterHealthBarBackground.setOutlineColor(sf::Color::White);
    characterHealthBarBackground.setPosition(character->getPosition().x - 24, character->getPosition().y - 55);
    characterHealthBarForeground.setFillColor(sf::Color::Green);
    characterHealthBarForeground.setPosition(characterHealthBarBackground.getPosition().x + 1, characterHealthBarBackground.getPosition().y + 1);

    // 레벨 바 초기화
    levelBarBackground.setSize(sf::Vector2f(window.getSize().x / 2, 30));
    levelBarBackground.setFillColor(sf::Color(50, 50, 50));
    levelBarBackground.setOutlineThickness(2);
    levelBarBackground.setOutlineColor(sf::Color(101, 67, 33));

    levelText.setFont(font);
    levelText.setCharacterSize(20);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(0, 0);

    // 메인 타워 내구도 바 초기화
    mainTowerHealthBarBackground.setSize(sf::Vector2f(window.getSize().x / 2, 30));
    mainTowerHealthBarBackground.setFillColor(sf::Color(50, 50, 50));
    mainTowerHealthBarBackground.setOutlineThickness(2);
    mainTowerHealthBarBackground.setOutlineColor(sf::Color(101, 67, 33));

    towerText.setFont(font);
    towerText.setCharacterSize(20);
    towerText.setFillColor(sf::Color::White);
    towerText.setString("Main tower HP");

    currentTime = 0;
    elapsedTime = 0;

    // 타이머 텍스트 초기화
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(800, 20);

    // 스킬 텍스트 초기화
    skillText.setFont(font);
    skillPositions["BladeWhirl"] = sf::Vector2i(window.getSize().x - 130, 600);
    skillPositions["BulkUp"] = sf::Vector2i(window.getSize().x - 130, 700);
    skillPositions["Dash"] = sf::Vector2i(window.getSize().x - 130, 800);
    skillPositions["Teleport"] = sf::Vector2i(window.getSize().x - 130, 900);
}

// 캐릭터 체력바 업데이트
void UIManager::updateCharacterHealth() {
    float healthPercentage = character->getHealth() / character->getMaxHealth();
    characterHealthBarBackground.setPosition(character->getPosition().x - 24, character->getPosition().y - 55);
    characterHealthBarForeground.setSize(sf::Vector2f(46 * healthPercentage, 8));
    characterHealthBarForeground.setPosition(characterHealthBarBackground.getPosition().x + 1, characterHealthBarBackground.getPosition().y + 1);
}

// 레벨 바 업데이트
void UIManager::updateLevelBar(int level, float experience, float experienceToNextLevel) {
    levelBarBackground.setPosition(window.mapPixelToCoords(sf::Vector2i(0, 5)));
    float levelPercentage = experience / experienceToNextLevel;
    levelBarForeground.setSize(sf::Vector2f((window.getSize().x / 2) * levelPercentage, 20));
    levelBarForeground.setFillColor(sf::Color(135, 206, 250));
    levelBarForeground.setPosition(levelBarBackground.getPosition().x, levelBarBackground.getPosition().y + 5);
    levelText.setString("Level: " + std::to_string(level));
    levelText.setPosition(levelBarBackground.getPosition().x + 5, levelBarBackground.getPosition().y + 5);
}

// 타워 내구도 바 업데이트
void UIManager::updateTowerDurability(float currentDurability, float maxDurability) {
    float durabilityPercentage = currentDurability / maxDurability;
    mainTowerHealthBarBackground.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, 5)));
    mainTowerHealthBarForeground.setSize(sf::Vector2f((window.getSize().x / 2 - 3) * durabilityPercentage, 20));
    mainTowerHealthBarForeground.setFillColor(sf::Color::Red);
    mainTowerHealthBarForeground.setPosition(mainTowerHealthBarBackground.getPosition().x + 3, mainTowerHealthBarBackground.getPosition().y + 5);
    towerText.setPosition(mainTowerHealthBarBackground.getPosition().x + 5, mainTowerHealthBarBackground.getPosition().y + 5);
}

// 타이머 업데이트
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

// 스킬 쿨타임 업데이트
void UIManager::updateSkillCoolTime(SkillManager& skillManager) {
    for (const auto& skillPair : skillPositions) {
        const std::string& skillName = skillPair.first;
        const sf::Vector2i skillPosition = skillPair.second;

        // 스킬 상태 정보 가져오기
        bool isUnlocked = skillManager.isSkillUnlocked(skillName);
        float remainingCooldown = skillManager.getRemainingCooldown(skillName);

        // 사각형 배경 설정
        skillBoxBackground.setSize(sf::Vector2f(window.getSize().x * 3 / 40, window.getSize().y * 9 / 100));
        skillBoxBackground.setPosition(window.mapPixelToCoords(skillPosition));
        skillBoxBackground.setFillColor(isUnlocked ? sf::Color(50, 50, 50) : sf::Color(100, 100, 100));
        window.draw(skillBoxBackground);

        // 텍스트 설정
        std::string text;
        if (!isUnlocked) {
            text = "Locked";
            skillText.setFillColor(sf::Color(200, 50, 50));
        }
        else if (remainingCooldown > 0) {
            text = skillName + "\n" + std::to_string(static_cast<int>(remainingCooldown)) + "s";
            skillText.setFillColor(sf::Color::White);
        }
        else {
            text = skillName + "\nReady";
            skillText.setFillColor(sf::Color::Green);
        }

        skillText.setString(text);
        skillText.setCharacterSize(20);
        skillText.setPosition(window.mapPixelToCoords(sf::Vector2i(skillPosition.x + 10, skillPosition.y + 5)));
        window.draw(skillText);
    }
}

// UI 요소 그리기
void UIManager::draw(sf::RenderWindow& window) {
    window.draw(characterHealthBarBackground);
    window.draw(characterHealthBarForeground);
    window.draw(levelBarBackground);
    window.draw(levelBarForeground);
    window.draw(mainTowerHealthBarBackground);
    window.draw(mainTowerHealthBarForeground);
    window.draw(levelText);
    window.draw(towerText);
    window.draw(timerText);
}
