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

    Root().resources.addTexture("player",           "data/textures/player.png");
    Root().resources.addTexture("pair",             "data/textures/pair.png");
    Root().resources.addTexture("wall-box",         "data/textures/box.png");
    Root().resources.addTexture("wall-platform-1",  "data/textures/platform-1.png");
    Root().resources.addTexture("wall-platform-2",  "data/textures/platform-2.png");
    Root().resources.addTexture("wall-gradient",    "data/textures/gradient.png");
    Root().resources.addTexture("spiderweb",        "data/textures/spiderweb.png");
    Root().resources.addTexture("blob",             "data/textures/blob.png");
    Root().resources.addTexture("cave-1",           "data/textures/cave-1.jpg");
    Root().resources.addTexture("perlin",           "data/textures/perlin.png");
    Root().resources.addTexture("egg",              "data/textures/egg.png");
    Root().resources.addTexture("egg-top",          "data/textures/egg-top.png");
    Root().resources.addTexture("egg-bottom",       "data/textures/egg-bottom.png");
    Root().resources.addTexture("egg-crack",        "data/textures/egg-crack.png");

    Root().resources.addSound("crack",  "data/sounds/crack.ogg");
    Root().resources.addSound("rumble", "data/sounds/rumble.ogg");
    Root().resources.addSound("walk",   "data/sounds/walk.ogg");

    Root().resources.addMusic("horror-ambience", "data/music/horror-ambience.wav");

    Root().resources.addFont("title",   "data/fonts/Supernova.ttf");
    Root().resources.addFont("default", "data/fonts/what-fish-died.ttf");
    Root().resources.addFont("mono",    "data/fonts/UbuntuMono-R.ttf");

    Root().resources.addShader("pixel",             "data/shaders/pixel.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("backdrop",          "data/shaders/backdrop.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("fog",               "data/shaders/fog.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("blur-horizontal",   "data/shaders/blur-horizontal.fragment.glsl", sf::Shader::Fragment);
    Root().resources.addShader("blur-vertical",     "data/shaders/blur-vertical.fragment.glsl", sf::Shader::Fragment);

    // Initialize all the states
    Root().editor_state.init();
    Root().game_state.init();
    Root().menu_state.init();

    // Setup game stack
    std::stack<State*> states;
    states.push(&Root().menu_state);
    states.push(&Root().game_state);

    sf::Sound s;
    s.setBuffer(*Root().resources.getSound("rumble"));
    s.setLoop(true);
    s.setVolume(10);
    s.play();

    auto m = Root().resources.getMusic("horror-ambience");
    m->setVolume(10);
    m->play();

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
