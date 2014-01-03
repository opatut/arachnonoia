#include "GameState.hpp"

#include <iostream>

GameState::GameState() {}

void GameState::init() {
    m_player = std::make_shared<Player>();
    add(m_player);
}

void GameState::onUpdate(double dt) {

}

void GameState::onDraw(sf::RenderTarget& target) {
    sf::View view;
    float zoom = 6;
    float w = zoom;
    float h = w / target.getSize().x * target.getSize().y;
    view.reset(sf::FloatRect(m_player->m_position.x-w/2, m_player->m_position.y-h/2, w, h));
    //view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));
    target.setView(view);

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
}
