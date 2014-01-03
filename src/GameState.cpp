#include "GameState.hpp"

#include <iostream>

GameState::GameState() {}

void GameState::init() {
    m_player = std::make_shared<Player>();
    add(m_player);
}

void GameState::onUpdate(double dt) {
    m_zoom = 6;
    m_center = m_player->m_position;
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
