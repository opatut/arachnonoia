#include "GameState.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include "Wall.hpp"
#include "Pair.hpp"
#include "Root.hpp"
#include "Marker.hpp"
#include "Toy.hpp"
#include "CollisionShape.hpp"

GameState::GameState() {
    m_zoom = 6;
    m_debugDrawEnabled = false;
}

void GameState::onInit() {
    resize();
    loadLevel(1);
}

void GameState::onUpdate(float dt) {
    if(m_renderTexture.getSize() != Root().window->getSize()) {
        resize();
    }

    // m_zoom = 6;
    if(m_player) {
        float targetZoom = 6 + m_player->physicsBody()->getLinearVelocity().length();
        float zoomSpeed = 1;
        m_zoom = m_zoom * (1 - dt * zoomSpeed) + targetZoom * (dt * zoomSpeed);
        m_center = m_player->position();
    }
}

void GameState::onDraw(sf::RenderTarget& target) {
    auto& t = m_renderTexture;

    target.clear();
    t.clear(sf::Color(80, 80, 80));

    // backdrop
    t.setView(t.getDefaultView());
    sf::RectangleShape backdrop(sf::Vector2f(t.getSize()));
    backdrop.setFillColor(sf::Color(100, 150, 255));
    backdrop.setFillColor(sf::Color(40, 40, 40));
    auto shader = Root().resources.getShader("backdrop");
    shader->setParameter("size", sf::Vector2f(t.getSize()));
    shader->setParameter("time", getTime());
    t.draw(backdrop, shader.get());

    // draw
    setView(t);
    drawEntities(t);

    // post-processing
    float w = target.getSize().x;
    float h = target.getSize().y;
    target.setView(sf::View(sf::FloatRect(0, h, w, -h)));
 
    sf::Sprite sprite;
    sprite.setTexture(m_renderTexture.getTexture());
    target.draw(sprite, Root().resources.getShader("pixel").get());

}

void GameState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Period) {
            m_debugDrawEnabled = !m_debugDrawEnabled;
        } else if(event.key.code == sf::Keyboard::Escape) {
            Root().window->close();
        }
    } else if(event.type == sf::Event::Resized) {
        resize();
    }    
}

void GameState::resize() {
    m_renderTexture.create(Root().window->getSize().x, Root().window->getSize().y);
}


void GameState::loadLevel(int num) {
    std::string filename = "level" + std::to_string(num) + ".dat";
    loadFromFile("levels/" + filename);

    m_player = nullptr;
    for(auto marker : getEntitiesByType<Marker>("Marker")) {
        if(marker->getType() == Marker::SPAWN) {
            if(!m_player) {
                spawnPlayer(marker->position());
            } else {
                std::cout << "Warning: level " << filename << " contains multiple SPAWN markers. Using first one." << std::endl;
            }
        }
    }

    if(!m_player) {
        std::cout << "Warning: level " << filename << " does not contain any spawn marker. Spawning at (0, 0)." << std::endl;
        spawnPlayer(glm::vec2(0, 0));
    }
}

void GameState::spawnPlayer(const glm::vec2& pos) {
    m_player = std::make_shared<Player>();
    add(m_player);
    m_player->setPhysicsPosition(pos);

    for(int i = 0; i < 5; ++i) {
        auto toy = std::make_shared<Toy>();
        add(toy);
        toy->setPhysicsPosition(pos + glm::vec2(i + 1, 0));
        toy->setScale(glm::vec2(0.2 + i * 0.1, 0.2 + i * 0.1));
    }
}
