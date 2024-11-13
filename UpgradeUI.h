#ifndef UPGRADEUI_H
#define UPGRADEUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class UpgradeUI {
private:
    sf::RectangleShape background;             // ���׷��̵� â ���
    std::vector<sf::Text> optionTexts;         // ���׷��̵� �ɼ� �ؽ�Ʈ��
    bool isVisible;                            // ���׷��̵� UI ǥ�� ����

public:
    // ������
    UpgradeUI(sf::Font& font, const sf::Vector2f& windowSize);

    // ���׷��̵� �ɼ��� ȭ�鿡 ǥ��
    void showOptions(const std::vector<std::string>& options);

    // ���׷��̵� UI �����
    void hide();

    // ���콺 Ŭ�� ó���Ͽ� ���õ� �ɼ��� �ε����� ��ȯ
    int handleClick(const sf::Vector2f& mousePos);

    // UI �׸���
    void draw(sf::RenderWindow& window);

    // UI�� ǥ�� ������ Ȯ��
    bool getIsVisible() const;
};

#endif // UPGRADEUI_H
