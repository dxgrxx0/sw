#include "UpgradeUI.h"
#include <SFML/Graphics.hpp>
#include <iostream>


UpgradeUI::UpgradeUI(sf::Font& font, const sf::Vector2f& windowSize)
    : font(font), isVisible(false) {
    // 배경 설정
    background.setSize(sf::Vector2f(windowSize.x * 0.6f, windowSize.y * 0.4f)); // 창 크기 설정 (화면 크기의 60% 폭, 40% 높이)
    background.setFillColor(sf::Color(50, 50, 50, 200)); // 반투명 회색 배경
    background.setPosition((windowSize.x - background.getSize().x) / 2, (windowSize.y - background.getSize().y) / 2); // 화면 중앙에 배치
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);


    optionSprites.clear();
    optionTextures.clear();

    //// 업그레이드 옵션 텍스트 초기화
    //for (int i = 0; i < 3; ++i) {
    //    sf::Text optionText;
    //    optionText.setFont(font);
    //    optionText.setCharacterSize(24);
    //    optionText.setFillColor(sf::Color::White);
    //    optionText.setPosition(background.getPosition().x + 20, background.getPosition().y + 50 + i * 50); // 옵션 위치
    //    optionTextures.push_back(optionText);
    //}
}

void UpgradeUI::showOptions(const std::vector<std::string>& descriptions, const std::vector<std::string>& imagePaths ) {
    isVisible = true;
    optionBoxes.clear();
    optionSprites.clear();
    optionTextures.clear();
    optionTexts.clear();

    float extraSpacing = 110.0f;
    sf::Vector2f boxSize(200.0f, 300.0f); 

    for (size_t i = 0; i < imagePaths.size(); ++i) {

        sf::Texture texture;
        if (!texture.loadFromFile(imagePaths[i])) {
            std::cerr << "Failed to load image: " << imagePaths[i] << std::endl;
            continue;
        }
        optionTextures.push_back(std::move(texture));


        sf::Sprite sprite(optionTextures.back());
        sf::Vector2u textureSize = optionTextures.back().getSize();
        sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 4.0f);


        float xOffset = background.getPosition().x + background.getSize().x / 2.0f
            - (imagePaths.size() - 1) * (boxSize.x + extraSpacing) / 2.0f
            + i * (boxSize.x + extraSpacing);
        float yOffset = background.getPosition().y + background.getSize().y / 2.0f;


        sprite.setPosition(xOffset, yOffset - boxSize.y / 4.0f);

        optionSprites.push_back(sprite);

        sf::Text text;
        text.setFont(font);
        text.setString(descriptions[i]);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::White);
        text.setOrigin(text.getLocalBounds().width / 2.0f, 0);
        text.setPosition(xOffset, yOffset + boxSize.y / 4.0f);

        optionTexts.push_back(text);

        sf::RectangleShape box(boxSize);
        box.setFillColor(sf::Color(0, 0, 0, 150)); 
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
    if (!isVisible) return -1;

    for (size_t i = 0; i < optionBoxes.size(); ++i) {
        if (optionBoxes[i].getGlobalBounds().contains(mousePos)) {
            return static_cast<int>(i);
        }
    }

    return -1; 

}



bool UpgradeUI::getIsVisible() const {
    return isVisible;

}

void UpgradeUI::draw(sf::RenderWindow& window) {
    if (!isVisible) return;


    window.draw(background);

    for (size_t i = 0; i < optionSprites.size(); ++i) {
        window.draw(optionBoxes[i]);  // 네모 박스
        window.draw(optionSprites[i]); // 이미지
        window.draw(optionTexts[i]);   // 텍스트
    }
}
