#include "EditorState.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

#include <Thor/Math.hpp>

#include "Wall.hpp"
#include "Pair.hpp"
#include "Root.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtx/vector_angle.hpp>

EditorState::EditorState() {
    m_zoom = 6;
    m_insertModeCurrentType = WALL;
}

void EditorState::onInit() {
    for(int i = 0; i < 5; ++i) {
        auto wall = std::make_shared<Wall>();
        wall->setPosition(glm::vec2(i * 1.5 - 3, 1));
        add(wall);
    }
    // m_currentEntity = m_entities[2];
}

void EditorState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::TextEntered && m_typing) {
        if(event.text.unicode <= 126 && event.text.unicode >= 32) {
            m_typingString += static_cast<char>(event.text.unicode);
        }
    }

    if(event.type == sf::Event::TextEntered) {
        if(event.text.unicode >= 48 && event.text.unicode <= 57) {
            int num = event.text.unicode - 48;

            if(m_mode == INSERT && num >= WALL && num <= PAIR) {
                m_insertModeCurrentType = (EntityType)num;
                remove(m_currentEntity);
                m_currentEntity = createNewEntity(m_insertModeCurrentType);
                add(m_currentEntity);
            }

            if(m_mode == NONE && m_currentEntity) {
                setStatus("Setting metadata: " + std::to_string(num));
                m_currentEntity->setMetadata(num);
            }
        }
    }

    if(event.type == sf::Event::KeyPressed) {
        if(m_typing && event.key.code == sf::Keyboard::BackSpace) {
            m_typingString = m_typingString.substr(0, m_typingString.length() - 1);
        } else if(m_mode == FOLLOW) {
            if(event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                // char!
                char c = 'A' + (event.key.code - sf::Keyboard::A);
                m_followModeInput += c;

                for(auto it = m_entityNumbers.begin(); (it = std::find_if(it, m_entityNumbers.end(), [c,this](std::pair<std::shared_ptr<Entity>, std::string> pair) -> bool {
                    return c != pair.second[m_followModeInput.length()-1];
                })) != m_entityNumbers.end(); )
                    m_entityNumbers.erase(it++);

                if(m_entityNumbers.size() == 1) {
                    m_currentEntity = m_entityNumbers.begin()->first;
                    commitMode();
                } else if(m_entityNumbers.size() == 0) {
                    m_currentEntity.reset();
                    cancelMode();
                }
            }
        } else {
            if(event.key.code == sf::Keyboard::G) {
                if(m_mode == NONE) startMode(GRAB);
            } else if(event.key.code == sf::Keyboard::R) {
                if(m_mode == NONE) startMode(ROTATE);
            } else if(event.key.code == sf::Keyboard::S) {
                if(m_mode == NONE) startMode(SCALE);
            } else if(event.key.code == sf::Keyboard::F) {
                if(m_mode == NONE) startMode(FOLLOW);
            } else if(event.key.code == sf::Keyboard::Space) {
                if(m_mode == NONE) startMode(INSERT);
            } else if(event.key.code == sf::Keyboard::X || event.key.code == sf::Keyboard::Delete) {
                if(m_mode == NONE && m_currentEntity) {
                    remove(m_currentEntity);
                    m_currentEntity.reset();
                    setStatus("Deleted.");
                }
            } else if(event.key.code == sf::Keyboard::Add) {
                if(m_currentEntity) {
                    m_currentZLevel = m_currentEntity->zLevel() + 1;
                    m_currentEntity->setZLevel(m_currentZLevel);
                    setStatus("Z-Level ++ " + std::to_string(m_currentZLevel));
                }
            } else if(event.key.code == sf::Keyboard::Subtract) {
                if(m_currentEntity) {
                    m_currentZLevel = m_currentEntity->zLevel() - 1;
                    m_currentEntity->setZLevel(m_currentZLevel);
                    setStatus("Z-Level -- " + std::to_string(m_currentZLevel));
                }
            }
        }

        if(event.key.code == sf::Keyboard::Escape) {
            cancelMode();
        } else if(event.key.code == sf::Keyboard::Return) {
            commitMode();
        } else if(event.key.code == sf::Keyboard::F5) {
            if(m_mode == NONE) startMode(SAVE);
        } else if(event.key.code == sf::Keyboard::F6) {
            if(m_mode == NONE) startMode(LOAD);
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

    if(event.type == sf::Event::MouseWheelMoved) {
        float new_zoom = m_zoom * (1.f - 0.2 * event.mouseWheel.delta);

        glm::vec2 mp = getMousePosition();
        m_center += (mp - m_center) / m_zoom * (new_zoom / m_zoom);
        // glm::vec2 diff = (mp - m_center) / m_zoom;
        // m_center += diff;

        m_zoom = new_zoom;
    }
}

void EditorState::onUpdate(float dt) {
    m_statusTime += dt;

    float speed = 1 * m_zoom;
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
    if(m_currentEntity) {
        sf::CircleShape highlight;
        highlight.setPosition(m_currentEntity->position().x, m_currentEntity->position().y);
        float r = 3 * m_pixelSize;
        highlight.setRadius(r);
        highlight.setOrigin(r, r);
        highlight.setFillColor(sf::Color(255, 128, 0, 128));
        highlight.setOutlineThickness(m_pixelSize);
        highlight.setOutlineColor(sf::Color(255, 128, 0));
        target.draw(highlight);

        glm::vec2 s = m_currentEntity->getSize();
        sf::RectangleShape rect(sf::Vector2f(s.x * m_currentEntity->scale().x, s.y * m_currentEntity->scale().y));
        rect.setPosition(m_currentEntity->position().x, m_currentEntity->position().y);
        rect.setRotation(thor::toDegree(m_currentEntity->rotation()));
        rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
        rect.setOutlineColor(sf::Color(255, 128, 255, 200));
        rect.setOutlineThickness(m_pixelSize);
        rect.setFillColor(sf::Color::Transparent);
        target.draw(rect);
    }

    // auto mp = getMousePosition();
    // if(m_mode != NONE) {
    //     // debug
    //     sf::Vertex line[] = {
    //         sf::Vertex(sf::Vector2f(mp.x, mp.y), sf::Color(0, 255, 0)),
    //         sf::Vertex(sf::Vector2f(m_modeStartPosition.x, m_modeStartPosition.y), sf::Color(255, 255, 0))
    //     };
    //     target.draw(line, 2, sf::Lines);
    // }


    if(m_mode == FOLLOW) {
        // draw the number above each entity
        for(auto pair: m_entityNumbers) {
            auto p = pair.first->position();

            sf::Text text;
            text.setFont(* Root().resources.getFont("mono"));
            text.setPosition(p.x, p.y);
            text.setString(pair.second);
            text.setColor(sf::Color(0, 0, 0));
            text.setCharacterSize(13);
            text.scale(m_pixelSize, m_pixelSize);

            int b = 2;
            auto bounds = text.getLocalBounds();
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(bounds.width + 2 * b, bounds.height + 2 * b));
            shape.setPosition(sf::Vector2f((bounds.left-b) * m_pixelSize + p.x, (bounds.top-b) * m_pixelSize + p.y));
            shape.scale(m_pixelSize, m_pixelSize);
            shape.setFillColor(sf::Color(255, 200, 50, 200));
            shape.setOutlineColor(sf::Color(255, 200, 50));
            shape.setOutlineThickness(1);

            target.draw(shape);
            target.draw(text);
        }
    }

    // reset the view
    target.setView(target.getDefaultView());

    // TODO: Replace by experimental threadlet
    if(m_statusTime < 2) {
        sf::Text text;
        text.setFont(* Root().resources.getFont("mono"));
        text.setPosition(5, Root().window->getSize().y - text.getFont()->getLineSpacing(14) - 5);
        text.setString(m_statusText);
        text.setColor(sf::Color(255, 255, 255, m_statusTime == 0 ? 255 : 100));
        text.setCharacterSize(14);

        sf::RectangleShape cmdbox;
        cmdbox.setSize(sf::Vector2f(Root().window->getSize().x, 20));
        cmdbox.setPosition(0, Root().window->getSize().y - cmdbox.getSize().y);
        cmdbox.setFillColor(sf::Color(0, 0, 0, text.getColor().a));

        target.draw(cmdbox);
        target.draw(text);
    }

    setView(target);
}

