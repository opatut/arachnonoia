#ifndef EDITORSTATE_HPP
#define EDITORSTATE_HPP

#include <memory>

#include "State.hpp"
#include "Player.hpp"

class EditorState : public State {
public:
    enum EditorMode {
        NONE,
        GRAB,
        ROTATE,
        SCALE
    };

    EditorState();
    void init();

    void onHandleEvent(sf::Event& event);
    void onUpdate(double dt);
    void onDraw(sf::RenderTarget& target);

    void startMode(EditorMode mode);
    void updateMode();
    void commitMode();
    void cancelMode();

private:
    std::shared_ptr<Entity> m_currentEntity;

    EditorMode m_mode;
    glm::vec2 m_modeStartPosition;
    glm::vec2 m_modeStartValue;

};

#endif
