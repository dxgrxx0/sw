#ifndef TRAINING_TOWER_H
#define TRAINING_TOWER_H

#include "SubTower.h"
#include "Knight.h"
#include <vector>
#include <memory>

class TrainingTower : public SubTower {
private:
    std::vector<std::unique_ptr<Knight>> knights;
    int maxKnights;            // 최대 기사 수
    float spawnInterval;       // 기사 생성 간격
    sf::Clock spawnClock;      // 기사 생성 타이머
    sf::Vector2f mainTowerPos; // 메인 타워 위치

    // 텍스처를 한 번만 로드하기 위한 static 멤버
    static sf::Texture knightTexture;
    static bool textureLoaded;

    // 애니메이션 프레임 관련 상수
    static const int FRAME_COUNT = 60;  // 이미지에 있는 전체 프레임 수
    static const int FRAMES_PER_ROW = 8;  // 한 줄당 프레임 수
    static const int FRAME_WIDTH = 64;   // 프레임 너비
    static const int FRAME_HEIGHT = 64;  // 프레임 높이

public:
    TrainingTower(sf::Vector2f position)
        : SubTower(position, 200.0f, 0.2f, 0.0f)  // 범위, 공격속도(5초당 1번), 공격력(0: 직접공격X)
        , maxKnights(3)
        , spawnInterval(5.0f)
        , mainTowerPos(sf::Vector2f(650.0f, 500.0f))  // 메인 타워의 위치를 중앙으로 고정
    {
        if (!textureLoaded) {
            if (knightTexture.loadFromFile("knightbg.png")) {
                textureLoaded = true;
            }
        }
        if (!texture.loadFromFile("TrainingTower.png")) {
            std::cout << "Failed to load training tower texture!" << std::endl;
        }

        // 기본 SubTower의 range indicator 설정
        rangeIndicator.setPosition(position);
        rangeIndicator.setRadius(range);
        rangeIndicator.setOrigin(range, range);
        rangeIndicator.setFillColor(sf::Color(0, 255, 0, 10));
        rangeIndicator.setOutlineColor(sf::Color(0, 255, 0, 100));
        rangeIndicator.setOutlineThickness(2.0f);
    }


    void attack(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) override {
        // 스폰 간격 체크 및 최대 기사 수 제한
        if (attackClock.getElapsedTime().asSeconds() >= 1.0f / attackSpeed &&
            knights.size() < maxKnights) {
            spawnKnight();
            attackClock.restart();
        }
        updateKnights(deltaTime, monsters);
    }

    void draw(sf::RenderTarget& target) override {
        // SubTower의 기본 draw 호출 (range indicator와 sprite 그리기)
        SubTower::draw(target);

        // 기사들 그리기
        for (const auto& knight : knights) {
            knight->draw(target);
        }
    }

private:
    void spawnKnight() {
        auto knight = std::make_unique<Knight>(position, mainTowerPos);
        if (textureLoaded) {
            knight->setTexture(knightTexture);
        }
        knights.push_back(std::move(knight));
    }

    void updateKnights(float deltaTime, std::vector<std::unique_ptr<Monster>>& monsters) {
        // 죽은 기사 제거
        knights.erase(
            std::remove_if(knights.begin(), knights.end(),
                [](const auto& knight) { return knight->isDead(); }
            ),
            knights.end()
        );

        for (auto& knight : knights) {
            // 가장 가까운 몬스터 찾기
            Monster* closestMonster = nullptr;
            float closestDistance = std::numeric_limits<float>::max();

            for (const auto& monster : monsters) {
                if (!monster->isDead()) {
                    float distance = getDistance(knight->getPosition(), monster->getPosition());
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestMonster = monster.get();
                    }
                }
            }

            // 타워 범위 내에 몬스터가 있으면 공격, 없으면 순찰
            if (closestMonster && isInRange(closestMonster->getPosition())) {
                knight->engage(closestMonster, deltaTime);
            }
            else {
                knight->patrol(position, deltaTime);
            }
        }
    }

    float getDistance(const sf::Vector2f& pos1, const sf::Vector2f& pos2) {
        float dx = pos1.x - pos2.x;
        float dy = pos1.y - pos2.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
// Static 멤버 변수 초기화
sf::Texture TrainingTower::knightTexture;
bool TrainingTower::textureLoaded = false;
#endif // TRAINING_TOWER_H