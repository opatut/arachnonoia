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
        SCALE,
        FOLLOW,
        SAVE,
        LOAD
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

protected:
    void setStatus(const std::string& text);

private:
    std::shared_ptr<Entity> m_currentEntity;
    std::map<std::shared_ptr<Entity>, std::string> m_entityNumbers;

    EditorMode m_mode;
    bool m_typing;
    std::string m_typingString;
    glm::vec2 m_modeStartPosition;
    glm::vec2 m_modeStartValue;

    std::string m_followModeInput;

    std::string m_statusText;
    float m_statusTime;

};

#endif
