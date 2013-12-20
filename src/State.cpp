#include "State.hpp"

void State::update(double dt) {
    onUpdate(dt);
    for(auto entity : m_entities) {
        entity->onUpdate(dt);
    }
}

void State::draw(sf::RenderTarget& target) {
    onDraw(target);
    for(auto entity : m_entities) {
        entity->onDraw(target);
    }
}

void State::handleEvent(sf::Event& event) {
    onHandleEvent(event);
    for(auto entity : m_entities) {
        entity->onHandleEvent(event);
    }
}

void State::add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
}
