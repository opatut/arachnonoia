#include "GameState.hpp"

#include <iostream>
#include <functional>

#include <Thor/Math.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtx/vector_angle.hpp>

#include "Wall.hpp"
#include "Pair.hpp"
#include "Root.hpp"
#include "Marker.hpp"
#include "Toy.hpp"
#include "CollisionShape.hpp"

GameState::GameState() {
    m_zoom = 6;
    m_debugDrawEnabled = false;

    m_currentHelp = "";

    m_levels.push_back(std::make_pair("spawn", Player::WALK));
    m_levels.push_back(std::make_pair("pairs", Player::WALK));
    m_levels.push_back(std::make_pair("jump-1", Player::JUMP));
    m_levels.push_back(std::make_pair("jump-2", Player::JUMP));
    m_levels.push_back(std::make_pair("walls", Player::WALLS));
    m_levels.push_back(std::make_pair("upside-down", Player::WALLS));

    // help points
    m_levelHelp["spawn"] = "walk";
    m_levelHelp["jump-1"] = "jump";
    m_levelHelp["walls"] = "walls";
}

void GameState::onInit() {
    resize();

    loadLevel(0);

    m_rumbleSound.setBuffer(*Root().resources.getSound("rumble"));
    m_rumbleSound.setLoop(true);
    m_rumbleSound.setVolume(10);
    m_rumbleSound.play();

    m_music = Root().resources.getMusic("horror-ambience");
    m_music->setVolume(10);
    m_music->play();
}

void GameState::onUpdate(float dt) {
    if(m_renderTextures[0].getSize() != Root().window->getSize()) {
        resize();
    }

    // m_zoom = 6;
    if(m_player) {
        float targetZoom = 6;// + m_player->physicsBody()->getLinearVelocity().length();
        float zoomSpeed = 2;
        m_zoom = m_zoom * (1 - dt * zoomSpeed) + targetZoom * (dt * zoomSpeed);

        glm::vec2 target = m_player->position() - glm::vec2(0, 0.3);

        glm::vec2 d(0.5, 0.5);
        glm::vec2 diff = target - m_center;
        diff.x = diff.x < -d.x ? -d.x : (diff.x > d.x ? d.x : diff.x);
        diff.y = diff.y < -d.y ? -d.y : (diff.y > d.y ? d.y : diff.y);
        auto new_center = target - diff;
        zoomSpeed = 8;
        m_center = m_center * (1 - dt * zoomSpeed) + new_center * (dt * zoomSpeed);

        if(m_helpProgress < 1.f) {
            if(m_currentHelp == "") {
                // check marker distance
                auto trigger = getMarker(Marker::HELP_TRIGGER);
                if(trigger) {
                    float distance = glm::length(trigger->position() - m_player->position());
                    if(distance < 2.f) { // trigger distance
                        m_currentHelp = m_levelHelp[m_currentLevelName];
                    }
                }
            } else {
                m_helpProgress = fmin(1.f, m_helpProgress + dt / 5.f);
            }
        }
    }

    if(m_message != "") {
        m_messageTime += dt;
        if(m_messageTime > 5) {
            m_message = "";
        }
    }
}

