#include "Monster.h"
#include "Utility.h"
#include <cmath>
#include "Character.h"
#include "MainTower.h"
#include <iostream>

// 생성자
Monster::Monster(float x, float y, float speed,MonsterType type)
    : movementSpeed(speed),damageTaken(0.0f), isTakingDamage(false), damageDisplayDuration(0.3f), damageDisplayTime(0.0f),attackPower(0),defense(0)
    ,attackRange(50),skillDuration(5.0f), isSkillActive(false), isCloneActive(false)
    , cloneDistance(50.0f), cloneRotationAngle(0.0f), cloneRotationSpeed(180.0f)
    {
    shape.setSize(sf::Vector2f(30.0f, 30.0f));
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(x, y);
    shape.setOrigin(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2); // 원점을 중앙으로 설정
	healthPoint = 100.0f;
    originalSpeed = speed;
    originalDefense = defense;
    originalAttackPower = attackPower;
    originalattackRange = attackRange;
    switch (type) {
    case MonsterType::Speed:  //이속 3배,체력 1/2배
        texturePath = ("speedMonster.PNG");
        movementSpeed = 150.0f;
        healthPoint = 50.0f;
        attackPower = 10.0f;
        defense = 10.0f;
        //shape.setFillColor(sf::Color::Cyan);
        break;
    case MonsterType::Attack: //공격력 3배
        texturePath = ("attackMonster.PNG");
        movementSpeed = 50.0f;
        healthPoint = 100.0f;
        defense = 10.0f;
        attackPower = 30.0f; // 추가 공격력
        //shape.setFillColor(sf::Color::Red);
        break;
    case MonsterType::Defense: //방어력3배,체력2배
        texturePath = ("defenseMonster.PNG");
        movementSpeed = 50.0f;
        healthPoint = 200.0f;
        attackPower = 10.0f;
        defense = 30.0f; // 방어력 추가
        //shape.setFillColor(sf::Color::Magenta);
        break;
    case MonsterType::Mid_Boss: // 미드보스
        texturePath = "midboss.PNG";
        movementSpeed = 100.0f;
        healthPoint = 1000.0f;  
        attackPower = 50.0f;   
        defense = 20.0f;       
        skillDuration = 5.0f;
        //shape.setFillColor(sf::Color::Yellow);
        break;
    case MonsterType::Main_Boss: // 메인보스
        texturePath = "mainboss.PNG";
        movementSpeed = 80.0f;
        healthPoint = 5000.0f; 
        attackPower = 100.0f;  
        defense = 50.0f;       
        skillDuration = 8.0f;
        //shape.setFillColor(sf::Color::Black);
        break;
    case MonsterType::Basic:
    default:
        texturePath = ("basicMonster.PNG");
        movementSpeed = 50.0f;
        healthPoint = 100.0f;
        attackPower = 10.0f;
        defense = 10.0f;
        //shape.setFillColor(sf::Color::White);
        break;

    }
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        // 텍스처 로딩 실패시 기본 색상 설정
        shape.setFillColor(sf::Color::White);
    }
    else {
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(x, y);
        //sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        sprite.setOrigin(500, 500);
    }
}
void Monster::createClones() {
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

void Monster::updateClones(float deltaTime) {
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

void Monster::removeClones() {
    if (isCloneActive) {
        cloneSprites.clear();
        isCloneActive = false;
    }
}

// update 함수 구현
void Monster::update(const sf::Vector2f& heroinePosition, const sf::Vector2f& towerPosition, float deltaTime,Character& character,MainTower& mainTower) {
    sf::Vector2f targetPosition;
    float distanceToHeroine = calculateDistance(sprite.getPosition(), heroinePosition);
    float distanceToTower = calculateDistance(sprite.getPosition(), towerPosition);

    if (distanceToHeroine < distanceToTower) {
        targetPosition = heroinePosition;
    }
    else {
        targetPosition = towerPosition;
    }
    //투사체업데이트
    updateProjectiles(deltaTime, character, mainTower);

    sf::Vector2f direction = targetPosition - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    // 특정 거리 이내로 접근했을 때 피해를 입힘
    if (length <= attackRange) {
        if (attackTimer.getElapsedTime().asSeconds() >= attackCooldown) {
            if (targetPosition == towerPosition) {
                mainTower.takeDamage(attackPower);
                //printf("Tower takeDamage!");
            }
            else { 
                character.takeDamage(attackPower);
                //printf("character takeDamage!");
            } // 캐릭터에 피해 입힘
            attackTimer.restart(); // 타이머 초기화
        }
    }
    else if (length >=attackRange) {
        direction /= length; // 방향 정규화
        sprite.move(direction * movementSpeed * deltaTime); // 몬스터 이동
    }
    if (isTakingDamage) {
        damageDisplayTime += deltaTime;
        if (damageDisplayTime >= damageDisplayDuration) {
            isTakingDamage = false; // 피해 표시 종료
            damageTaken = 0.0f; // 피해량 초기화
        }
    }
    // 메인 스킬 (15초 쿨타임)
    if (skillCooldown.getElapsedTime().asSeconds() >= 15.0f) {
        Fir_useSkill(character, mainTower);
    }

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
}

// draw 함수 구현
void Monster::draw(sf::RenderTarget& target)const {
    if (isCloneActive) {
        for (const auto& cloneSprite : cloneSprites) {
            target.draw(cloneSprite);
        }
    }
    target.draw(sprite);

    drawProjectiles(target); //투사체 그리기

    if (isTakingDamage) {
        sf::Font font;
        font.loadFromFile("arial.ttf"); // 폰트 로드 (폰트 파일이 필요합니다)
        sf::Text damageText;
        damageText.setFont(font);
        damageText.setString(std::to_string(static_cast<int>(damageTaken))); // 피해량을 문자열로 변환
        damageText.setCharacterSize(15);
        damageText.setFillColor(sf::Color::White);


        damageText.setPosition(sprite.getPosition().x, sprite.getPosition().y - 70); // 위치 조정

        target.draw(damageText); // 피해량 텍스트 그리기
    }

}

// getPosition 함수 구현
sf::Vector2f Monster::getPosition() {
    return sprite.getPosition();
}
// 특정 위치 근처에 있는지 확인
bool Monster::isNear(sf::Vector2f position, float radius) const {
    sf::Vector2f distanceVec = position - sprite.getPosition();
    float distance = std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);
    return distance <= radius;
}

void Monster::takeDamage(float attackDamage) {
    attackDamage = attackDamage * (100 - defense) / 100;
    healthPoint -= attackDamage;
    if (attackDamage > 0) {
        // 피해량 저장
        damageTaken = attackDamage;
        isTakingDamage = true;
        damageDisplayTime = 0.0f; // 초기화

    }
}
float Monster::getHealthPoint()const {
    return healthPoint;
}

//원거리 공격 로직 추가

void Monster::shootProjectile(const sf::Vector2f& targetPos) {
    sf::Vector2f direction = targetPos - sprite.getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    projectiles.emplace_back(sprite.getPosition(), direction * projectileSpeed, attackPower);
}

void Monster::updateProjectiles(float deltaTime, Character& character, MainTower& mainTower) {
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
            printf("Tower hit by projectile!\n"); // 디버깅용 메시지 추가
            continue;
        }

        // 화면 밖으로 나간 투사체 제거
        if (projectilePos.x < -100 || projectilePos.x > 2100 ||
            projectilePos.y < -100 || projectilePos.y > 2100) {
            projectile.active = false;
        }
    }

    // 비활성화된 투사체 제거
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](const Projectile& p) { return !p.active; }),
        projectiles.end());
}