void EditorState::startMode(EditorMode mode) {
    // do not start an entity mode if no entity selected
    if(!m_currentEntity && (mode == GRAB || mode == ROTATE || mode == SCALE)) {
        return;
    }

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
    } else if(m_mode == INSERT) {
        m_currentEntity = createNewEntity(m_insertModeCurrentType);
        add(m_currentEntity);
    } else if(m_mode == FOLLOW) {
        m_followModeInput = "";

        // find visible entities
        std::vector<std::shared_ptr<Entity>> visible_entities;
        for(auto entity: m_entities) {
            sf::Vector2i p = Root().window->mapCoordsToPixel(sf::Vector2f(entity->position().x, entity->position().y));
            if(Root().window->getViewport(m_view).contains(p)) {
                visible_entities.push_back(entity);
            }
        }

        // initialize the entity numbers
        m_entityNumbers.clear();
        static const std::string FOLLOW_CHARS = "ASDFGHJKL"; // uppercase letters only
        int N = FOLLOW_CHARS.length();

        int len = std::max(1, (int)ceil(log(visible_entities.size()) / log(N)));
        int index = 0;
        for(auto entity: visible_entities) {
            std::string number = "";
            for(int i = 0; i < len; i++) {
                int p = (int)pow(N, i);
                int val = index % (p*N) - index % p;
                int num = val / p;
                number += FOLLOW_CHARS[num];
            }
            m_entityNumbers[entity] = number;
            index++;
        }

        for(auto entity: visible_entities) {
            const std::string& number = m_entityNumbers[entity];
            int matching_chars = 0;
            for(auto other: visible_entities) {
                if(other == entity) continue;

                const std::string& other_number = m_entityNumbers[other];

                for(int i = 0; i < len; ++i) {
                    if(number[i] == other_number[i]) {
                        if(matching_chars < i+1)
                            matching_chars = i+1;
                    } else {
                        break;
                    }
                }
            }
            m_entityNumbers[entity] = number.substr(0, matching_chars+1);
        }
    } else if(m_mode == SAVE || m_mode == LOAD) {
        m_typing = true;
        m_typingString = "";
    }
}

