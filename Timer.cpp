#include "Timer.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

Timer::Timer(float x, float y, int fontSize)
    : positionX(x), positionY(y), fontSize(fontSize) {
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    timerText.setFont(font);
    timerText.setCharacterSize(fontSize);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);
    timerText.setPosition(positionX, positionY);

    reset();
}

void Timer::reset() {
    clock.restart();
}

float Timer::getWidth() const {
    return timerText.getLocalBounds().width;
}

void Timer::update(float viewCenterX, float viewCenterY) {
    sf::Time elapsed = clock.getElapsedTime();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << elapsed.asSeconds();
    timerText.setString("Timer: " + ss.str() + "s");

    // X ��ǥ�� mainView�� �߾ӿ� ���߰� Y ��ǥ�� mainView�� �߾ӿ� ����
    positionX = viewCenterX - getWidth() / 2; // X ��ǥ
    positionY = viewCenterY - timerText.getLocalBounds().height / 2; // Y ��ǥ (������ �ݸ�ŭ ����)
    timerText.setPosition(positionX, positionY);
}



void Timer::draw(sf::RenderWindow& window) {
    window.draw(timerText);
}
