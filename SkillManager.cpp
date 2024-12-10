#include "SkillManager.h"

SkillManager::SkillManager() {
    // 초기 상태: 모든 스킬 잠금
    skillStates["BladeWhirl"] = false;
    skillStates["BulkUp"] = false;
    skillStates["Teleport"] = false;
}

BaseSkill* SkillManager::getSkill(const std::string& skillName) {
    auto it = skills.find(skillName);
    if (it != skills.end()) {
        return it->second.get();  // std::unique_ptr에서 포인터를 반환
    }
    return nullptr;
}

void SkillManager::addSkill(const std::string& name, std::unique_ptr<BaseSkill> skill) {
    skills[name] = std::move(skill);
    skillStates[name] = true; // 스킬 추가 시 잠금 해제
}

bool SkillManager::hasSkill(const std::string& name) const {
    return skills.find(name) != skills.end();
}

bool SkillManager::isSkillUnlocked(const std::string& name) const {
    auto it = skillStates.find(name);
    return it != skillStates.end() && it->second;
}

void SkillManager::unlockSkill(const std::string& name) {
    if (skillStates.find(name) != skillStates.end()) {
        skillStates[name] = true; // 잠금 해제
    }
}

void SkillManager::activateSkill(sf::Keyboard::Key key) {
    for (const auto& skillPair : skills) { // skills는 std::map<std::string, std::unique_ptr<BaseSkill>>
        const std::string& name = skillPair.first;
        const std::unique_ptr<BaseSkill>& skill = skillPair.second;

        if (skill->getKey() == key && isSkillUnlocked(name)) {
            skill->activate();
            break;
        }
    }
}
float SkillManager::getRemainingCooldown(const std::string& skillName) const {
    auto it = skills.find(skillName);
	if (it != skills.end()) { // 스킬이 존재하는 경우
        return it->second->getRemainCool(); // 스킬 객체에서 쿨타임 가져오기
    }
    return -1.0f; // 스킬이 존재하지 않으면 -1 반환
}
void SkillManager::upgradeSkil(const std::string& skillName) {
    auto it = skills.find(skillName);
	if (it != skills.end()) {
		it->second->upgrade();
	}
}
void SkillManager::updateSkills(float deltaTime) {
    for (const auto& skillPair : skills) {
        const std::unique_ptr<BaseSkill>& skill = skillPair.second;

        skill->update(deltaTime);
    }
}
