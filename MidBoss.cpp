#include "MidBoss.h"
#include "Character.h"
#include "MainTower.h"
#include <iostream>

MidBoss::MidBoss(float x, float y, float speed, MonsterType type) : Monster(x, y, speed, type), skillDuration(5.0f), isSkillActive(false), isCloneActive(false)
, cloneDistance(50.0f), cloneRotationAngle(0.0f), cloneRotationSpeed(180.0f),hitByExplosion(false) {
    switch (type) {
    case MonsterType::Mid_Boss: // 미드보스
        textureName = "MidBoss";
        movementSpeed = 50.0f;
		maxHealth = 10000.0f;
        healthPoint = maxHealth;
        attackPower = 50.0f;
        defense = 20.0f;
        skillDuration = 5.0f;
        bossExplodeTexture = ResourceManager::getInstance().getTexture("BossExplode");
        bossExplodeSprite.setTexture(bossExplodeTexture);
        bossExplodeCircle.setRadius(200);
        bossExplodeCircle.setTexture(&bossExplodeTexture);
        initializeCircularPath(200);
        //shape.setFillColor(sf::Color::Yellow);
        break;
    default:
        break;

    }
    originalSpeed = speed;
    originalDefense = defense;
    originalAttackPower = attackPower;
    originalattackRange = attackRange;

    healthBarBack.setSize(sf::Vector2f(96, 20));
    healthBarBack.setFillColor(sf::Color::Black);
    healthBarBack.setOutlineThickness(2);
    healthBarBack.setOutlineColor(sf::Color::White);
    healthBar.setFillColor(sf::Color::Red);
    texture = ResourceManager::getInstance().getTexture(textureName);
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(x, y);
    sprite.setOrigin(500, 500);
}
void MidBoss::update(const sf::Vector2f& CharacterPos, const sf::Vector2f& MainTowerPos, float deltaTime, Character& character, MainTower& mainTower) {
	Monster::update(CharacterPos, MainTowerPos, deltaTime, character, mainTower);
    float healthPercentage = healthPoint / maxHealth;
    healthBarBack.setPosition(sprite.getPosition().x - 48, sprite.getPosition().y - 120);
    healthBar.setSize(sf::Vector2f(healthPercentage * 92, 16));
    healthBar.setPosition(healthBarBack.getPosition().x + 2, healthBarBack.getPosition().y + 2);
    //투사체업데이트
    updateProjectiles(deltaTime, character, mainTower);
    // 메인 스킬 (15초 쿨타임)
        /*if (skillCooldown.getElapsedTime().asSeconds() >= 15.0f) {
            Fir_useSkill(character, mainTower);
        }
        */
        // 원거리 공격 스킬 (5초 쿨타임)

    if (rangedAttackCooldown.getElapsedTime().asSeconds() >= 5.0f) {
        Sec_useSkill(character, mainTower);
        rangedAttackCooldown.restart();
    }

    // 스킬 지속시간 체크
    if (isSkillActive && skillCooldown.getElapsedTime().asSeconds() >= skillDuration) {
        removeSkillEffects();
    }

    //분신 업데이트
    if (isCloneActive) {
        updateClones(deltaTime);
    }

    if (isSkillActive && skillCooldown.getElapsedTime().asSeconds() >= skillDuration) {
        removeSkillEffects();
        removeClones();
    }
    // 세 번째 스킬 업데이트
    if (thirdSkillTimer.getElapsedTime().asSeconds() >= 10.0f) {
        Third_useSkill(character);
    }
    if (isThirdSkillActive) {
        updateThirdSkill(deltaTime, character);
    }
    if (circularAttacking) {
        updateCircularAttack(deltaTime, playerPosition);
    }
    if (hitByExplosion) {
		character.takeDamage(attackPower * 2.0f);
		hitByExplosion = false;
    }
}
void MidBoss::draw(sf::RenderTarget& target) {
    target.draw(healthBarBack);
    target.draw(healthBar);
    if (circularAttacking) {
        drawTrails(target); // 흔적 그리기

        if (drawingBossExplode) {
            target.draw(bossExplodeCircle);

        }
    }
    if (isCloneActive) {
        for (const auto& cloneSprite : cloneSprites) {
            target.draw(cloneSprite);
        }
    }
    drawProjectiles(target); //투사체 그리기
	Monster::draw(target);
}


//원거리 공격 로직 추가

