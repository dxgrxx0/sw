
#include <SFML/Graphics.hpp>
#include"ScreenUi.h"

ScreenUi::ScreenUi(const sf::Vector2f& windowSize) : isVisible(true), isGameOver(false), isVictory(false) {
    // 시작 버튼 설정
    startButton.setSize(sf::Vector2f(300.f, 60.f));
    startButton.setFillColor(sf::Color(129, 193, 71)); // 보라색
    startButton.setPosition(
        windowSize.x / 2 - startButton.getSize().x / 2,
        windowSize.y * 0.7f
    );
    // START GAME 버튼 테두리
    startButtonBorder.setSize(sf::Vector2f(308.f, 68.f));
    startButtonBorder.setFillColor(sf::Color(129, 199, 132)); // 밝은 초록색
    startButtonBorder.setPosition(
        startButton.getPosition().x - 4,
        startButton.getPosition().y - 4
    );

    // EXIT 버튼 설정
    exitButton.setSize(sf::Vector2f(300.f, 60.f));
    exitButton.setFillColor(sf::Color(76, 175, 80));
    exitButton.setPosition(
        windowSize.x / 2 - exitButton.getSize().x / 2,
        windowSize.y * 0.8f
    );

    // EXIT 버튼 테두리
    exitButtonBorder.setSize(sf::Vector2f(308.f, 68.f));
    exitButtonBorder.setFillColor(sf::Color(129, 199, 132));
    exitButtonBorder.setPosition(
        exitButton.getPosition().x - 4,
        exitButton.getPosition().y - 4
    );
}

void ScreenUi::loadResources(const std::string& backgroundPath, const std::string& fontPath) {
    // 배경 이미지 로드
    if (backgroundTexture.loadFromFile(backgroundPath)) {
        background.setTexture(backgroundTexture);
        
        // 윈도우 크기에 맞게 배경 크기 조정
        sf::Vector2u textureSize = backgroundTexture.getSize();
        float scaleX = 1600.0f / textureSize.x;  // 창의 너비에 맞춘 비율
        float scaleY = 1000.0f / textureSize.y;  // 창의 높이에 맞춘 비율

        // 비율 유지: 가로와 세로 비율 중 작은 값 선택
        float scale = std::min(scaleX, scaleY);

        // 비율을 유지하며 스케일 적용
        background.setScale(scale, scale);

        //배경을 윈도우 중앙에 배치
        float backgroundWidth = textureSize.x * scale;
        float backgroundHeight = textureSize.y * scale;
        sf::Vector2f position(
            (1600.0f - backgroundWidth) / 2.0f,  // 창 가로 중앙에 배치
            (1000.0f - backgroundHeight) / 2.0f  // 창 세로 중앙에 배치
        );
        background.setPosition(position);
    }

    // 폰트 로드
    if (font.loadFromFile(fontPath)) {
        // START GAME 텍스트 설정
        startButtonText.setFont(font);
        startButtonText.setString("START GAME");
        startButtonText.setCharacterSize(30);
        startButtonText.setFillColor(sf::Color::White);
        startButtonText.setStyle(sf::Text::Bold);

        sf::FloatRect startTextBounds = startButtonText.getLocalBounds();
        startButtonText.setPosition(
            startButton.getPosition().x + (startButton.getSize().x - startTextBounds.width) / 2,
            startButton.getPosition().y + (startButton.getSize().y - startTextBounds.height) / 2-6
        );

        // EXIT 텍스트 설정
        exitButtonText.setFont(font);
        exitButtonText.setString("EXIT");
        exitButtonText.setCharacterSize(30);
        exitButtonText.setFillColor(sf::Color::White);
        exitButtonText.setStyle(sf::Text::Bold);

        sf::FloatRect exitTextBounds = exitButtonText.getLocalBounds();
        exitButtonText.setPosition(
            exitButton.getPosition().x + (exitButton.getSize().x - exitTextBounds.width) / 2,
            exitButton.getPosition().y + (exitButton.getSize().y - exitTextBounds.height) / 2-6
        );
    }


    // Game Over 이미지 로드
    if (gameOverTexture.loadFromFile("gameover.png")) {
        gameOverSprite.setTexture(gameOverTexture);

        // 윈도우 크기에 맞게 Game Over 이미지 크기 조정
        sf::Vector2u textureSize = gameOverTexture.getSize();
        float scaleX = 1600.0f / textureSize.x;
        float scaleY = 1000.0f / textureSize.y;
        float scale = std::min(scaleX, scaleY);

        gameOverSprite.setScale(scale, scale);

        // Game Over 이미지를 윈도우 중앙에 배치
        float spriteWidth = textureSize.x * scale;
        float spriteHeight = textureSize.y * scale;
        sf::Vector2f position(
            (1600.0f - spriteWidth) / 2.0f,
            (1000.0f - spriteHeight) / 2.0f
        );
        gameOverSprite.setPosition(position);
    }
    // 승리 화면 텍스처 로딩
    if (victoryTexture.loadFromFile("gameclear.png")) {
        victorySprite.setTexture(victoryTexture);

        // 승리 화면 크기 조정
        sf::Vector2u victorySize = victoryTexture.getSize();
        float scaleX = 1600.0f / victorySize.x;
        float scaleY = 1000.0f / victorySize.y;
        float scale = std::min(scaleX, scaleY);

        victorySprite.setScale(scale, scale);

        // 승리 화면 중앙 정렬
        float spriteWidth = victorySize.x * scale;
        float spriteHeight = victorySize.y * scale;
        victorySprite.setPosition(
            (1600.0f - spriteWidth) / 2.0f,
            (1000.0f - spriteHeight) / 2.0f
        );
    }
}

bool ScreenUi::handleClick(const sf::Vector2f& mousePos) {
    if (!isVisible) return false;

    // 시작 버튼 클릭 확인
    if (startButton.getGlobalBounds().contains(mousePos)) {
        return true;  // 게임 시작
    }
    if (exitButton.getGlobalBounds().contains(mousePos)) {
        std::exit(0);  // 게임 종료
    }
    return false;
}

void ScreenUi::update(const sf::Vector2f& mousePos) {
    // START GAME 버튼 호버 효과
    if (startButton.getGlobalBounds().contains(mousePos)) {
        startButton.setFillColor(sf::Color(129, 199, 132));
        startButtonBorder.setFillColor(sf::Color(76, 175, 80));
    }
    else {
        startButton.setFillColor(sf::Color(76, 175, 80));
        startButtonBorder.setFillColor(sf::Color(129, 199, 132));
    }

    // EXIT 버튼 호버 효과
    if (exitButton.getGlobalBounds().contains(mousePos)) {
        exitButton.setFillColor(sf::Color(129, 199, 132));
        exitButtonBorder.setFillColor(sf::Color(76, 175, 80));
    }
    else {
        exitButton.setFillColor(sf::Color(76, 175, 80));
        exitButtonBorder.setFillColor(sf::Color(129, 199, 132));
    }
}


void ScreenUi::draw(sf::RenderWindow& window) {
    if (!isVisible) return;

    if (isGameOver) {
        window.clear();
        window.draw(gameOverSprite);
    }
    else if (isVictory) {
        window.clear();
        window.draw(victorySprite);
    }
    else {
        window.draw(background);
        // 테두리 먼저 그리기
        window.draw(startButtonBorder);
        window.draw(exitButtonBorder);

        // 버튼 그리기
        window.draw(startButton);
        window.draw(exitButton);

        // 텍스트 그리기
        window.draw(startButtonText);
        window.draw(exitButtonText);
    }
}