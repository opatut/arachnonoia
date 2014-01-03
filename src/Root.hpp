#ifndef ROOT_HPP
#define ROOT_HPP

#include "ResourceManager.hpp"
#include "GameState.hpp"
#include "EditorState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Root {
public:
    // objects
    static ResourceManager resources;
    static GameState game_state;
    static EditorState editor_state;

    static sf::RenderWindow* window;
};

#endif
