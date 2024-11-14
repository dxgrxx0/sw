#include "UpgradeUI.h"
#include <SFML/Graphics.hpp>

UpgradeUI::UpgradeUI(sf::Font& font, const sf::Vector2f& windowSize)
    : isVisible(false) {
    // 배경 설정
    background.setSize(sf::Vector2f(windowSize.x * 0.6f, windowSize.y * 0.4f)); // 창 크기 설정 (화면 크기의 60% 폭, 40% 높이)
    background.setFillColor(sf::Color(50, 50, 50, 200)); // 반투명 회색 배경
    background.setPosition((windowSize.x - background.getSize().x) / 2, (windowSize.y - background.getSize().y) / 2); // 화면 중앙에 배치
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);

    // 업그레이드 옵션 텍스트 초기화
    for (int i = 0; i < 3; ++i) {
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setCharacterSize(24);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition(background.getPosition().x + 20, background.getPosition().y + 50 + i * 50); // 옵션 위치
        optionTexts.push_back(optionText);
    }
}

void UpgradeUI::showOptions(const std::vector<std::string>& options) {
    isVisible = true;
    for (int i = 0; i < 3; ++i) {
        if (i >= options.size()) {
            optionTexts[i].setString("");
            continue;
        }
        optionTexts[i].setString(options[i]); // 각 옵션 설명 설정
    }
}

void UpgradeUI::hide() {
    isVisible = false;
}

int UpgradeUI::handleClick(const sf::Vector2f& mousePos) {
    if (!isVisible) return -1; // UI가 표시되지 않으면 클릭 처리 안 함

    for (int i = 0; i < optionTexts.size(); ++i) {
        if (optionTexts[i].getGlobalBounds().contains(mousePos)) {
            return i; // 클릭된 옵션 인덱스 반환
        }
    }
    return -1; // 아무 것도 클릭되지 않으면 -1 반환
}

void UpgradeUI::draw(sf::RenderWindow& window) {
    if (!isVisible) return; // UI가 비활성화 상태라면 그리지 않음

    window.draw(background); // 배경 그리기
    for (const auto& text : optionTexts) {
        window.draw(text); // 각 옵션 텍스트 그리기
    }
}

bool UpgradeUI::getIsVisible() const {
    return isVisible;
}
