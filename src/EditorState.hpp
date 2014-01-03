#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class EditorState : public State {
public:
    EditorState();

    void onInit() override;
    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;
    void onHandleEvent(sf::Event& event) override;

private:
    std::shared_ptr<Entity> m_currentEntity;

};

#endif
