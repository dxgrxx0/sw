#include "Timer.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

// ������
Timer::Timer(float x, float y, int fontSize)
    : positionX(x), positionY(y), fontSize(fontSize) {
    // ��Ʈ �ε�
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    // �ؽ�Ʈ ����
    timerText.setFont(font);
    timerText.setCharacterSize(fontSize);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);
    timerText.setPosition(positionX, positionY);

    // Ÿ�̸� �ʱ�ȭ
    reset();
}

// Ÿ�̸� �ʱ�ȭ
void Timer::reset() {
    clock.restart();
}

// Ÿ�̸� ������Ʈ
void Timer::update() {
    sf::Time elapsed = clock.getElapsedTime();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << elapsed.asSeconds();
    timerText.setString("Timer: " + ss.str() + "s");
}

// Ÿ�̸� �׸���
void Timer::draw(sf::RenderWindow& window) {
    window.draw(timerText);
}
