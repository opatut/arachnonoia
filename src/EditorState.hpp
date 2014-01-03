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

    void onInit() override;
    void onHandleEvent(sf::Event& event) override;
    void onUpdate(double dt) override;
    void onDraw(sf::RenderTarget& target) override;

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
