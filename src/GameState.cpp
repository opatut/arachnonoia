#include "GameState.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include "Wall.hpp"
#include "Pair.hpp"

GameState::GameState() {
    m_zoom = 12;
    m_debugDrawEnabled = true;
}

void GameState::onInit() {
    loadFromFile("levels/debug.json");

    m_player = std::make_shared<Player>();
    add(m_player);
}

void GameState::onUpdate(double dt) {
    // m_zoom = 6;
    float targetZoom = 6 + m_player->physicsBody()->getLinearVelocity().length();
    float zoomSpeed = 1;
    m_zoom = m_zoom * (1 - dt * zoomSpeed) + targetZoom * (dt * zoomSpeed);
    m_center = m_player->position();
}

void GameState::onDraw(sf::RenderTarget& target) {
    setView(target);

    // draw some background stuff
    for(int x = -10; x < 10; ++x) {
        for(int y = -10 + (x%2); y < 10; y+=2) {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(1, 1));
            rect.setFillColor(sf::Color(100, 100, 100));
            rect.setPosition(x, y);
            target.draw(rect);
        }
    }

    drawEntities(target);
}

void GameState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Period) {
            m_debugDrawEnabled = !m_debugDrawEnabled;
        }
    }
}
