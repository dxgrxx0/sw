#pragma once
#ifndef SCREENUI_H
#define SCREENUI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ScreenUi {
private:
    sf::Sprite background;            // 배경 이미지
    sf::Texture backgroundTexture;    // 배경 텍스처
    sf::Font font;
    bool isVisible;                   // 화면 표시 여부

    sf::RectangleShape startButton;  //시작 버튼
    sf::RectangleShape exitButton;   //나가기 버튼
    sf::RectangleShape startButtonBorder;
    sf::RectangleShape exitButtonBorder;
    sf::Text startButtonText;
    sf::Text exitButtonText;

    //gameover
    sf::Texture gameOverTexture;
    sf::Sprite gameOverSprite;
    bool isGameOver;
    //gameclear
    bool isVictory;
    sf::Texture victoryTexture;
    sf::Sprite victorySprite;

public:
    ScreenUi(const sf::Vector2f& windowSize);
    void loadResources(const std::string& backgroundPath, const std::string& fontPath);
    bool handleClick(const sf::Vector2f& mousePos);
    void update(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window);
    bool getIsVisible() const { return isVisible; }
    void hide() { isVisible = false; }

    //gameover
    void setGameOver(bool gameOver) { isGameOver = gameOver; }
    bool getGameOver() const {
        return isGameOver;
    }
    //gameclear
    void setVictory(bool victory) {
        isVictory = victory;
    }
};

#endif // SCREENUI_H