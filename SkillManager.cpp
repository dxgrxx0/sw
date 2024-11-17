#include "SkillManager.h"

void SkillManager::addSkill(std::unique_ptr<BaseSkill> skill) {
    skills.push_back(std::move(skill));
}

void SkillManager::activateSkill(sf::Keyboard::Key key) {
    for (auto& skill : skills) {
        if (skill->getKey() == key) {
            skill->activate(); // 해당 키에 할당된 스킬 활성화
        }
    }
}

void SkillManager::updateSkills(float deltaTime) {
    for (auto& skill : skills) {
        skill->update(deltaTime); // 모든 스킬 업데이트 (쿨타임 등)
    }
}
