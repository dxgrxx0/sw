#ifndef TRAINING_TOWER_H
#define TRAINING_TOWER_H
#include "SubTower.h"
#include "Knight.h"
#include <vector>
#include <memory>

class TrainingTower : public SubTower {
private:
    std::vector<std::unique_ptr<Knight>> knights;
    int maxKnights;
    float spawnInterval;
    sf::Clock spawnClock;
    sf::Vector2f mainTowerPos;

public:
    TrainingTower(sf::Vector2f position)
        : SubTower(position, 200.0f, 0.2f, 0.0f)
        , maxKnights(3)
        , spawnInterval(5.0f)
        , mainTowerPos(sf::Vector2f(650.0f, 500.0f))
    {
        if (!texture.loadFromFile("TrainingTower.png")) {
            std::cout << "Failed to load training tower texture!" << std::endl;
        }

        rangeIndicator.setPosition(position);
        rangeIndicator.setRadius(range);
        rangeIndicator.setOrigin(range, range);
        rangeIndicator.setFillColor(sf::Color(0, 255, 0, 10));
        rangeIndicator.setOutlineColor(sf::Color(0, 255, 0, 100));
        rangeIndicator.setOutlineThickness(2.0f);
    }

    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed && knights.size() < maxKnights) {
            spawnKnight();
            attackClock.restart();
        }
        updateKnights(deltaTime, monsters);
    }

    void draw(sf::RenderTarget& target) override {
        SubTower::draw(target);
        for (const auto& knight : knights) {
            knight->draw(target);
        }
    }

private:
    void spawnKnight() {
        knights.push_back(std::make_unique<Knight>(position, mainTowerPos));
    }

    void updateKnights(float deltaTime, std::vector<std::unique_ptr<Monster>>& monsters) {
        for (auto it = knights.begin(); it != knights.end();) {
            if ((*it)->isDead()) {
                it = knights.erase(it);
                continue;
            }

            // 가장 가까운 몬스터 찾기
            Monster* closestMonster = nullptr;
            float closestDistance = std::numeric_limits<float>::max();

            for (const auto& monster : monsters) {
                if (!monster->isDead()) {  // 죽지 않은 몬스터만 대상으로
                    float distance = getDistance((*it)->getPosition(), monster->getPosition());
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestMonster = monster.get();
                    }
                }
            }

            // 타워 범위 내에 몬스터가 있으면 공격, 없으면 순찰
            if (closestMonster && isInRange(closestMonster->getPosition())) {
                (*it)->engage(closestMonster, deltaTime);
            }
            else {
                (*it)->patrol(position, deltaTime);  // 메인타워가 아닌 훈련타워 주변을 순찰
            }

            ++it;
        }
    }

    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

#endif // TRAINING_TOWER_H