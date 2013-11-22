#include "State.hpp"

State::State() {}

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

void State::event(sf::Event& event) {
    onEvent(event);
    for(auto entity : m_entities) {
        entity->onEvent(event);
    }
}

void State::onUpdate(double dt) {}
void State::onDraw(sf::RenderTarget& target) {}
void State::onEvent(sf::Event& event) {}

void State::add(std::shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
}
