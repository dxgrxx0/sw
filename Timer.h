#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

class Timer {
public:
    Timer(float x, float y, int fontSize = 24);

    void reset();

    void update(float x, float y);

    void draw(sf::RenderWindow& window);

private:
    sf::Font font;
    sf::Text timerText;
    sf::Clock clock;
    float positionX, positionY;
    int fontSize;
    float getWidth() const;
};
