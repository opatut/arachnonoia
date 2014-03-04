#include "ResourceManager.hpp"

#include <iostream>

void ResourceManager::addTexture(const std::string& name, const std::string& filename) {
    auto texture = std::make_shared<sf::Texture>();
    texture->loadFromFile(filename);
    texture->setSmooth(true);
    m_textures[name] = texture;
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const std::string& name) {
    return m_textures[name];
}

void ResourceManager::addFont(const std::string& name, const std::string& filename) {
    auto font = std::make_shared<sf::Font>();
    font->loadFromFile(filename);
    m_fonts[name] = font;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const std::string& name) {
    return m_fonts[name];
}

void ResourceManager::addSound(const std::string& name, const std::string& filename) {
    auto sound = std::make_shared<sf::SoundBuffer>();
    sound->loadFromFile(filename);
    m_sounds[name] = sound;
}

std::shared_ptr<sf::SoundBuffer> ResourceManager::getSound(const std::string& name) {
    return m_sounds[name];
}


void ResourceManager::addShader(const std::string& name, const std::string& filename, sf::Shader::Type type) {
    auto shader = std::make_shared<sf::Shader>();
    shader->loadFromFile(filename, type);
    m_shaders[name] = shader;
}

std::shared_ptr<sf::Shader> ResourceManager::getShader(const std::string& name) {
    return m_shaders[name];
}
