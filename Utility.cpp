// Utility.cpp
#include "Utility.h"

float calculateDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
    return std::sqrt(std::pow(pos2.x - pos1.x, 2) + std::pow(pos2.y - pos1.y, 2));
}
float degreesToRadians(float degrees) {
    const float PI = 3.14159265358979323846f;
    return degrees * (PI / 180.0f);
}
