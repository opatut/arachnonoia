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
#include "Pair.hpp"

bool isFullscreen = false;
sf::VideoMode defaultMode(1200, 900);
sf::VideoMode fullscreenMode = sf::VideoMode::getDesktopMode();

void createWindow() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    if(isFullscreen) {
        Root().window->create(fullscreenMode, "Arachnonoia", sf::Style::Fullscreen, settings);
    } else {
        Root().window->create(defaultMode, "Arachnonoia", sf::Style::Default, settings);
    }
    Root().window->setVerticalSyncEnabled(false);
    Root().window->setFramerateLimit(0);
}

int main() {
    if(!sf::Shader::isAvailable()) {
        std::cerr << "Sorry, your system does not support shaders. Please upgrade your video driver, enable your graphics card, or use a different device." << std::endl;
        exit(1);
    }
    
    Root().window = new sf::RenderWindow();
    createWindow();

    sf::RenderWindow& window = *Root().window;

    sf::Clock clock;

    Root().resources.addTexture("player", "data/player.png");
    Root().resources.addTexture("pair", "data/pair.png");
    Root().resources.addTexture("wall-box", "data/box.png");
    Root().resources.addTexture("wall-platform-1", "data/platform-1.png");
    Root().resources.addTexture("wall-platform-2", "data/platform-2.png");
    Root().resources.addTexture("wall-gradient", "data/gradient.png");
    Root().resources.addTexture("spiderweb", "data/spiderweb.png");
    Root().resources.addTexture("blob", "data/blob.png");
    Root().resources.addTexture("cave-1", "data/cave-1.jpg");
    Root().resources.addTexture("perlin", "data/perlin.png");
    // Root().resources.addFont("default", "data/OpenSans-Regular.ttf");
    Root().resources.addFont("title", "data/Supernova.ttf");
    Root().resources.addFont("default", "data/what-fish-died.ttf");
    Root().resources.addFont("mono", "data/UbuntuMono-R.ttf");
    Root().resources.addShader("pixel", "data/pixel.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("backdrop", "data/backdrop.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("fog", "data/fog.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("blur-horizontal", "data/blur-horizontal.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("blur-vertical", "data/blur-vertical.fragment.glsl", sf::Shader::Fragment);

    // Initialize all the states
    Root().editor_state.init();
    Root().game_state.init();
    Root().menu_state.init();

    // Setup game stack
    std::stack<State*> states;
    states.push(&Root().menu_state);
    states.push(&Root().game_state);

    while(window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::Escape) {
                    // window.close();
                } else if(event.key.code == sf::Keyboard::Tab) {
                    if(states.top() == &Root().game_state) {
                        states.push(&Root().editor_state);
                    } else {
                        states.pop();
                    }
                } else if(event.key.code == sf::Keyboard::F12) {
                    isFullscreen = !isFullscreen;
                    createWindow();
                }
            }

            states.top()->handleEvent(event);
        }

        // update
        states.top()->update(dt);

        // render
        window.clear();
        states.top()->draw(window);
        window.display();
    }

    return 0;
}
