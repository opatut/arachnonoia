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
    void onHandleEvent(sf::Event& event);

private:
    std::shared_ptr<Entity> m_currentEntity;

};

#endif
