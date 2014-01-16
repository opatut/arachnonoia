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
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arachnonoia", sf::Style::Default, settings);

    sf::Clock clock;

    Root().window = &window;

    Root().resources.addTexture("player", "data/player.png");
    Root().resources.addTexture("wall-box", "data/box.png");
    Root().resources.addFont("default", "data/OpenSans-Regular.ttf");
    Root().resources.addFont("mono", "data/UbuntuMono-R.ttf");

    // Initialize all the states
    Root().editor_state.init();
    Root().game_state.init();

    // Setup game stack
    std::stack<State*> states;
    states.push(&Root().game_state);
//    states.push(&Root().editor_state);

    // Level level;
    // level.width = 10;
    // level.height = 20;

    // cereal::JSONOutputArchive ar(std::cout);
    // ar(cereal::make_nvp("level", level));

    while(window.isOpen()) {
        double dt = clock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) {
                    // window.close();
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
