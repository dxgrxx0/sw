#include "Character.h"
#include <iostream>
#include <cmath>  // If needed for calculations

Character::Character(const std::string& textureFile, float x, float y, float scale, float speed)
    : movementSpeed(speed), animationSpeed(0.1f), timeSinceLastFrame(0.0f),

    currentFrameIndex(0), isSwinging(false), frameWidth(96), frameHeight(97), totalFrames(5),attackRange(200),attackDamage(50),attackApplied(true),facingDirection(90.0f),attackCoolDown(1.0f),health(100),maxHealth(100),defense(0){

    if (!texture.loadFromFile(textureFile)) {
        std::cerr << "Failed to load texture" << std::endl;
        throw std::runtime_error("Failed to load texture");
    }

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setOrigin(frameWidth / 2, frameHeight / 2); // 원점을 중앙으로 설정
    sprite.setPosition(x + sprite.getGlobalBounds().width / 2, y + sprite.getGlobalBounds().height / 2);
    sprite.setScale(scale, scale);
    // 슬래시 텍스처 로드
    if (!slashTexture.loadFromFile("slash.png")) {
        throw std::runtime_error("Failed to load slash texture");
    }
    slashSprite.setTexture(slashTexture);
    slashSprite.setOrigin(48, 48);
    currentFrame = sf::IntRect(0, 0, frameWidth, frameHeight);
    sprite.setTextureRect(currentFrame);
}

void Character::handleInput(float deltaTime) {
    // 방향 입력 처리
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (!isSwinging) { currentFrame.top = 97; facingDirection = 270.0f; } // 북쪽
        sprite.move(0, -movementSpeed * deltaTime);
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (!isSwinging) { currentFrame.top = 0; facingDirection = 90.0f; }// 남쪽
        sprite.move(0, movementSpeed * deltaTime);
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (!isSwinging) { currentFrame.top = 291; facingDirection = 180.0f; } // 서쪽
        sprite.move(-movementSpeed * deltaTime, 0);
        
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (!isSwinging) { currentFrame.top = 194; facingDirection = 0.0f; } // 동쪽
        sprite.move(movementSpeed * deltaTime, 0);
    }

    // 스페이스바 입력 처리
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isSwinging) {
        startSwinging();
        slashSprite.setRotation(facingDirection - 90.f); // 캐릭터의 바라보는 방향으로 회전
        // 슬래시의 위치 설정
        
    }
}

void Character::updateAnimation(float deltaTime) {
    timeSinceLastFrame += deltaTime;

    if (timeSinceLastFrame >= animationSpeed) {
        currentFrame.left = currentFrameIndex * frameWidth;
        sprite.setTextureRect(currentFrame);

        if (isSwinging) {
            if (currentFrameIndex == 0)animationSpeed = 0.1f * attackCoolDown;
            if (currentFrameIndex == totalFrames / 2) attackApplied = false;
            if (currentFrameIndex >= totalFrames - 1) {
                currentFrameIndex = 0;
                isSwinging = false;
                currentFrame.top -= 388; // 다음 애니메이션 프레임으로 이동
                animationSpeed = 0.1f;
            }
            // 슬래시 스프라이트 위치는 스윙 시 고정
            slashSprite.setTextureRect(sf::IntRect(0, 0, 96, 96));
            float angleInRadians = degreesToRadians(facingDirection);
            slashSprite.setPosition(
                sprite.getPosition().x + attackRange * cos(angleInRadians)*0.2f*currentFrameIndex,
                sprite.getPosition().y + attackRange * sin(angleInRadians)*0.2f*currentFrameIndex
            );
            slashSprite.setScale(currentFrameIndex, currentFrameIndex);
            // 슬래시의 위치는 공격이 진행 중일 때만 설정됨
            currentFrameIndex++;
        }
        else {
            currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
        }
        timeSinceLastFrame = 0.0f;
    }
}

void Character::draw(sf::RenderTarget& target) {
    target.draw(sprite);

    if (isSwinging) {
        target.draw(slashSprite); // 슬래시 이미지 그리기
    }

}

sf::Vector2f Character::getPosition() {
    return sprite.getPosition();
}

void Character::startSwinging() {
    currentFrame.top += 388;
    isSwinging = true;
    currentFrameIndex = 0;
}
bool Character::getAttackApplied() {
	return attackApplied;
}
bool Character::getIsSwinging() {
    return isSwinging;
}
float Character::getAttackRange() {
    return attackRange;
}
float Character::getAttackDamage() {
    return attackDamage;
}
void Character::setAttackApplied(bool applied) {
	attackApplied = applied;
}
float Character::getHealth() {
    return health;
}
float Character::getMaxHealth() {
    return maxHealth;
}
void Character::setPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

void Character::basicAttack(std::vector<std::unique_ptr<Monster>>& monsters) {
    float attackRange = this->getAttackRange();
    float attackAngle = 90.0f; // 부채꼴 각도

    for (auto& monster : monsters) {
        if (isMonsterInAttackRange(this->getPosition(), monster->getPosition(), attackRange, attackAngle, facingDirection)) {
            monster->takeDamage(this->getAttackDamage());
        }
    }
    attackApplied = true;
}
void Character::heal(float healAmount) {
    this->health += healAmount;
    if (health > maxHealth)health = maxHealth;
}

bool Character:: isMonsterInAttackRange(const sf::Vector2f& characterPosition, const sf::Vector2f& monsterPosition,
    float attackRange, float attackAngle, float characterRotation) {
    // 캐릭터와 몬스터 간의 벡터 계산
    sf::Vector2f direction = monsterPosition - characterPosition;

    // 거리 계산
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (distance <= 50)return true;
    // 거리가 공격 범위 이내인지 체크
    if (distance > attackRange) {
        return false; // 공격 범위를 초과함
    }

    // 방향 각도 계산
    float monsterAngle = std::atan2(direction.y, direction.x) * 180 / 3.141592f; // radian을 degree로 변환
    float angleDiff = fmod((monsterAngle - characterRotation + 360), 360); // 각도 차이를 계산하여 0-360 범위로 맞춤

    // 공격 범위의 각도 체크
    return angleDiff <= attackAngle / 2 || angleDiff >= 360 - (attackAngle / 2);
}
void Character::takeDamage(float damageAmount) {
    damageAmount = damageAmount * (100 - defense) / 100;
    health -= damageAmount;
    if (health < 0) {
        health = 0;  // 체력은 0 이하로 내려가지 않음
    }
}
void Character::increaseAttackPower(float power) {
    attackDamage += power;
}
void Character::increaseMaxHealth(float hp) {
    maxHealth += hp;
}
void Character::reduceCooldown(float cooldown) {
    attackCoolDown -= cooldown;
}
void Character::increaseSpeed(float speed) {
    movementSpeed += speed;
}
void Character::increaseAttackRange(float range) {
    attackRange += range;
}
/*void Character::addSkill(std::unique_ptr<BaseSkill> skill) {
    skillManager.addSkill(std::move(skill));
}

void Character::activateSkill(sf::Keyboard::Key key) {
    skillManager.activateSkill(key);
}

void Character::updateSkills(float deltaTime) {
    skillManager.updateSkills(deltaTime);
}*/
void Character::setScale(float scale) {
    sprite.setScale(scale,scale);
}