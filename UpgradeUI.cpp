#include "UpgradeUI.h"
#include <SFML/Graphics.hpp>

UpgradeUI::UpgradeUI(sf::Font& font, const sf::Vector2f& windowSize)
    : isVisible(false) {
    // ��� ����
    background.setSize(sf::Vector2f(windowSize.x * 0.6f, windowSize.y * 0.4f)); // â ũ�� ���� (ȭ�� ũ���� 60% ��, 40% ����)
    background.setFillColor(sf::Color(50, 50, 50, 200)); // ������ ȸ�� ���
    background.setPosition((windowSize.x - background.getSize().x) / 2, (windowSize.y - background.getSize().y) / 2); // ȭ�� �߾ӿ� ��ġ
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);

    // ���׷��̵� �ɼ� �ؽ�Ʈ �ʱ�ȭ
    for (int i = 0; i < 3; ++i) {
        sf::Text optionText;
        optionText.setFont(font);
        optionText.setCharacterSize(24);
        optionText.setFillColor(sf::Color::White);
        optionText.setPosition(background.getPosition().x + 20, background.getPosition().y + 50 + i * 50); // �ɼ� ��ġ
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
        optionTexts[i].setString(options[i]); // �� �ɼ� ���� ����
    }
}

void UpgradeUI::hide() {
    isVisible = false;
}

int UpgradeUI::handleClick(const sf::Vector2f& mousePos) {
    if (!isVisible) return -1; // UI�� ǥ�õ��� ������ Ŭ�� ó�� �� ��

    for (int i = 0; i < optionTexts.size(); ++i) {
        if (optionTexts[i].getGlobalBounds().contains(mousePos)) {
            return i; // Ŭ���� �ɼ� �ε��� ��ȯ
        }
    }
    return -1; // �ƹ� �͵� Ŭ������ ������ -1 ��ȯ
}

void UpgradeUI::draw(sf::RenderWindow& window) {
    if (!isVisible) return; // UI�� ��Ȱ��ȭ ���¶�� �׸��� ����

    window.draw(background); // ��� �׸���
    for (const auto& text : optionTexts) {
        window.draw(text); // �� �ɼ� �ؽ�Ʈ �׸���
    }
}

bool UpgradeUI::getIsVisible() const {
    return isVisible;
}
