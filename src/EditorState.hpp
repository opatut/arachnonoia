#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class EditorState : public State {
public:
    EditorState();
    void init();

    void onUpdate(double dt);
    void onDraw(sf::RenderTarget& target);

};

#endif
