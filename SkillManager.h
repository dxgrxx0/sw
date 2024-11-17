#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H

#include <vector>
#include <memory>
#include <SFML/Window/Keyboard.hpp>
#include "BaseSkill.h"

class SkillManager {
private:
    std::vector<std::unique_ptr<BaseSkill>> skills;

public:
    void addSkill(std::unique_ptr<BaseSkill> skill);
    void activateSkill(sf::Keyboard::Key key); // 키 입력에 따라 스킬 활성화
    void updateSkills(float deltaTime);
};

#endif // SKILLMANAGER_H
