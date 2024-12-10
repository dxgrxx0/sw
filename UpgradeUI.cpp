#include "UpgradeUI.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
UpgradeUI::UpgradeUI(sf::Font& font, const sf::Vector2f& windowSize)
    : isVisible(false),font(font) {
    // 배경 설정
    background.setSize(sf::Vector2f(windowSize.x * 0.6f, windowSize.y * 0.4f)); // 창 크기 설정 (화면 크기의 60% 폭, 40% 높이)
    background.setFillColor(sf::Color(50, 50, 50, 200)); // 반투명 회색 배경
    background.setPosition((windowSize.x - background.getSize().x) / 2, (windowSize.y - background.getSize().y) / 2); // 화면 중앙에 배치
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);

    /*// 업그레이드 옵션 텍스트 초기화
    for (int i = 0; i < 3; ++i) {
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setCharacterSize(24);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition(background.getPosition().x + 20, background.getPosition().y + 50 + i * 50); // 옵션 위치
        optionTexts.push_back(optionText);
    }
    optionSprites.clear();
    optionTextures.clear();*/
}

void UpgradeUI::showOptions(const std::vector<std::string>& descriptions, const std::vector<std::string>& imagePaths) {
    isVisible = true;
    optionBoxes.clear();
    optionSprites.clear();
    optionTextures.clear();
    optionTexts.clear();
    float extraSpacing = 110.0f; // X축 간격 추가
    sf::Vector2f boxSize(200.0f, 300.0f); // 박스 크기 설정
    for (size_t i = 0; i < imagePaths.size(); ++i) {
        // 1. 텍스처 로드
        sf::Texture texture;
        if (!texture.loadFromFile(imagePaths[i])) {
            std::cerr << "Failed to load image: " << imagePaths[i] << std::endl;
            continue;
        }
        size_t dotPos = imagePaths[i].find_last_of('.');
        optionTextures.push_back(ResourceManager::getInstance().getTexture(imagePaths[i].substr(0, dotPos)));
        // 2. 스프라이트 생성 및 위치 설정
        sf::Sprite sprite;
        //sprite.setTexture(optionTextures.back());
        
        sprite.setTexture(ResourceManager::getInstance().getTexture(imagePaths[i].substr(0, dotPos)));
        sf::Vector2u textureSize = optionTextures.back().getSize();
        sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 4.0f);
        // 박스의 X, Y 좌표 계산
        float xOffset = background.getPosition().x + background.getSize().x / 2.0f
            - (imagePaths.size() - 1) * (boxSize.x + extraSpacing) / 2.0f
            + i * (boxSize.x + extraSpacing);
        float yOffset = background.getPosition().y + background.getSize().y / 2.0f;
        sprite.setPosition(xOffset, yOffset - boxSize.y / 4.0f);
        optionSprites.push_back(sprite);
        // 3. 텍스트 생성 및 위치 설정
        sf::Text text;
        text.setFont(font);
        text.setString(descriptions[i]);
        text.setCharacterSize(17);
        text.setFillColor(sf::Color::White);
        text.setOrigin(text.getLocalBounds().width / 2.0f, 0);
        text.setPosition(xOffset, yOffset + boxSize.y / 4.0f);
        optionTexts.push_back(text);
        // 4. 네모 박스 생성 및 설정
        sf::RectangleShape box(boxSize);
        box.setFillColor(sf::Color(0, 0, 0, 150)); // 반투명 검은색
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2);
        box.setOrigin(boxSize.x / 2.0f, boxSize.y / 2.0f);
        box.setPosition(xOffset, yOffset);
        optionBoxes.push_back(box);
        // 디버그 출력
        std::cout << "Loaded image: " << imagePaths[i] << ", Text: " << descriptions[i] << ", Box position: ("
            << xOffset << ", " << yOffset << ")\n";
    }
}

void UpgradeUI::hide() {
    isVisible = false;
}

int UpgradeUI::handleClick(const sf::Vector2f& mousePos) {
    if (!isVisible) return -1; // UI가 표시되지 않으면 클릭 처리 안 함

    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i);
        }
    }
    return -1; // 아무 것도 클릭되지 않으면 -1 반환
}

void UpgradeUI::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    window.draw(background);
    // 스프라이트 및 텍스트 그리기
    for (size_t i = 0; i < optionSprites.size(); ++i) {
        window.draw(optionBoxes[i]);  // 네모 박스
        window.draw(optionSprites[i]); // 이미지
        window.draw(optionTexts[i]);   // 텍스트
    }
}

bool UpgradeUI::getIsVisible() const {
    return isVisible;
}