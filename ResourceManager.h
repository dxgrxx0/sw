#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    void loadTexture(const std::string& name, const std::string& filename) {
        auto& tex = textures[name];
        if (!tex.loadFromFile(filename)) {
            throw std::runtime_error("Failed to load texture: " + filename);
        }
    }
	void loadFont(const std::string& name, const std::string& filename) {
		auto& font = fonts[name];
		if (!font.loadFromFile(filename)) {
			throw std::runtime_error("Failed to load font: " + filename);
		}
	}
    void loadImage(const std::string& name, const std::string& filename) {
		auto& img = images[name];
        if (!img.loadFromFile(filename)) {
			throw std::runtime_error("Failed to load image: " + filename);
		}
	}
    void loadResources() {
        // Texture 로드
        loadTexture("ArrowTower", "ArrowTower.png");
        loadTexture("WizardTower", "WizardTower.png");
        loadTexture("TrainingTower", "TrainingTower.png");
        loadTexture("BombTower", "BombTower.png");
        loadTexture("CannonTower", "CannonTower.png");
        loadTexture("SpeedMonster", "speedMonster.png");
        loadTexture("AttackMonster", "attackMonster.png");
        loadTexture("BasicMonster", "basicMonster.png");
        loadTexture("DefenseMonster", "defenseMonster.png");
        loadTexture("MainBoss", "mainboss.png");
        loadTexture("MidBoss", "midboss.png");
        loadTexture("BossExplode", "BossExplode.png");
        loadTexture("GreenBomb", "greenBomb.png");
        loadTexture("GreenBombExplode", "greenBombExplode.png");
        loadTexture("PinkBomb", "pinkBomb.png");
        loadTexture("PinkBombExplode", "pinkBombExplode.png");
        loadTexture("YellowBomb", "yellowBomb.png");
        loadTexture("YellowBombExplode", "yellowBombExplode.png");
        loadTexture("BlueBomb", "blueBomb.png");
        loadTexture("BlueBombExplode", "blueBombExplode.png");
        loadTexture("RedPen", "RedPen.png");
        loadTexture("OrangePen", "OrangePen.png");
        loadTexture("YellowPen", "YellowPen.png");
        loadTexture("GreenPen", "GreenPen.png");
        loadTexture("BluePen", "BluePen.png");
        loadTexture("NavyPen", "NavyPen.png");
        loadTexture("PurplePen", "PurplePen.png");
        loadTexture("pencil", "pencil.png");
        loadImage("MainBossYellowSkill", "MainBossYellowSkill.png");
        loadImage("MainBossRedSkill", "MainBossRedSkill.png");
        loadImage("MainBossSkillPurple", "MainBossSkillPurple.png");
        loadImage("MainBossPencilSkill", "MainBossPencilSkill.png");
        loadTexture("UpgradeMainTower", "UpgradeMainTower.png");
        loadTexture("UpgradeArrowTower", "UpgradeArrowTower.png");
        loadTexture("UpgradeWizardTower", "UpgradeWizardTower.png");
        loadTexture("UpgradeTrainingTower", "UpgradeTrainingTower.png");
        loadTexture("UpgradeBombTower", "UpgradeBombTower.png");
        loadTexture("UpgradeCannonTower", "UpgradeCannonTower.png");
        loadTexture("UpgradeAttackPower", "UpgradeAttackPower.png");
        loadTexture("UpgradeHealth", "UpgradeHealth.png");
        loadTexture("UpgradeBasicCooldown", "UpgradeBasicCooldown.png");
        loadTexture("UpgradePlayerSpeed", "UpgradePlayerSpeed.png");
        loadTexture("UpgradeBladeWhirl", "UpgradeBladeWhirl.png");
        loadTexture("UpgradeBulkUp", "UpgradeBulkUp.png");
        loadTexture("UpgradeDash", "UpgradeDash.png");
        loadTexture("QSkillUI", "QSkillUI.png");
        loadTexture("WSkillUI", "WSkillUI.png");
        loadTexture("ESkillUI", "ESkillUI.png");
        loadTexture("RSkillUI", "RSkillUI.png");
        loadTexture("SkillLock", "SkillLock.png");

        // Font 로드
        loadFont("Arial", "arial.ttf");
        loadFont("Pixel", "PixelOperator8.ttf");
    }
    sf::Texture& getTexture(const std::string& name) {
        return textures.at(name);  // Using at to get an exception if texture is not found
    }

	sf::Font& getFont(const std::string& name) {
		return fonts.at(name);  // Using at to get an exception if font is not found
	}
	sf::Image& getImage(const std::string& name) {
		return images.at(name);  // Using at to get an exception if image is not found
	}
private:
    ResourceManager() { loadResources(); } // Private constructor for singleton pattern
    std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::map<std::string, sf::Image> images;
};

#endif // RESOURCE_MANAGER_H
