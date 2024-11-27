#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include "BaseSkill.h"
#include<SFML/Graphics.hpp>

class SkillManager {
private:
    std::map<std::string, std::unique_ptr<BaseSkill>> skills; // 스킬 이름으로 관리
    std::map<std::string, bool> skillStates;                 // 스킬 잠금 상태 (true = 해제, false = 잠금)

public:
    SkillManager();

    void addSkill(const std::string& name, std::unique_ptr<BaseSkill> skill);
    bool hasSkill(const std::string& name) const;
    bool isSkillUnlocked(const std::string& name) const;
    void unlockSkill(const std::string& name);
    void activateSkill(sf::Keyboard::Key key);
    void updateSkills(float deltaTime);
	float getRemainingCooldown(const std::string& name) const;
    BaseSkill* getSkill(const std::string& skillName);
};

#endif // SKILL_MANAGER_H
