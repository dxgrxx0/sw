#ifndef BASESKILL_H
#define BASESKILL_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>
#include <string>
#include <iostream>
class BaseSkill {
protected:
    std::string name;           // 스킬 이름
    sf::Keyboard::Key key;      // 스킬 활성화 키
    float cooldown;             // 쿨타임
    sf::Clock cooldownTimer;    // 쿨타임 타이머
    bool isActive;              // 활성화 여부

public:
    BaseSkill(const std::string& name, sf::Keyboard::Key key, float cooldown)
        : name(name), key(key), cooldown(cooldown), isActive(false) {}

    virtual ~BaseSkill() {}

    bool canActivate() const {
        return cooldownTimer.getElapsedTime().asSeconds() >= cooldown;
    }
    
    virtual void activate() {
        if (canActivate()) {
            isActive = true;
            cooldownTimer.restart();
            applyEffect(); // 스킬 효과 적용
        }
    }

    virtual void update(float deltaTime) {
        if (isActive) {
            isActive = false; // 스킬의 지속 시간이 끝났다고 가정
        }
    }
    float getRemainCool() {
        return (cooldown - cooldownTimer.getElapsedTime().asSeconds())<0?0: cooldown - cooldownTimer.getElapsedTime().asSeconds();
    }
    virtual void applyEffect() = 0; // 각 스킬 효과 구현

    sf::Keyboard::Key getKey() const { return key; }
    virtual void upgrade() = 0;
};

#endif // BASESKILL_H