void MidBoss::shootProjectile(const sf::Vector2f& targetPos) {
    sf::Vector2f direction = targetPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    projectiles.emplace_back(sprite.getPosition(), direction * projectileSpeed, attackPower);
}

void MidBoss::updateProjectiles(float deltaTime, Character& character, MainTower& mainTower) {
    for (auto& projectile : projectiles) {
        if (!projectile.active) continue;

        // 투사체 이동
        projectile.shape.move(projectile.velocity * deltaTime);

        float collisionRadius = 40.0f; // 투사체 충돌 범위


        // 캐릭터와 충돌 체크
        sf::Vector2f projectilePos = projectile.shape.getPosition();
        if (calculateDistance(projectilePos, character.getPosition()) < collisionRadius) {
            character.takeDamage(projectile.damage);
            projectile.active = false;
            continue;
        }

        // 타워와 충돌 체크
        if (calculateDistance(projectilePos, mainTower.getPosition()) < collisionRadius) {
            mainTower.takeDamage(projectile.damage);
            projectile.active = false;
           // printf("Tower hit by projectile!\n"); // 디버깅용 메시지 추가
            continue;
        }

        // 화면 밖으로 나간 투사체 제거
        if (projectilePos.x < -1000 || projectilePos.x > 2100 ||
            projectilePos.y < -1000 || projectilePos.y > 2100) {
            projectile.active = false;
        }
    }

    // 비활성화된 투사체 제거
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.active; }),
        projectiles.end());
}

void MidBoss::drawProjectiles(sf::RenderTarget& target) const {
    for (const auto& projectile : projectiles) {
        if (projectile.active) {
            target.draw(projectile.shape);
        }
    }
}

void MidBoss::Fir_useSkill(Character& character, MainTower& mainTower) {
    if (textureName == "MidBoss") {
        // 미드보스 스킬: "광폭화" - 이동속도와 공격력 증가
        movementSpeed = originalSpeed * 2.5f;
        attackPower = originalAttackPower * 1.5f;
        attackRange = originalattackRange * 2.0f;

        createClones();

        // 시각적 효과
        sprite.setColor(sf::Color::Black);
        //printf("Mid Boss uses Berserk!\n");
    }
    else if (textureName == "MainBoss") {
        // 메인보스 스킬: "절대방어" - 방어력 대폭 증가,주변 광역 공격
        defense = originalDefense * 3.0f;
        movementSpeed = originalSpeed * 4.5f;
        attackRange = originalattackRange * 2.0f;

        createClones();
        //광역 공격
        float attackRadius = 200.0f;
        if (calculateDistance(sprite.getPosition(), character.getPosition()) <= attackRadius) {
            character.takeDamage(attackPower * 1.5f);
        }
        if (calculateDistance(sprite.getPosition(), mainTower.getPosition()) <= attackRadius) {
            mainTower.takeDamage(attackPower * 1.5f);
        }

        // 시각적 효과
        sprite.setColor(sf::Color::Blue);
        //printf("Main Boss uses Absolute Defense!\n");
    }

    isSkillActive = true;
    skillCooldown.restart();
}

