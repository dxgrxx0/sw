#include "Timer.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

// 생성자
Timer::Timer(float x, float y, int fontSize)
    : positionX(x), positionY(y), fontSize(fontSize) {
    // 폰트 로드
    if (!font.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed to load font");
    }

    // 텍스트 설정
    timerText.setFont(font);
    timerText.setCharacterSize(fontSize);
    timerText.setFillColor(sf::Color::White);
    timerText.setStyle(sf::Text::Bold);
    timerText.setPosition(positionX, positionY);

    // 타이머 초기화
    reset();
}

// 타이머 초기화
void Timer::reset() {
    clock.restart();
}

// 타이머 업데이트
void Timer::update() {
    sf::Time elapsed = clock.getElapsedTime();

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << elapsed.asSeconds();
    timerText.setString("Timer: " + ss.str() + "s");
}

// 타이머 그리기
void Timer::draw(sf::RenderWindow& window) {
    window.draw(timerText);
}