void GameState::onDraw(sf::RenderTarget& target) {
    float w = target.getSize().x;
    float h = target.getSize().y;
    m_renderTextures[0].clear();
    m_renderTextures[1].clear();

    sf::RenderTarget& t = m_renderTextures[0];

    target.clear();
    t.clear(sf::Color(80, 80, 80));

    // backdrop
    // t.setView(t.getDefaultView());
    // sf::RectangleShape backdrop(sf::Vector2f(t.getSize()));
    // backdrop.setFillColor(sf::Color(100, 150, 255));
    // backdrop.setFillColor(sf::Color(40, 40, 40));
    // auto shader = Root().resources.getShader("backdrop");
    // shader->setParameter("size", sf::Vector2f(t.getSize()));
    // shader->setParameter("time", getTime());
    // t.draw(backdrop, shader.get());

    setView(t);
    int backTiles = 50;
    float s = 4.0;

    auto tex = Root().resources.getTexture("cave-1");
    tex->setRepeated(true);
    sf::Sprite back(*tex.get());
    back.setTextureRect(sf::IntRect(0, 0, tex->getSize().x * backTiles, tex->getSize().y * backTiles));
    back.setScale(s / tex->getSize().x, s / tex->getSize().y);
    back.setPosition(m_center.x * 0.2, m_center.y * 0.2);
    back.setOrigin(tex->getSize().x / 2 * backTiles, tex->getSize().y / 2 * backTiles);

    auto levelColor = {
        sf::Color(100, 20, 0),
        sf::Color(100, 120, 200),
        sf::Color(250, 200, 0),
        sf::Color(255, 0, 128)
    };
    back.setColor(*(levelColor.begin() + (m_currentLevel) % levelColor.size()));
    t.draw(back);

    s = 8.0;
    tex = Root().resources.getTexture("perlin");
    tex->setRepeated(true);
    back.setTexture(*tex.get(), false);
    back.setTextureRect(sf::IntRect(0, 0, tex->getSize().x * backTiles, tex->getSize().y * backTiles));
    back.setScale(s / tex->getSize().x, s / tex->getSize().y);
    back.setOrigin(tex->getSize().x / 2 * backTiles, tex->getSize().y / 2 * backTiles);
    back.setColor(sf::Color(128, 128, 128));
    back.setPosition(m_center.x * 0.1, m_center.y * 0.1);
    t.draw(back, sf::BlendMultiply);

    s = 8.0;
    back.setScale(s / tex->getSize().x, s / tex->getSize().y);
    back.setPosition(0, 0);
    back.setColor(sf::Color(255, 255, 255, 80));
    back.setPosition(-m_center.x * 0.2, -m_center.y * 0.2);
    t.draw(back, sf::BlendAdd);

    s = 8.0;
    back.setScale(s / tex->getSize().x, s / tex->getSize().y);
    back.setPosition(-m_center.x * 0.3, -m_center.y * 0.3);
    back.setColor(sf::Color(255, 255, 255, 255));
    t.draw(back, sf::BlendMultiply);

    // draw
    setView(t);
    drawEntities(t);

    setView(m_renderTextures[1]);
    float f = - 0.2;
    back.setColor(sf::Color(255, 255, 255, 150));
    back.setPosition(m_center.x * f + m_time * f * 1.5, m_center.y * f);
    m_renderTextures[1].draw(back, sf::BlendAdd);

    sf::Sprite sprite;
    sprite = sf::Sprite(m_renderTextures[1].getTexture());
    Root().resources.getShader("fog")->setParameter("size", sf::Vector2f(m_renderTextures[1].getSize()));
    t.setView(sf::View(sf::FloatRect(0, h, w, -h)));
    t.draw(sprite, sf::RenderStates(sf::BlendAdd, sf::RenderStates::Default.transform, sf::RenderStates::Default.texture, Root().resources.getShader("fog").get()));

    // post-processing
    target.setView(sf::View(sf::FloatRect(0, h, w, -h)));
 
    auto pixel          = Root().resources.getShader("pixel");
    auto verticalBlur   = Root().resources.getShader("blur-vertical");
    auto horizontalBlur = Root().resources.getShader("blur-horizontal");

    horizontalBlur->setParameter("blurSize", 2.5 / w);
    verticalBlur->setParameter("blurSize", 2.5 / h);
    // pixel->setParameter("center", m_center.x, m_center.y);
    pixel->setParameter("size", w, h);

    m_renderTextures[0].setView(m_renderTextures[0].getDefaultView());
    m_renderTextures[1].setView(m_renderTextures[1].getDefaultView());
    m_renderTextures[0].setSmooth(true);
    m_renderTextures[1].setSmooth(true);

    if(!m_debugDrawEnabled) {
        sprite = sf::Sprite(m_renderTextures[0].getTexture());
        m_renderTextures[1].draw(sprite, horizontalBlur.get());

        sprite = sf::Sprite(m_renderTextures[1].getTexture());
        m_renderTextures[0].draw(sprite, verticalBlur.get());

        sprite = sf::Sprite(m_renderTextures[0].getTexture());
        target.draw(sprite, pixel.get());
    } else {
        sprite = sf::Sprite(m_renderTextures[0].getTexture());
        target.draw(sprite);
    }

    // help
    setView(target);
    if(m_currentHelp != "" && m_helpProgress > 0 && m_helpProgress < 1) {
        float fade = glm::smoothstep(0.f, 0.1f, m_helpProgress) - glm::smoothstep(0.9f, 1.f, m_helpProgress);
        float wobble = sin(m_time * 5);

        float alpha = fade;
        float angle = tween::Cubic().easeIn(1 - fade, 0, 1, 1) * 0.2;
        float scale = (0.6 + 0.01 * wobble) * m_pixelSize;

        std::string texture = "help-" + m_currentHelp;
        auto tex = Root().resources.getTexture(texture);
        if(tex) {
            sf::Sprite sprite(*tex.get());
            glm::vec2 ang(-1, 0);
            glm::vec2 pos = m_player->position() - glm::vec2(0, 1.1f) + ang - glm::rotate(ang, angle);
            sprite.setPosition(pos.x, pos.y);
            sprite.setOrigin(tex->getSize().x / 2, tex->getSize().y / 2);
            sprite.setColor(sf::Color(255, 255, 255, 255 * alpha));
            sprite.setScale(scale, scale);
            sprite.setRotation(wobble + thor::toDegree(angle));
            target.draw(sprite);
        }
    }

    // message
    target.setView(target.getDefaultView());
    if(m_message != "") {
        float alpha = fmin(1, fmax(0, m_messageTime)) * fmin(1, fmax(0, 4 - m_messageTime));
        alpha = tween::Cubic().easeOut(alpha, 0, 1, 1);

        sf::Text text;
        text.setFont(* Root().resources.getFont("default"));
        text.setCharacterSize(36);
        text.setString(m_message);
        text.setStyle(sf::Text::Bold);
        text.setPosition(sf::Vector2f(target.getSize().x / 2 - text.getLocalBounds().width / 2, target.getSize().y * 0.8 - fabs(sin(m_time)) * 20));
        text.setColor(sf::Color(255, 255, 255, 255 * alpha));

        sf::Vector2f b(10, 5);
        sf::RectangleShape rect(sf::Vector2f(text.getLocalBounds().width + 2 * b.x, text.getLocalBounds().height * 1.5 + 2 * b.y));
        rect.setPosition(text.getPosition() - b);
        rect.setFillColor(sf::Color(0, 0, 0, 100 * alpha));

        target.draw(rect);
        target.draw(text);
    }

    if(Root().debug) {
        sf::Text text;
        text.setFont(* Root().resources.getFont("mono"));
        text.setCharacterSize(20);
        text.setString(std::to_string(getFPS()) + " FPS");
        text.setPosition(sf::Vector2f(10, 10));
        text.setColor(sf::Color(255, 255, 255, 100));
        target.draw(text);
    }

    if(m_levelFade) {
        sf::RectangleShape rect(sf::Vector2f(target.getSize()));
        rect.setFillColor(sf::Color(0, 0, 0, 255 * m_levelFade));
        target.draw(rect);
    }
}