void MidBoss::Sec_useSkill(Character& character, MainTower& mainTower) {
    if (textureName == "MidBoss") {
        // 미드보스의 원거리 공격
        for (int i = 0; i < 8; ++i) {
            float angle = i * 45.0f * 3.14159f / 180.0f;
            sf::Vector2f direction(std::cos(angle), std::sin(angle));
            sf::Vector2f targetPos = sprite.getPosition() + direction * 100.f;
            shootProjectile(targetPos);
        }
        //printf("Mid Boss uses Ranged Attack!\n");
    }
    else if (textureName == "MainBoss") {
        // 메인보스의 원거리 공격
        sf::Vector2f targetPos;
        float distanceToHeroine = calculateDistance(sprite.getPosition(), character.getPosition());
        float distanceToTower = calculateDistance(sprite.getPosition(), mainTower.getPosition());

        if (distanceToHeroine < distanceToTower) {
            targetPos = character.getPosition();
        }
        else {
            targetPos = mainTower.getPosition();
        }
        sf::Vector2f baseDirection = targetPos - sprite.getPosition();
        float baseLength = std::sqrt(baseDirection.x * baseDirection.x + baseDirection.y * baseDirection.y);
        if (baseLength != 0) {
            baseDirection /= baseLength;

            // 부채꼴 형태로 투사체 발사 (각도 범위를 좁힘)
            for (int i = 0; i < 8; ++i) {
                float spreadAngle = (i - 3.5f) * 10.0f; // -35도에서 +35도 사이로 조정
                float randomOffset = (rand() % 10 - 5) * 0.5f; // 약간의 랜덤성 추가                 
                float finalAngle = (spreadAngle + randomOffset) * 3.14159f / 180.0f;

                // 회전 행렬 적용
                float newX = baseDirection.x * std::cos(finalAngle) - baseDirection.y * std::sin(finalAngle);
                float newY = baseDirection.x * std::sin(finalAngle) + baseDirection.y * std::cos(finalAngle);
                sf::Vector2f direction(newX, newY);

                // 투사체 생성 및 설정
                Projectile projectile(sprite.getPosition(), direction * projectileSpeed, attackPower * 2.0f);
                projectile.shape.setFillColor(sf::Color::Red);
                projectile.shape.setRadius(15.f);
                projectiles.push_back(projectile);
            }
        }
        //printf("Main Boss uses Ranged Attack!\n");
    }
}
void MidBoss::Third_useSkill(Character& character) {
    if (!isThirdSkillActive) {
        isThirdSkillActive = true;      // 스킬 활성화
        thirdSkillTimer.restart();      // 스킬 타이머 초기화
        alpha = 255;                    // 투명도 초기화
        oscillationTime = 0.0f;         // 진동 시간 초기화
    }
}

