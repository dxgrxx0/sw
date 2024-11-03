#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Timer {
public:
    // ������
    Timer(float x, float y, int fontSize = 24);

    // Ÿ�̸� �ʱ�ȭ
    void reset();

    // Ÿ�̸� ������Ʈ
    void update();

    // Ÿ�̸� �׸���
    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Text timerText;
    sf::Clock clock;
    float positionX, positionY;
    int fontSize;
};
