#include "GameState.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include "Wall.hpp"
#include "Pair.hpp"
#include "Root.hpp"

GameState::GameState() {
    m_zoom = 12;
    m_debugDrawEnabled = true;
}

void GameState::onInit() {
    resize();
    loadFromFile("levels/debug.json");

    m_player = std::make_shared<Player>();
    add(m_player);
}

void GameState::onUpdate(float dt) {
    if(m_renderTexture.getSize() != Root().window->getSize()) {
        resize();
    }

    // m_zoom = 6;
    float targetZoom = 6 + m_player->physicsBody()->getLinearVelocity().length();
    float zoomSpeed = 1;
    m_zoom = m_zoom * (1 - dt * zoomSpeed) + targetZoom * (dt * zoomSpeed);
    m_center = m_player->position();
}

void GameState::onDraw(sf::RenderTarget& target) {
    m_renderTexture.clear(sf::Color(80, 80, 80));
    setView(m_renderTexture);

    // draw some background stuff
    for(int x = -10; x < 10; ++x) {
        for(int y = -10 + (x%2); y < 10; y+=2) {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(1, 1));
            rect.setFillColor(sf::Color(100, 100, 100));
            rect.setPosition(x, y);
            m_renderTexture.draw(rect);
        }
    }

    drawEntities(m_renderTexture);

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
        }
    } else if(event.type == sf::Event::Resized) {
        resize();
    }    
}

void GameState::resize() {
    m_renderTexture.create(Root().window->getSize().x, Root().window->getSize().y);
}
