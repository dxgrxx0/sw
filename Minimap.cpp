#include "Minimap.h"


MiniMap::MiniMap(float width, float height, float scale) {
    minimapTexture.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height));
    minimapSprite.setScale(scale, scale);
    minimapBorder.setSize(sf::Vector2f(width * scale, height * scale));
    minimapBorder.setOutlineColor(sf::Color::Red);
    minimapBorder.setOutlineThickness(3);
    minimapBorder.setFillColor(sf::Color::Transparent);
}

void MiniMap::setPosition(float x, float y) {
    minimapSprite.setPosition(x, y);
    minimapBorder.setPosition(x, y);
}


void MiniMap::update(const sf::View& towerView, MainTower mainTower,
    const std::vector<std::unique_ptr<Monster>>& monsters, Character& warrior) {
    minimapTexture.clear();
    minimapTexture.setView(towerView);
    minimapTexture.draw(mainTower.getSprite()); // MainTower의 스프라이트 그리기

    for (auto& monster : monsters) {
        if (monster->isNear(sf::Vector2f(650, 500), 500)) {
            monster->draw(minimapTexture);
        }
    }
    warrior.draw(minimapTexture);
    minimapTexture.display();
    minimapSprite.setTexture(minimapTexture.getTexture());
}

void MiniMap::draw(sf::RenderWindow& window) {
    window.draw(minimapSprite);
    window.draw(minimapBorder);
}
