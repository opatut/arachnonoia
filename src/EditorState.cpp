#include "EditorState.hpp"

#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

#include "Wall.hpp"
#include "Root.hpp"

#include <glm/gtx/vector_angle.hpp>

EditorState::EditorState()
{
    // we don't want physics in the editor :(
    m_usePhysics = false;
}

void EditorState::onInit() {
    for(int i = 0; i < 5; ++i) {
        std::shared_ptr<Wall> wall = std::make_shared<Wall>();
        wall->setPosition(glm::vec2(i * 1.5 - 3, 1));
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
        } else if(event.key.code == sf::Keyboard::F5) {
            std::cout << "saving... " << std::endl;

            cereal::JSONOutputArchive ar(std::cout);
            ar(cereal::make_nvp("entities", m_entities));
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
    m_statusTime -= dt;

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
    highlight.setPosition(m_currentEntity->position().x, m_currentEntity->position().y);
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

    if(m_statusTime > 0) {
        sf::Text text;
        text.setFont(* Root().resources.getFont("default"));
        text.setPosition(20, 20);
        text.setString(m_statusText);
        text.setColor(sf::Color(255, 255, 255, (int)(m_statusTime * 255 / 1.5)));
        text.setCharacterSize(12);
        target.draw(text);
    }

    setView(target);
}

void EditorState::startMode(EditorMode mode) {
    m_mode = mode;

    m_modeStartPosition = getMousePosition();

    if(m_mode == NONE) {
        return;
    } else if(m_mode == GRAB) {
        m_modeStartValue = m_currentEntity->position();
    } else if(m_mode == ROTATE) {
        m_modeStartValue.x = m_currentEntity->rotation();
    } else if(m_mode == SCALE) {
        m_modeStartValue = m_currentEntity->scale();
    }
}

void EditorState::updateMode() {
    auto mp = getMousePosition();
    glm::vec2 entity_start = m_currentEntity->position() - m_modeStartPosition;
    glm::vec2 entity_mouse = m_currentEntity->position() - mp;
    float entity_start_length = glm::length(entity_start);
    float entity_mouse_length = glm::length(entity_mouse);

    if(m_mode == GRAB) {
        glm::vec2 diff = mp - m_modeStartPosition;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            diff.x = round(diff.x * 10) / 10.0;
            diff.y = round(diff.y * 10) / 10.0;
        }

        m_currentEntity->setPosition(m_modeStartValue + diff);
        setStatus("Move: " + std::to_string(diff.x) + "|" + std::to_string(diff.y));
    } else if(m_mode == ROTATE) {
        float angle = glm::orientedAngle(glm::normalize(entity_start), glm::normalize(entity_mouse));
        if(entity_start == entity_mouse) angle = 0; // -nan failsafe

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            angle = round(angle/15)*15;
        }

        m_currentEntity->setRotation(m_modeStartValue.x + angle);
        setStatus("Rotate: " + std::to_string((int)angle));
    } else if(m_mode == SCALE) {
        float scale = ((entity_mouse_length != 0) ? (entity_mouse_length / entity_start_length) : 0.f);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            scale = round(scale*10)/10.0;
        }
        m_currentEntity->setScale(m_modeStartValue * scale);
        setStatus("Scale: " + std::to_string(scale));
    }
}

void EditorState::commitMode() {
    m_mode = NONE;
}

void EditorState::cancelMode() {
    if(m_mode == GRAB) {
        m_currentEntity->setPosition(m_modeStartValue);
    } else if(m_mode == ROTATE) {
        m_currentEntity->setRotation(m_modeStartValue.x);
    } else if(m_mode == SCALE) {
        m_currentEntity->setScale(m_modeStartValue);
    }

    m_mode = NONE;
}

void EditorState::setStatus(const std::string& text) {
    m_statusText = text;
    m_statusTime = 1.5f;
}