void GameState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        if(Root().debug) {
            if(event.key.code == sf::Keyboard::Period) {
                m_debugDrawEnabled = !m_debugDrawEnabled;
                message("Debug draws toggled");
            } else if(event.key.code == sf::Keyboard::Q) {
                m_player->setAbility((Player::Ability)(((int)m_player->getAbility() + 1) % ((int)Player::RAPPEL + 1)));
                message("Ability: " + std::to_string(m_player->getAbility()));
            } else if(event.key.code == sf::Keyboard::Escape) {
                Root().window->close();
            } else if(event.key.code == sf::Keyboard::Add) {
                switchLevel(m_currentLevel + 1);
            } else if(event.key.code == sf::Keyboard::Subtract) {
                switchLevel(m_currentLevel - 1);
            } else if(event.key.code == sf::Keyboard::H) {
                m_currentHelp = m_levelHelp[m_currentLevelName];
            } else if(event.key.code == sf::Keyboard::Tab) {
                Root().states.push(&Root().editor_state);
                if(m_player) m_player->m_walkSound.pause();
            }
        } else {
            if(event.key.code == sf::Keyboard::Escape) {
                Root().states.pop();
            }
        }
    } else if(event.type == sf::Event::Resized) {
        resize();
    }    
}

void GameState::resize() {
    m_renderTextures[0].create(Root().window->getSize().x, Root().window->getSize().y);
    m_renderTextures[1].create(Root().window->getSize().x, Root().window->getSize().y);
}