void EditorState::updateMode() {
    auto mp = getMousePosition();

    glm::vec2 entity_start, entity_mouse;
    float entity_start_length = 0, entity_mouse_length = 0;

    if(m_currentEntity) {
        entity_start = m_currentEntity->position() - m_modeStartPosition;
        entity_mouse = m_currentEntity->position() - mp;
        entity_start_length = glm::length(entity_start);
        entity_mouse_length = glm::length(entity_mouse);
    }

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

        float step = thor::toRadian(15.f);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            angle = round(angle/step)*step;
        }

        m_currentEntity->setRotation(m_modeStartValue.x + angle);
        setStatus("Rotate: " + std::to_string((int)thor::toDegree(angle)));
    } else if(m_mode == SCALE) {
        float scale = ((entity_mouse_length != 0) ? (entity_mouse_length / entity_start_length) : 0.f);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            scale = round(scale*10)/10.0;
        }
        m_currentEntity->setScale(m_modeStartValue * scale);
        setStatus("Scale: " + std::to_string(scale));
    } else if(m_mode == INSERT) {
        m_currentEntity->setPosition(mp);
        m_currentEntity->setZLevel(m_currentZLevel);
        setStatus("Select type: (1) Wall (2) Pair");
    } else if(m_mode == FOLLOW) {
        setStatus("Follow: " + m_followModeInput);
    } else if(m_mode == SAVE) {
        setStatus("Type filename to save: " + m_typingString);
    } else if(m_mode == LOAD) {
        setStatus("Type filename to load: " + m_typingString);
    }
}

void EditorState::commitMode() {
    if(m_mode == SAVE) {
        std::string filename = "levels/" + m_typingString;
        saveToFile(filename);
        setStatus("Saved to " + filename + ".");
    } else if(m_mode == LOAD) {
        std::string filename = "levels/" + m_typingString;
        loadFromFile(filename);
        m_currentEntity.reset();
        setStatus("Loaded from " + filename + ".");
    }

    m_mode = NONE;
}

void EditorState::cancelMode() {
    if(m_currentEntity) {
        if(m_mode == GRAB) {
            m_currentEntity->setPosition(m_modeStartValue);
        } else if(m_mode == ROTATE) {
            m_currentEntity->setRotation(m_modeStartValue.x);
        } else if(m_mode == SCALE) {
            m_currentEntity->setScale(m_modeStartValue);
        } else if(m_mode == INSERT) {
            remove(m_currentEntity);
            m_currentEntity.reset();
        }
    }

    m_mode = NONE;
    setStatus("Canceled.");
}

void EditorState::setStatus(const std::string& text) {
    m_statusText = text;
    m_statusTime = 0.f;
}

std::shared_ptr<Entity> EditorState::createNewEntity(EditorState::EntityType type) const {
    if(type == WALL) {
        return std::make_shared<Wall>();
    } else if(type == PAIR) {
        return std::make_shared<Pair>();
    }

    return nullptr;
}
