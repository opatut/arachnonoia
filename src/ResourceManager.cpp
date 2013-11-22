#include "ResourceManager.hpp"

void ResourceManager::addTexture(const std::string& name, const std::string& filename) {
    auto texture = std::make_shared<sf::Texture>();
    texture->loadFromFile(filename);
    texture->setSmooth(true);
    m_textures[name] = texture;
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& name) {
    return m_textures[name];
}
