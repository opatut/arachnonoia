#include "GameState.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include "Wall.hpp"
#include "Pair.hpp"
#include "Root.hpp"
#include "CollisionShape.hpp"

GameState::GameState() {
    m_zoom = 12;
    m_debugDrawEnabled = true;
}

void GameState::onInit() {
    resize();
    loadFromFile("levels/level1.dat");

    m_player = std::make_shared<Player>();
    add(m_player);
    m_player->setPhysicsPosition(glm::vec2(-4, 4));
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
    auto& t = m_renderTexture;

    target.clear();
    t.clear(sf::Color(80, 80, 80));

    // backdrop
    t.setView(t.getDefaultView());
    sf::RectangleShape backdrop(sf::Vector2f(t.getSize()));
    backdrop.setFillColor(sf::Color(100, 150, 255));
    backdrop.setFillColor(sf::Color(100, 80, 20));
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
