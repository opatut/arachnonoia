#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class ResourceManager {
public:
    void addTexture(const std::string& name, const std::string& filename);
    std::shared_ptr<sf::Texture> getTexture(const std::string& name);

    void addFont(const std::string& name, const std::string& filename);
    std::shared_ptr<sf::Font> getFont(const std::string& name);

private:
    std::map<std::string, std::shared_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::shared_ptr<sf::Font>> m_fonts;
};

#endif
