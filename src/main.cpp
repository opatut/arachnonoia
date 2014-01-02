#include <iostream>
#include <stack>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

#include "Root.hpp"
#include "GameState.hpp"
#include "Level.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arachnonoia");
    sf::Clock clock;

    Root::instance().resources.addTexture("player", "data/player.png");

    std::stack<std::shared_ptr<State>> states;
    states.push(std::make_shared<GameState>());

    Level level;
    level.width = 10;
    level.height = 20;

    cereal::JSONOutputArchive ar(std::cout);
    ar(cereal::make_nvp("level", level));

    while(window.isOpen()) {
        double dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            states.top()->handleEvent(event);
        }

        // update
        states.top()->update(dt);

        // render
        window.clear(sf::Color(171, 171, 161));
        states.top()->draw(window);
        window.display();
    }

    return 0;
}
