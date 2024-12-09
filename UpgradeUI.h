#ifndef UPGRADEUI_H
#define UPGRADEUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class UpgradeUI {
private:
    sf::RectangleShape background;             // 업그레이드 창 배경
    std::vector<sf::RectangleShape> optionBoxes; // 옵션 박스
    std::vector<sf::Sprite> optionSprites;      // 옵션 이미지
    std::vector<sf::Texture> optionTextures;    // 옵션 텍스처
    std::vector<sf::Text> optionTexts;          // 옵션 텍스트
    bool isVisible; // 업그레이드 UI 표시 여부
    sf::Font& font; // 외부에서 전달받은 폰트 참조

public:
    // 생성자
    UpgradeUI(sf::Font& font, const sf::Vector2f& windowSize);

    // 업그레이드 옵션을 화면에 표시
    void showOptions(const std::vector<std::string>& descriptions, const std::vector<std::string>& imagePaths);

    // 업그레이드 UI 숨기기
    void hide();

    // 마우스 클릭 처리하여 선택된 옵션의 인덱스를 반환
    int handleClick(const sf::Vector2f& mousePos);

    // UI 그리기
    void draw(sf::RenderWindow& window);

    // UI가 표시 중인지 확인
    bool getIsVisible() const;
};

#endif // UPGRADEUI_H