void GameState::loadLevel(int num) {
    if(Root().game_state.m_player) Root().game_state.m_player->m_walkSound.pause();
    if(num < 0 || num >= m_levels.size()) {
        Root().menu_state.setGameOver(num > 0);
        Root().states.pop();
        return;
    }

    m_currentLevel = num;
    m_currentLevelName = m_levels[m_currentLevel].first;
    m_helpProgress = 0.f;
    m_currentHelp = "";

    std::string filename = m_currentLevelName + ".dat";
    loadFromFile("levels/" + filename);

    // spawn something
    auto spawn = getMarker(Marker::SPAWN);
    m_player = nullptr;
    auto pos = glm::vec2(0, 0);
    if(spawn) {
        pos = spawn->position();
    } else {
        std::cout << "Warning: level " << filename << " does not contain any spawn marker. Spawning at (0, 0)." << std::endl;
    }

    if(m_currentLevelName == "spawn") {
        spawnEgg(pos);
    } else {
        spawnPlayer(pos);
    }

    m_levelFade = 1.f;
    tween::TweenerParam p2(1000, tween::SINE, tween::EASE_IN_OUT);
    p2.addProperty(&m_levelFade, 0.f);
    m_tweener.addTween(p2);

    if(Root().debug) {
        message(m_currentLevelName);
    }
}

void GameState::spawnPlayer(const glm::vec2& pos) {
    m_player = std::make_shared<Player>();
    add(m_player);
    m_player->setPhysicsPosition(pos);
    m_center = m_player->position();

    // set player abilities
    m_player->setAbility(m_levels[m_currentLevel].second);
}

void GameState::spawnEgg(const glm::vec2& pos) {
    m_egg = std::make_shared<Egg>();
    m_egg->setHatching(true);
    add(m_egg);
    m_egg->setPhysicsPosition(pos);
    m_egg->setPhysicsRotation(thor::Pi / 2);
    m_center = m_egg->position();

    for(int i = 0; i < 10; ++i) {
        auto egg = std::make_shared<Egg>();
        add(egg);
        egg->setPhysicsPosition(pos + glm::vec2(thor::random(-2.5f, -1.0f), thor::random(-0.5f, 0.f)));
        egg->setPhysicsRotation(thor::random(-thor::Pi, thor::Pi));
        float s = thor::random(0.5f, 0.7f);
        egg->setScale(glm::vec2(s, s));
        egg->handleUpdate(0);
    }

    m_dynamicsWorld->stepSimulation(10.f, 100);
}


void GameState::switchLevel(int num, bool reset) {
    if(m_nextLevel == num && !reset) return;

    m_nextLevel = num;

    if(reset) {
        loadLevel(num);
    } else {
        tween::TweenerParam p(1500, tween::SINE, tween::EASE_IN_OUT);
        m_levelFade = 0;
        p.addProperty(&m_levelFade, 1.f);
        p.onCompleteCallBack = []() {
            Root().game_state.loadLevel(Root().game_state.m_nextLevel);
        };
        m_tweener.addTween(p);
    }
}

void GameState::nextLevel() {
    switchLevel(m_currentLevel + 1);
}

void GameState::message(const std::string& msg) {
    if(m_message == msg) return;

    m_message = msg;
    m_messageTime = 0.f;
}

std::shared_ptr<Marker> GameState::getMarker(Marker::Type type) {
    std::shared_ptr<Marker> result = nullptr;

    for(auto marker : getEntitiesByType<Marker>("Marker")) {
        if(marker->getType() == type) {
            if(result) {
                std::cerr << "Warning: multiple markers of type " << type << " found in level " << m_currentLevelName << "." << std::endl;
            } else { 
                result = marker;
            }
        }
    }
    return result;
}
