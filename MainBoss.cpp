#include "MainBoss.h"
#include "ResourceManager.h"
#include "Character.h"
#include "MainTower.h"
#include <iostream>
#include <cmath>
static void updateBoundingBox(DrawingSkillInstance& inst) {
    if (inst.drawingVertices.getVertexCount() == 0) {
        inst.boundingBox = sf::FloatRect();
        return;
    }

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (size_t i = 0; i < inst.drawingVertices.getVertexCount(); ++i) {
        const sf::Vector2f& pos = inst.drawingVertices[i].position;
        if (pos.x < minX) minX = pos.x;
        if (pos.x > maxX) maxX = pos.x;
        if (pos.y < minY) minY = pos.y;
        if (pos.y > maxY) maxY = pos.y;
    }

    inst.boundingBox = sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
}
MainBoss::MainBoss(float x, float y, float speed, MonsterType type) : Monster(x, y, speed, type) {
    switch (type) {
    case MonsterType::Main_Boss:
        textureName = "MainBoss";
        movementSpeed = 10.0f;
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
    penTextures.reserve(penNames.size());
    for (auto& pname : penNames) {
        penTextures.push_back(ResourceManager::getInstance().getTexture(pname));
        sf::Sprite ps;
        ps.setTexture(penTextures.back());
        penSprites.push_back(ps);
    }
}

bool MainBoss::initializeDrawingFromImage(DrawingSkillInstance& instance, const std::string& imagePath, int imageIndex) {
    sf::Image image;
    if (!image.loadFromFile(imagePath)) {
        std::cerr << "Failed to load drawing image: " << imagePath << std::endl;
        return false;
    }

    sf::Vector2u size = image.getSize();
    if (size.x != 200 || size.y != 200) {
        std::cerr << "Image must be 200x200!" << std::endl;
        return false;
    }

    sf::Vector2f offset(sprite.getPosition().x - size.x / 2, sprite.getPosition().y - size.y / 2);

    instance.drawingPath.clear();
    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            if (image.getPixel(x, y).a > 0) {
                instance.drawingPath.emplace_back(x + offset.x, y + offset.y);
            }
        }
    }

    instance.currentPathIndex = 0;
    instance.drawingDuration = 1.0f;
    instance.pointsPerSecond = static_cast<float>(instance.drawingPath.size()) / instance.drawingDuration;
    instance.elapsedDrawingTime = 0.0f;

    // 펜 스프라이트 선택 (랜덤)
    int penIndex = imageIndex;
    instance.penSprite = penSprites[penIndex];
    instance.penSprite.setOrigin(5, 95);
    //instance.penSprite.setScale(0.4f, 0.4f);
    if (!instance.drawingPath.empty()) {
        instance.penSprite.setPosition(instance.drawingPath.front());
    }

    instance.drawingVertices.setPrimitiveType(sf::Quads);
    instance.drawingVertices.clear();

    instance.isDrawing = true;
    instance.isThrowing = false;
    instance.drawingOffset = sprite.getPosition();
    instance.imagePath = imagePath;

    return true;
}

