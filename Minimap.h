#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Character.h"
#include "Monster.h"
#include "MainTower.h"

class MiniMap {
public:
    MiniMap(float width, float height, float scale);
    void setPosition(float x, float y);
    void update(const sf::View& towerView,MainTower mainTower,
        const std::vector<Monster>& monsters, Character& warrior);
    void draw(sf::RenderWindow& window);

private:
    sf::RenderTexture minimapTexture;
    sf::Sprite minimapSprite;
    sf::RectangleShape minimapBorder;
};

#endif // MINIMAP_H
