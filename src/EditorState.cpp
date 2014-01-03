#include "EditorState.hpp"

#include <iostream>

#include "Wall.hpp"

EditorState::EditorState() {
}

void EditorState::onInit() {
    for(int i = 0; i < 5; ++i) {
        std::shared_ptr<Wall> wall = std::make_shared<Wall>();
        wall->m_position = glm::vec2(i, 0);
        add(wall);
    }

    m_currentEntity = m_entities[0];
}

void EditorState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Tab) {
        }
    }
}

void EditorState::onUpdate(double dt) {
}

void EditorState::onDraw(sf::RenderTarget& target) {
    sf::View view;
    float zoom = 6;
    float w = zoom;
    float h = w / target.getSize().x * target.getSize().y;
    glm::vec2 pos(0, 0);
    view.reset(sf::FloatRect(pos.x-w/2, pos.y-h/2, w, h));
    //view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
    target.setView(view);
}