void Monster::drawProjectiles(sf::RenderTarget& target) const {
    for (const auto& projectile : projectiles) {
        if (projectile.active) {
            target.draw(projectile.shape);
        }
    }
}
//아니 이거 왜안됨 ㅋㅋ (pull용 지워도됨)
void Monster::Fir_useSkill(Character& character, MainTower& mainTower) {
    if (texturePath == "midboss.PNG") {
        // 미드보스 스킬: "광폭화" - 이동속도와 공격력 증가
        movementSpeed = originalSpeed * 2.5f;
        attackPower = originalAttackPower * 1.5f;
        attackRange = originalattackRange * 2.0f;

        createClones();

        // 시각적 효과
        sprite.setColor(sf::Color::Magenta);
        printf("Mid Boss uses Berserk!\n");
    }
    else if (texturePath == "mainboss.PNG") {
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
        printf("Main Boss uses Absolute Defense!\n");
    }

    isSkillActive = true;
    skillCooldown.restart();
}

void Monster::Sec_useSkill(Character& character, MainTower& mainTower) {
    if (texturePath == "midboss.PNG") {
        // 미드보스의 원거리 공격
        for (int i = 0; i < 8; ++i) {
            float angle = i * 45.0f * 3.14159f / 180.0f;
            sf::Vector2f direction(std::cos(angle), std::sin(angle));
            sf::Vector2f targetPos = sprite.getPosition() + direction * 100.f;
            shootProjectile(targetPos);
        }
        printf("Mid Boss uses Ranged Attack!\n");
    }
    else if (texturePath == "mainboss.PNG") {
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
        printf("Main Boss uses Ranged Attack!\n");
    }
}

void Monster::removeSkillEffects() {
    // 스킬 효과 제거
    movementSpeed = originalSpeed;
    attackPower = originalAttackPower;
    defense = originalDefense;
    attackRange = originalattackRange;
    // 시각적 효과 제거
    sprite.setColor(sf::Color::White);

    removeClones();

    isSkillActive = false;
}