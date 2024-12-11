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

	QSkillSprite.setTexture(ResourceManager::getInstance().getTexture("QSkillUI"));
	WSkillSprite.setTexture(ResourceManager::getInstance().getTexture("WSkillUI"));
	ESkillSprite.setTexture(ResourceManager::getInstance().getTexture("ESkillUI"));
	RSkillSprite.setTexture(ResourceManager::getInstance().getTexture("RSkillUI"));
    
	skillLockSprite.setTexture(ResourceManager::getInstance().getTexture("SkillLock"));
    skillLockSprite.setColor(sf::Color(255, 255, 255, 128));
	cooldownOverlay.setFillColor(sf::Color(0, 0, 0, 128));
    updateSkillIconPositions();
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
    std::vector<std::pair<std::string, sf::Sprite*>> skills = {
        {"BladeWhirl", &QSkillSprite},
        {"BulkUp", &WSkillSprite},
        {"Dash", &ESkillSprite},
        {"Teleport", &RSkillSprite}
    };

    for (const auto& skillPair : skills) {
        const std::string& skillName = skillPair.first;
        sf::Sprite* skillSprite = skillPair.second;

        // 스킬 상태 정보 가져오기
        bool isUnlocked = skillManager.isSkillUnlocked(skillName);
        float remainingCooldown = skillManager.getRemainingCooldown(skillName);
        float maxCooldown = skillManager.getSkillMaxCooldown(skillName); // 스킬 최대 쿨타임

        // 스킬 아이콘 그리기
        window.draw(*skillSprite);

        // 스킬이 잠겨있으면 SkillLock 이미지 덮기
        if (!isUnlocked) {
            skillLockSprite.setPosition(skillSprite->getPosition());
            skillLockSprite.setOrigin(skillLockSprite.getGlobalBounds().width / 2, skillLockSprite.getGlobalBounds().height / 2);
            skillLockSprite.setScale(skillSprite->getScale()); // 아이콘 크기와 동일하게 설정
            window.draw(skillLockSprite);
        }

        // 스킬이 쿨타임 중이면 쿨타임 오버레이 그리기
        else if (remainingCooldown > 0.0f) {
            // 쿨타임 비율 계산
            float cooldownRatio = remainingCooldown / maxCooldown;

            // 쿨타임 오버레이 크기와 위치 설정
            cooldownOverlay.setSize(sf::Vector2f(
                skillSprite->getGlobalBounds().width,
                skillSprite->getGlobalBounds().height * cooldownRatio
            ));
            cooldownOverlay.setPosition(
                skillSprite->getPosition().x - skillSprite->getGlobalBounds().width / 2,
                skillSprite->getPosition().y - skillSprite->getGlobalBounds().height / 2
            );
            window.draw(cooldownOverlay);
        }
    }
}

void UIManager::updateSkillIconPositions() {
    const int numSkills = 4;
    float skillSpacing = 20.0f; // 스킬 간 간격
    float totalSkillWidth = 0.0f;

    // 스킬 스프라이트들의 크기 계산
    std::vector<sf::Sprite*> skills = { &QSkillSprite, &WSkillSprite, &ESkillSprite, &RSkillSprite };
    for (auto* skill : skills) {
        skill->setScale(1.0f, 1.0f); // 필요시 크기 조정
        skill->setOrigin(skill->getLocalBounds().width / 2, skill->getLocalBounds().height / 2); // 중심 설정
        totalSkillWidth += skill->getGlobalBounds().width;
    }
    totalSkillWidth += skillSpacing * (numSkills - 1);

    // 화면의 뷰 중심 계산
    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();
    float startX = viewCenter.x - totalSkillWidth / 2.0f;
    float yPos = viewCenter.y + viewSize.y / 2.0f - 100.0f; // 화면 하단에서 100 픽셀 위

    // 각 스킬 위치 설정
    for (int i = 0; i < numSkills; ++i) {
        skills[i]->setPosition(
            startX + skills[i]->getGlobalBounds().width / 2 + i * (skills[i]->getGlobalBounds().width + skillSpacing),
            yPos
        );
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
