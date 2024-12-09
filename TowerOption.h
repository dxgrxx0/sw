#pragma once
#ifndef TOWER_OPTION_H
#define TOWER_OPTION_H

#include <string>
#include <functional>
#include <memory>
#include "SubTower.h"
#include <SFML/System/Vector2.hpp>

struct TowerOption {
    std::string name;
    std::function<std::unique_ptr<SubTower>(sf::Vector2f)> towerCreator;
    sf::Vector2f position;
};

#endif // TOWER_OPTION_H