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

    sf::Texture& getTexture(const std::string& name) {
        return textures.at(name);  // Using at to get an exception if texture is not found
    }

	sf::Font& getFont(const std::string& name) {
		return fonts.at(name);  // Using at to get an exception if font is not found
	}

private:
    ResourceManager() {} // Private constructor for singleton pattern
    std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
};

#endif // RESOURCE_MANAGER_H