void MidBoss::updateThirdSkill(float deltaTime, Character& character) {
    if (isThirdSkillActive) {
        // 진동 효과
        oscillationTime += deltaTime;
        float oscillationOffset = std::sin(oscillationTime * 15.0f) * 100.0f; // 진동 속도와 범위
        sprite.move(oscillationOffset * deltaTime, 0); // 좌우 이동

        // 투명도 감소
        alpha -= 50 * deltaTime; // 투명도 감소 속도
        if (alpha < 0) alpha = 0;
        sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha))); // 투명도 적용

        // 순간이동 처리
        if (alpha <= 0) {
            // 플레이어 주변 50 거리 내로 순간이동
            float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f; // 랜덤 각도
            float teleportDistance = 50.0f;
            sf::Vector2f teleportOffset(std::cos(angle) * teleportDistance, std::sin(angle) * teleportDistance);
            sf::Vector2f teleportPosition = character.getPosition() + teleportOffset;

            sprite.setPosition(teleportPosition); // 순간이동
            //printf("Boss teleports to position: (%f, %f)\n", teleportPosition.x, teleportPosition.y);
            circularAttacking = false;
            CircularAttack(character);

            // 스킬 종료
            removeSkillEffects(); // 기존 스킬 효과 제거
            isThirdSkillActive = false; // 스킬 비활성화
        }
    }
}
void MidBoss::CircularAttack(Character& character) {
    if (!circularAttacking) {
        drawingBossExplode = false;
        circularAttacking = true;
        // 원형 이동 초기화
        circularAngle = 0.0f;
        circularTimer.restart();        // 원형 이동 지속 시간 타이머 초기화
        playerPosition = character.getPosition();
        bossExplodeSprite.setPosition(playerPosition.x - 200, playerPosition.y - 200);
        bossExplodeCircle.setPosition(playerPosition.x - 200, playerPosition.y - 200);
        bossExplodeFrameClock = 0.0f;
        bossExplodeCurrentFrame = 0;

    }
}
void MidBoss::updateCircularAttack(float deltaTime, sf::Vector2f playerPosition) {
    circularAngle += circularSpeed * deltaTime; // 각도 업데이트
    float radians, xOffset, yOffset;
    if (circularAngle >= 360.0f) {
        circularAngle -= 360.0f; // 각도 초기화
        // 플레이어를 중심으로 원형 궤적 계산
        radians = circularAngle * 3.14159f / 180.0f; // 각도를 라디안으로 변환
        xOffset = circularRadius * std::cos(radians);
        yOffset = circularRadius * std::sin(radians);
        circularAngle += 360.0f;
    }
    else {
        // 플레이어를 중심으로 원형 궤적 계산
        radians = circularAngle * 3.14159f / 180.0f; // 각도를 라디안으로 변환
        xOffset = circularRadius * std::cos(radians);
        yOffset = circularRadius * std::sin(radians);
    }


    sprite.setPosition(playerPosition.x + xOffset, playerPosition.y + yOffset); // 원형 위치 갱신

    if (circularTimer.getElapsedTime().asSeconds() >= 0.5f) {
        drawingBossExplode = true;
        bossExplodeFrameClock += deltaTime;
    }
    if (bossExplodeFrameClock >= 1.0f / 16.0f) {
		if (bossExplodeCurrentFrame == 6 && bossExplodeCircle.getGlobalBounds().contains(playerPosition)) {
			hitByExplosion = true;
		}
        bossExplodeCurrentFrame++;
        int frameX = (bossExplodeCurrentFrame % 4) * 400;
        int frameY = (bossExplodeCurrentFrame / 4) * 400;
        bossExplodeCircle.setTextureRect(sf::IntRect(frameX, frameY, 400, 400));
        bossExplodeFrameClock = 0;
    }

    // 스킬 종료
    if (circularTimer.getElapsedTime().asSeconds() >= circularDuration) {
        circularAttacking = false;
    }
}
void MidBoss::drawTrails(sf::RenderTarget& target) {
    int vertexCount;
    circularAngle >= 360.0f ? vertexCount = 360 * 4 : vertexCount = (int)circularAngle * 4;
    // Transform 객체 생성
    sf::Transform transform;
    // 원하는 중심 위치로 이동
    transform.translate(playerPosition.x - 200, playerPosition.y - 200);

    target.draw(&trailVertices[0], vertexCount, sf::Quads, transform); // 한 번의 draw 호출로 모든 흔적 그리기
}
void MidBoss::initializeCircularPath(int radius) {
    int thickness = 20;
    trailVertices.setPrimitiveType(sf::Quads);
    trailVertices.resize(361 * 4);  // 각 세그먼트마다 4개의 꼭짓점 필요
    for (int i = 0; i <= 360; ++i) {
        float angle = i * 3.14159f / 180.0f;
        float nextAngle = (i + 1) * 3.14159f / 180.0f;
        sf::Vector2f outer1(radius * cos(angle), radius * sin(angle));
        sf::Vector2f outer2(radius * cos(nextAngle), radius * sin(nextAngle));
        sf::Vector2f inner1((radius - thickness) * cos(angle), (radius - thickness) * sin(angle));
        sf::Vector2f inner2((radius - thickness) * cos(nextAngle), (radius - thickness) * sin(nextAngle));
        trailVertices[4 * i + 0].position = sf::Vector2f(radius, radius) + outer1;
        trailVertices[4 * i + 1].position = sf::Vector2f(radius, radius) + outer2;
        trailVertices[4 * i + 2].position = sf::Vector2f(radius, radius) + inner2;
        trailVertices[4 * i + 3].position = sf::Vector2f(radius, radius) + inner1;
        for (int j = 0; j < 4; j++) {
            trailVertices[4 * i + j].color = sf::Color::Black;
        }
    }
}
void MidBoss::removeSkillEffects() {
    // 스킬 효과 제거
    movementSpeed = originalSpeed;
    attackPower = originalAttackPower;
    defense = originalDefense;
    attackRange = originalattackRange;
    // 시각적 효과 복구
    sprite.setColor(sf::Color(255, 255, 255, 255)); // 원래 흰색으로 복원
    removeClones();

    isSkillActive = false;
}

void MidBoss::createClones() {
    if (!isCloneActive) {
        cloneSprites.clear();  // 기존 분신들 제거

        // 4개의 분신 생성
        for (int i = 0; i < 4; ++i) {
            sf::Sprite cloneSprite = sprite;
            cloneSprite.setColor(sf::Color(255, 255, 255, 128));  // 반투명 설정
            cloneSprites.push_back(cloneSprite);
        }
        isCloneActive = true;
    }
}

void MidBoss::updateClones(float deltaTime) {
    if (isCloneActive) {
        cloneRotationAngle += cloneRotationSpeed * deltaTime;

        for (size_t i = 0; i < cloneSprites.size(); ++i) {
            float angle = cloneRotationAngle + (i * 90.0f);  // 90도 간격으로 배치
            float radians = angle * 3.14159f / 180.0f;

            float xOffset = cloneDistance * std::cos(radians);
            float yOffset = cloneDistance * std::sin(radians);

            cloneSprites[i].setPosition(
                sprite.getPosition().x + xOffset,
                sprite.getPosition().y + yOffset
            );
        }
    }
}

void MidBoss::removeClones() {
    if (isCloneActive) {
        cloneSprites.clear();
        isCloneActive = false;
    }
}