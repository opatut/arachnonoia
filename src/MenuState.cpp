#include "MenuState.hpp"

#include "Root.hpp"

MenuState::MenuState() {}

void MenuState::onUpdate(float dt) {}

void MenuState::onDraw(sf::RenderTarget &target) {
    target.clear();
    target.setView(target.getDefaultView());

    sf::Text t;
    t.setFont(* Root().resources.getFont("default"));
    t.setCharacterSize(80);
    t.setString("Arachnonoia");
    t.setPosition(sf::Vector2f(100, 100));
    t.setColor(sf::Color::White);
    target.draw(t);
}
