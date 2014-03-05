#ifndef ROOT_HPP
#define ROOT_HPP

#include "ResourceManager.hpp"
#include "GameState.hpp"
#include "EditorState.hpp"
#include "MenuState.hpp"
#include <stack>

#include <SFML/Graphics/RenderWindow.hpp>

class Root {
public:
    // objects
    static ResourceManager resources;
    static GameState game_state;
    static EditorState editor_state;
    static MenuState menu_state;
    static std::stack<State*> states;

    static sf::RenderWindow* window;

    static bool debug;
};

#endif
