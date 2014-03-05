#include "MenuState.hpp"

#include "Root.hpp"

MenuState::MenuState() {}

void MenuState::onInit() {
    m_egg = std::make_shared<Egg>();
    add(m_egg);
    m_center = glm::vec2(0, 0);
    m_zoom = 3;
    m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

void MenuState::onUpdate(float dt) {
    if(m_renderTextures[0].getSize() != Root().window->getSize()) {
        resize();
    }

    float t = fmod(m_time, 2) / 2;
    float r = 0.f;

    if((t > 0.0 && t < 0.02) || (t > 0.06 && t < 0.08)) r = 0.03;
    else if((t > 0.02 && t < 0.04) || (t > 0.08 && t < 0.10)) r = -0.03;

    m_egg->setPhysicsRotation(r);
}

void MenuState::onDraw(sf::RenderTarget &target) {
    target.clear();
    m_renderTextures[0].clear();
    m_renderTextures[1].clear();

    auto& t = m_renderTextures[0];
    float w = target.getSize().x;
    float h = target.getSize().y;

    auto pixel = Root().resources.getShader("pixel");
    auto fog = Root().resources.getShader("fog");
    pixel->setParameter("size", w, h);
    fog->setParameter("size", w, h);

    setView(t);

    int backTiles = 20;
    auto s = 2.f;
    auto tex = Root().resources.getTexture("cave-1");
    tex->setRepeated(true);
    sf::Sprite back(*tex.get());
    back.setTextureRect(sf::IntRect(0, 0, tex->getSize().x * backTiles, tex->getSize().y * backTiles));
    back.setScale(s / tex->getSize().x, s / tex->getSize().y);
    back.setPosition(m_center.x * 0.2, m_center.y * 0.2);
    back.setOrigin(tex->getSize().x / 2 * backTiles, tex->getSize().y / 2 * backTiles);
    t.draw(back);

    m_renderTextures[1].setView(sf::View(sf::FloatRect(0, h, w, -h)));
    m_renderTextures[1].draw(sf::Sprite(m_renderTextures[0].getTexture()), fog.get());

    // draw
    setView(m_renderTextures[1]);
    drawEntities(m_renderTextures[1]);

    target.setView(sf::View(sf::FloatRect(0, h, w, -h)));
    target.draw(sf::Sprite(m_renderTextures[1].getTexture()), pixel.get());

    target.setView(target.getDefaultView());

    sf::Text text;
    text.setFont(* Root().resources.getFont("default"));
    text.setCharacterSize(80);
    text.setString(m_gameOver ? "Game over" : "Arachnonoia");
    text.setPosition(w / 2 - text.getLocalBounds().width / 2, 100);
    text.setColor(sf::Color::White);
    target.draw(text);

    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setString(std::string("Press any key to ") + (m_gameOver ? "play again" : "start the adventure"));
    text.setPosition(w / 2 - text.getLocalBounds().width / 2, 200);//target.getSize().y - 100);
    text.setColor(sf::Color(255, 255, 255, fabs(sin(m_time * 2)) * 128 + 127));
    target.draw(text);

    if(!m_gameOver) {
        text.setCharacterSize(24);
        text.setString("Oh, and this is you!");
        text.setStyle(sf::Text::Regular);
        text.setPosition(w / 2 - text.getLocalBounds().width / 2, h * 0.7);//target.getSize().y - 100);
        text.setColor(sf::Color::White);
        target.draw(text);
    }

    text.setCharacterSize(20);
    text.setString("Created by Sven-Hendrik 'svenstaro' Haase and Paul 'opatut' Bienkowski");
    text.setPosition(w / 2 - text.getLocalBounds().width / 2, h - 70);
    text.setColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    target.draw(text);
    text.setString("for \"Programmierung interaktiver Visualisierungen / Game Development\" - University of Hamburg - 2013/14");
    text.setStyle(sf::Text::Regular);
    text.setPosition(w / 2 - text.getLocalBounds().width / 2, h - 40);
    target.draw(text);
    
}

void MenuState::onHandleEvent(sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Escape) {
            Root().states.pop();
        } else {
            Root().game_state.switchLevel(0, true);
            Root().states.push(&Root().game_state);
        }
    } else if(event.type == sf::Event::Resized) {
        resize();
    }
}

void MenuState::resize() {
    m_renderTextures[0].create(Root().window->getSize().x, Root().window->getSize().y);
    m_renderTextures[1].create(Root().window->getSize().x, Root().window->getSize().y);
}

void MenuState::setGameOver(bool gameOver) {
    m_gameOver = gameOver;
}
