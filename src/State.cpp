#include "State.hpp"

#include "Root.hpp"

#include <iostream>

void State::update(double dt) {
    onUpdate(dt);
    for(auto entity : m_entities) {
        entity->onUpdate(dt);
    }
}

void State::draw(sf::RenderTarget& target) {
    onDraw(target);
}

void State::handleEvent(sf::Event& event) {
    onHandleEvent(event);
    for(auto entity : m_entities) {
        entity->onHandleEvent(event);
    }
}

void State::onUpdate(double dt) {
}

void State::onDraw(sf::RenderTarget& target) {
    drawEntities(target);
}

void State::onHandleEvent(sf::Event& event) {
}

void State::add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
}

glm::vec2 State::getMousePosition() {
    sf::Vector2i windowCoords = sf::Mouse::getPosition(*Root().window);
    sf::Vector2f worldCoords = Root().window->mapPixelToCoords(windowCoords);
    return glm::vec2(worldCoords.x, worldCoords.y);
}

void State::drawEntities(sf::RenderTarget& target) {
    for(auto entity : m_entities) {
        entity->onDraw(target);
    }
}

void State::setView(sf::RenderTarget& target) {
    float w = m_zoom;
    float h = w / target.getSize().x * target.getSize().y;
    m_pixelSize = w / target.getSize().x;
    m_view.reset(sf::FloatRect(m_center.x-w/2, m_center.y-h/2, w, h));
    target.setView(m_view);
}
