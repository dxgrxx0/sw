#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Timer {
public:
    // 생성자
    Timer(float x, float y, int fontSize = 24);

    // 타이머 초기화
    void reset();

    // 타이머 업데이트
    void update();

    // 타이머 그리기
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Text timerText;
    sf::Clock clock;
    float positionX, positionY;
    int fontSize;
};
