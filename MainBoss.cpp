#include "MainBoss.h"
#include "Character.h"
#include "MainTower.h"
#include <iostream>
#include <cmath>

MainBoss::MainBoss(float x, float y, float speed, MonsterType type) : Monster(x, y, speed, type) {
    switch (type) {
    case MonsterType::Main_Boss: // 메인보스
        textureName = "MainBoss";
        movementSpeed = 8.0f;
        healthPoint = 5000.0f;
        attackPower = 100.0f;
        defense = 50.0f;
        break;
    default:
        break;
    }

    texture = ResourceManager::getInstance().getTexture(textureName);
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(x, y);
    sprite.setOrigin(500, 500);

    // 펜 텍스처 및 스프라이트 초기화
    std::vector<std::string> penNames = { "RedPen", "OrangePen", "YellowPen", "GreenPen", "BluePen", "NavyPen", "PurplePen" };
    for (size_t i = 0; i < 7; ++i) {
        penTextures[i] = ResourceManager::getInstance().getTexture(penNames[i]);
        penSprites[i].setTexture(penTextures[i]);
    }
	penSprites[2].setOrigin(10, 190); // YellowPen 중심점 설정
    penSprites[2].setScale(0.4f, 0.4f); // YellowPen 크기 설정

    // 그림 초기화 (이미지를 기반으로 경로 설정)
    if (!initializeDrawingFromImage("zeus.png",100)) {
        std::cerr << "Failed to load drawing image!" << std::endl;
    }
}
bool MainBoss::initializeDrawingFromImage(const std::string& imagePath, float penThickness) {
    sf::Image image;
    if (!image.loadFromFile(imagePath)) {
        return false;
    }

    sf::Vector2u size = image.getSize();
    if (size.x != 200 || size.y != 200) {
        std::cerr << "Image must be 200x200!" << std::endl;
        return false;
    }

    sf::Vector2f offset(sprite.getPosition().x - size.x / 2, sprite.getPosition().y - size.y / 2);

    // 여기서는 경로만 저장
    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            if (image.getPixel(x, y).a) { // 투명하지 않은 픽셀만
                drawingPath.emplace_back(x + offset.x, y + offset.y);
            }
        }
    }

    currentPathIndex = 0;
    drawingDuration = 3.0f;
    pointsPerSecond = static_cast<float>(drawingPath.size()) / drawingDuration;
    elapsedDrawingTime = 0.0f;

    // 펜 초기 위치
    if (!drawingPath.empty()) {
        penSprites[2].setPosition(drawingPath.front());
    }
	isDrawing = true;
    return true;
}
void MainBoss::update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos,
    float deltaTime, Character& character, MainTower& mainTower) {
    Monster::update(CharacterPos, MainTowerPos, deltaTime, character, mainTower);

    elapsedDrawingTime += deltaTime;
    size_t pointsToDraw = static_cast<size_t>(elapsedDrawingTime * pointsPerSecond);

    float penThickness = 1.0f; // 펜 두께

    // 아직 그리지 않은 부분을 그린다.
    while (currentPathIndex < pointsToDraw && currentPathIndex < drawingPath.size() - 1) {
        if (currentPathIndex > 0) {
            sf::Vector2f prevPoint = drawingPath[currentPathIndex - 1];
            sf::Vector2f currentPoint = drawingPath[currentPathIndex];

            sf::Vector2f direction = currentPoint - prevPoint;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length != 0) {
                direction /= length;
                sf::Vector2f perpendicular(-direction.y * penThickness / 2, direction.x * penThickness / 2);

                // 두께 20의 사각형을 만들어 line segment를 형성
                drawingVertices.append(sf::Vertex(prevPoint - perpendicular, sf::Color::Yellow));
                drawingVertices.append(sf::Vertex(prevPoint + perpendicular, sf::Color::Yellow));
                drawingVertices.append(sf::Vertex(currentPoint + perpendicular, sf::Color::Yellow));
                drawingVertices.append(sf::Vertex(currentPoint - perpendicular, sf::Color::Yellow));
            }
        }
        currentPathIndex++;
    }        // 펜 위치 갱신
    if (currentPathIndex % 5 == 0)penSprites[2].setPosition(drawingPath[currentPathIndex]);
    if (currentPathIndex >= drawingPath.size() - 1&&isDrawing) {
        isDrawing = false;
        isThrowing = true;
        // 던질 방향 계산
        throwDirection = CharacterPos - sprite.getPosition();
        float length = std::sqrt(throwDirection.x * throwDirection.x + throwDirection.y * throwDirection.y);
        if (length != 0) {
            throwDirection /= length; // 단위 벡터로 변환
        }
		drawingOffset = sprite.getPosition();
        
        float angle = std::atan2(throwDirection.y, throwDirection.x) * 180 / 3.14159265f;
		sf::Transform transform;
		transform.rotate(angle+90, sf::Vector2f(1000,1000));
        for (size_t i = 0; i < drawingVertices.getVertexCount(); ++i) {
			drawingVertices[i].position = transform.transformPoint(drawingVertices[i].position);
        }
    }
	
    if (isThrowing) {

        // 그림을 플레이어 쪽으로 던지기
        sf::Vector2f offset = throwDirection * throwSpeed * deltaTime;

        // 모든 버텍스 위치 이동
        for (size_t i = 0; i < drawingVertices.getVertexCount(); ++i) {
            drawingVertices[i].position += offset;
        }
        drawingOffset += offset;

        // 플레이어와의 충돌 확인
        float distanceToPlayer = std::sqrt(std::pow(drawingOffset.x - CharacterPos.x, 2) +
            std::pow(drawingOffset.y - CharacterPos.y, 2));
        if (distanceToPlayer < 100.0f) { // 플레이어와 충돌
            isThrowing = false;
            // TODO: 충돌 처리 로직 (예: 플레이어에게 데미지 주기)
			character.takeDamage(attackPower);
        }
    }

}



void MainBoss::draw(sf::RenderTarget& target) {
    Monster::draw(target);

    // 그려진 선
    if (isDrawing||isThrowing) {
        target.draw(drawingVertices);

        // 펜 스프라이트
        if (isDrawing) {
            target.draw(penSprites[2]);
        }
    }
}