void MainBoss::castSkill() {
    DrawingSkillInstance instance;
    int imageIndex = rand() % imagePaths.size();
    if (imageIndex == 3) {
		imageIndex = rand() % imagePaths.size();
    }
    std::string chosenImage = imagePaths[imageIndex];
    if (!initializeDrawingFromImage(instance, chosenImage, imageIndex)) {
        return;
    }
    skillInstances.push_back(instance);
}
bool MainBoss::getIsDrawing() {
	for (auto& inst : skillInstances) {
		if (inst.isDrawing) {
            return true;
		}
	}
	return false;
}
void MainBoss::update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos,
    float deltaTime, Character& character, MainTower& mainTower) {


    // 기본 공격 타이머 업데이트
    if (!getIsDrawing()) {
        basicAttackTimer += deltaTime;
        if (basicAttackTimer >= basicAttackCooldown) {
            basicAttackTimer = 0.0f;
            performBasicAttack(CharacterPos); // 캐릭터를 목표로 투사체 발사
        }
        if (isDashing) {
            dashTimeCheck += deltaTime;
			sprite.move(dashDirection * 500.0f * deltaTime);
			if (dashTimeCheck >= dashDuration) {
				isDashing = false;
				dashTimeCheck = 0.0f;
				dashCooldown = 0.0f;
			}
        }
        if (!isDashing) {
            dashCooldown += deltaTime;
            if (dashCooldown >= 5.0f) {
			    float distance = calculateDistance(getPosition(), CharacterPos);
                float deltaX = getPosition().x - CharacterPos.x;
                float deltaY = getPosition().y - CharacterPos.y;
                float magnitude = sqrt(deltaX * deltaX + deltaY * deltaY);

                if (magnitude != 0) {
                    dashDirection = sf::Vector2f(deltaX / magnitude, deltaY / magnitude);
                }
                if (distance <= 100) {
                    isDashing = true;
                }
            }
        }
		
    }
    // 투사체 업데이트
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(deltaTime);

        // 캐릭터와의 충돌 처리
        if (it->checkCollision(character.getPosition())) {
            character.takeDamage(attackPower);
            it = projectiles.erase(it); // 충돌한 투사체 제거
            continue;
        }
		if (it->checkCollision(mainTower.getPosition())) {
			mainTower.takeDamage(attackPower);
			it = projectiles.erase(it); // 충돌한 투사체 제거
			continue;
		}

        // 화면 밖으로 나간 투사체 제거
        if (it->isOutofBound()) {
            it = projectiles.erase(it);
            continue;
        }

        ++it;
    }

    Monster::update(CharacterPos, MainTowerPos, deltaTime, character, mainTower);


    skillTimer += deltaTime;
    if (skillTimer >= skillCooldown) {
        skillTimer = 0.0f;
        basicAttackTimer = 0.0f;
        castSkill();
    }

    for (auto& inst : skillInstances) {
        if (inst.isDrawing) {
            inst.elapsedDrawingTime += deltaTime;
            size_t pointsToDraw = static_cast<size_t>(inst.elapsedDrawingTime * inst.pointsPerSecond);

            float penThickness = 1.0f;
            size_t dotPos = inst.imagePath.find_last_of('.');
            
            sf::Image image = ResourceManager::getInstance().getImage(inst.imagePath.substr(0, dotPos));

            while (inst.currentPathIndex < pointsToDraw && inst.currentPathIndex < inst.drawingPath.size() - 1) {
                if (inst.currentPathIndex > 0) {
                    sf::Vector2f prevPoint = inst.drawingPath[inst.currentPathIndex - 1];
                    sf::Vector2f currentPoint = inst.drawingPath[inst.currentPathIndex];
                    
                    sf::Vector2u imageSize = image.getSize();
                    sf::Vector2f offset(sprite.getPosition().x - imageSize.x / 2, sprite.getPosition().y - imageSize.y / 2);

                    // 이전과 현재 점의 이미지 좌표 계산
                    sf::Vector2f prevImageCoord = prevPoint - offset;
                    sf::Vector2f currentImageCoord = currentPoint - offset;

                    // 이미지 범위를 벗어나지 않도록 체크
                    if (prevImageCoord.x >= 0 && prevImageCoord.x < imageSize.x &&
                        prevImageCoord.y >= 0 && prevImageCoord.y < imageSize.y &&
                        currentImageCoord.x >= 0 && currentImageCoord.x < imageSize.x &&
                        currentImageCoord.y >= 0 && currentImageCoord.y < imageSize.y) {

                        // 픽셀 색상 추출
                        sf::Color prevColor = image.getPixel(static_cast<unsigned int>(prevImageCoord.x), static_cast<unsigned int>(prevImageCoord.y));
                        sf::Color currentColor = image.getPixel(static_cast<unsigned int>(currentImageCoord.x), static_cast<unsigned int>(currentImageCoord.y));

                        sf::Vector2f direction = currentPoint - prevPoint;
                        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                        if (length != 0) {
                            direction /= length;
                            sf::Vector2f perpendicular(-direction.y * penThickness / 2, direction.x * penThickness / 2);

                            inst.drawingVertices.append(sf::Vertex(prevPoint - perpendicular, prevColor));
                            inst.drawingVertices.append(sf::Vertex(prevPoint + perpendicular, prevColor));
                            inst.drawingVertices.append(sf::Vertex(currentPoint + perpendicular, currentColor));
                            inst.drawingVertices.append(sf::Vertex(currentPoint - perpendicular, currentColor));
                        }
                    }
                }
                inst.currentPathIndex++;
            }


            if (inst.currentPathIndex < inst.drawingPath.size() && inst.currentPathIndex % 5 == 0) {
                inst.penSprite.setPosition(inst.drawingPath[inst.currentPathIndex]);
            }

            // 그림 그리기 완료 시 바운딩 박스는 일단 여기선 계산 안 함.
            if (inst.currentPathIndex >= inst.drawingPath.size() - 1 && inst.isDrawing) {
                inst.isDrawing = false;
                inst.isThrowing = true;
                inst.throwDirection = CharacterPos - sprite.getPosition();
                float length = std::sqrt(inst.throwDirection.x * inst.throwDirection.x + inst.throwDirection.y * inst.throwDirection.y);
                if (length != 0) {
                    inst.throwDirection /= length;
                }
                inst.drawingOffset = sprite.getPosition();

                float angle = std::atan2(inst.throwDirection.y, inst.throwDirection.x) * 180 / 3.14159265f;
                sf::Transform transform;
                transform.rotate(angle + 90, sprite.getPosition());
                for (size_t i = 0; i < inst.drawingVertices.getVertexCount(); ++i) {
                    inst.drawingVertices[i].position = transform.transformPoint(inst.drawingVertices[i].position);
                }

                // 회전 후 바운딩 박스 갱신
                updateBoundingBox(inst);
            }
        }

        if (inst.isThrowing) {
            if (inst.imagePath == "MainBossPencilSkill.png") {
                inst.isThrowing = false;
                spawnMidBoss = true;
            }
            // 던지는 중에는 오브젝트 이동
            sf::Vector2f offset = inst.throwDirection * inst.throwSpeed * deltaTime;
            for (size_t i = 0; i < inst.drawingVertices.getVertexCount(); ++i) {
                inst.drawingVertices[i].position += offset;
            }
            inst.drawingOffset += offset;

            // 이동 후 바운딩 박스 갱신
            updateBoundingBox(inst);

            // 캐릭터 충돌 체크: 캐릭터의 위치가 boundingBox 내부에 있는지 확인
            // 간단히 캐릭터를 점으로 보고 바운딩 박스 내에 들어오면 충돌로 간주
            if (inst.boundingBox.contains(CharacterPos)) {
                inst.isThrowing = false;
                character.takeDamage(attackPower);
                // 필요하다면 인스턴스 제거 등의 추가 처리
            }
			if (inst.drawingOffset.x < -1000 || inst.drawingOffset.x > 3600 || inst.drawingOffset.y < -1000 || inst.drawingOffset.y > 2000) {
				inst.isThrowing = false;
			}
        }
    }

    // 끝난 인스턴스 제거 로직 등 필요 시 추가 가능
}
void MainBoss::performBasicAttack(const sf::Vector2f& targetPos) {
    sf::Texture& penTexture = penTextures[rand() % penTextures.size()]; // 랜덤 펜 텍스처 선택
    projectiles.emplace_back(penTexture, sprite.getPosition(), targetPos, 500.0f, attackPower);
}

void MainBoss::draw(sf::RenderTarget& target) {
    Monster::draw(target);

    for (auto& inst : skillInstances) {
        if (inst.isDrawing || inst.isThrowing) {
            target.draw(inst.drawingVertices);
            if (inst.isDrawing) {
                target.draw(inst.penSprite);
            }
        }
    }
    // 투사체 그리기
    for (auto& projectile : projectiles) {
        projectile.draw(target);
    }
}