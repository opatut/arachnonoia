#include "Root.hpp"

ResourceManager Root::resources;
GameState Root::game_state;
EditorState Root::editor_state;
MenuState Root::menu_state;
std::stack<State*> Root::states;
sf::RenderWindow* Root::window;
