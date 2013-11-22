#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Root.hpp"
#include "Stack.hpp"
#include "GameState.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arachnonoia");
    sf::Clock clock;

    Root::instance().resources.addTexture("player", "data/player.png");

    Stack stack;
    stack.push(std::make_shared<GameState>());

    while (window.isOpen()) {
        double dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // update
        stack.current()->update(dt);

        // render
        window.clear();
        stack.current()->draw(window);
        window.display();
    }

    return 0;
}
