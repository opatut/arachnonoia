#include "EditorState.hpp"

#include <iostream>

#include "Wall.hpp"
#include "Root.hpp"

#include <glm/gtx/vector_angle.hpp>

EditorState::EditorState() {
}

void EditorState::onInit() {
    for(int i = 0; i < 5; ++i) {
        std::shared_ptr<Wall> wall = std::make_shared<Wall>();
        wall->m_position = glm::vec2(i * 1.5 - 3, 1);
        add(wall);
    }

    m_currentEntity = m_entities[2];
}

void EditorState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::G) {
            startMode(GRAB);
        } else if(event.key.code == sf::Keyboard::R) {
            startMode(ROTATE);
        } else if(event.key.code == sf::Keyboard::S) {
            startMode(SCALE);
        } else if(event.key.code == sf::Keyboard::Escape) {
            cancelMode();
        } else if(event.key.code == sf::Keyboard::Return) {
            commitMode();
        }
    }

    if(event.type == sf::Event::MouseButtonPressed) {
        if(m_mode != NONE) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                commitMode();
            } else if(event.mouseButton.button == sf::Mouse::Right) {
                cancelMode();
            }
        }
    }
}

void EditorState::onUpdate(double dt) {
    m_zoom = 6;

    float speed = 1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_center.x -= dt * speed;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_center.x += dt * speed;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        m_center.y -= dt * speed;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        m_center.y += dt * speed;
    }

    updateMode();
}

void EditorState::onDraw(sf::RenderTarget& target) {
    Root().window->clear(sf::Color(60, 60, 60));

    setView(target);

    drawEntities(target);

    // draw current entity highlight
    sf::CircleShape highlight;
    highlight.setPosition(m_currentEntity->m_position.x, m_currentEntity->m_position.y);
    float r = 3 * m_pixelSize;
    highlight.setRadius(r);
    highlight.setOrigin(r, r);
    highlight.setFillColor(sf::Color(255, 128, 0, 128));
    highlight.setOutlineThickness(m_pixelSize);
    highlight.setOutlineColor(sf::Color(255, 128, 0));
    target.draw(highlight);

    auto mp = getMousePosition();

    if(m_mode != NONE) {
        // debug
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(mp.x, mp.y), sf::Color(0, 255, 0)),
            sf::Vertex(sf::Vector2f(m_modeStartPosition.x, m_modeStartPosition.y), sf::Color(255, 255, 0))
        };
        target.draw(line, 2, sf::Lines);
    }

    // reset the view
    target.setView(target.getDefaultView());

    sf::Text text;
    text.setFont(* Root().resources.getFont("default"));
    text.setPosition(20, 20);
    text.setString("Status: " + std::to_string(m_mode));
    text.setColor(sf::Color::White);
    text.setCharacterSize(12);
    target.draw(text);

    setView(target);
}

void EditorState::startMode(EditorMode mode) {
    m_mode = mode;

    m_modeStartPosition = getMousePosition();

    if(m_mode == NONE) {
        return;
    } else if(m_mode == GRAB) {
        m_modeStartValue = m_currentEntity->m_position;
    } else if(m_mode == ROTATE) {
        m_modeStartValue.x = m_currentEntity->m_rotation;
    } else if(m_mode == SCALE) {
        m_modeStartValue = m_currentEntity->m_scale;
    }
}

void EditorState::updateMode() {
    auto mp = getMousePosition();
    glm::vec2 entity_start = m_currentEntity->m_position - m_modeStartPosition;
    glm::vec2 entity_mouse = m_currentEntity->m_position - mp;
    float entity_start_length = glm::length(entity_start);
    float entity_mouse_length = glm::length(entity_mouse);

    if(m_mode == GRAB) {
        m_currentEntity->m_position = m_modeStartValue + (mp - m_modeStartPosition);
    } else if(m_mode == ROTATE) {
        float angle = glm::orientedAngle(glm::normalize(entity_start), glm::normalize(entity_mouse));
        if(entity_start == entity_mouse) angle = 0; // -nan failsafe
        m_currentEntity->m_rotation = m_modeStartValue.x + angle;
    } else if(m_mode == SCALE) {
        float scale = ((entity_mouse_length != 0) ? (entity_mouse_length / entity_start_length) : 0.f);
        m_currentEntity->m_scale = m_modeStartValue * scale;
    }
}

void EditorState::commitMode() {
    m_mode = NONE;
}

void EditorState::cancelMode() {
    if(m_mode == GRAB) {
        m_currentEntity->m_position = m_modeStartValue ;
    } else if(m_mode == ROTATE) {
        m_currentEntity->m_rotation = m_modeStartValue.x;
    } else if(m_mode == SCALE) {
        m_currentEntity->m_scale = m_modeStartValue;
    }

    m_mode = NONE;
}
