#include "Foot.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>
#include <Thor/Graphics.hpp>

#include "Player.hpp"
#include "Root.hpp"

Foot::Foot(Player* player, int offset, bool background) :
    m_player(player), m_offset(offset), m_background(background)
{}

std::string Foot::getTypeName() const {
    return "Foot";
}

void Foot::onUpdate(double dt) {
    int speed = (m_offset % 2 ? -1 : 1) * m_direction;
    float speedFactor = 16; // speed of leg movement
    m_phase += speed * speedFactor * dt;

    float ankleOffsetFactor = 0.2f; // distance between ankle
    float anklePhaseFactor = 0.1f; // distance of ankle move per phase

    float p = sin(m_phase);

    // Offset from main body
    sf::Vector2f offsetAnkle(p * anklePhaseFactor + (m_offset - 1.5) * ankleOffsetFactor,
                             -0.5f * m_player->m_scale_y * 4 - (m_background ? -0.05f : 0));
    sf::Vector2f relAnklePos = thor::rotatedVector(offsetAnkle, thor::toDegree(m_player->rotation()));
    m_anklePosition = glm::vec2(m_player->position().x - relAnklePos.x, m_player->position().y - relAnklePos.y);

    float legOffsetFactor = 0.5f; // distance between legs
    float legPhaseFactor = 0.2f; // distance of leg move per phase

    sf::Vector2f offsetFootRayEnd(p * legPhaseFactor + (m_offset - 1.5) * legOffsetFactor, 0.2f - p * 0.4);
    sf::Vector2f relFootRayEnd = thor::rotatedVector(offsetFootRayEnd, thor::toDegree(m_player->rotation()));
    sf::Vector2f absFootRayEnd(m_player->position().x - relFootRayEnd.x, m_player->position().y - relFootRayEnd.y);

    btVector3 rayStart(m_anklePosition.x, m_anklePosition.y, 0);
    btVector3 rayEnd(absFootRayEnd.x, absFootRayEnd.y, 0);

    class ClosestNonPlayerRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
    {
        public:
            explicit ClosestNonPlayerRayResultCallback(const btVector3 &rayFromWorld, const btVector3& rayToWorld) :
                ClosestRayResultCallback(rayFromWorld, rayToWorld)
            {}
            virtual ~ClosestNonPlayerRayResultCallback() {}

            virtual bool needsCollision(btBroadphaseProxy* proxy0) const {
                const btCollisionObject* obj = static_cast<const btCollisionObject*>(proxy0->m_clientObject);
                const Entity* ent = static_cast<const Entity*>(obj->getUserPointer());
                if(ent != 0 && ent->getTypeName() != "Player")
                    return true;
                else
                    return false;
            }
    };
    ClosestNonPlayerRayResultCallback rayCallback(rayStart, rayEnd);

    m_player->m_state->dynamicsWorld()->rayTest(rayStart, rayEnd, rayCallback);

    btVector3 hitPoint(0, 0, 0);
    if(rayCallback.hasHit()) {
        hitPoint = rayCallback.m_hitPointWorld;
        auto new_pos = glm::vec2(hitPoint.x(), hitPoint.y());
//        if((new_pos - m_position).length() > 2.f) {
        m_position = new_pos;
//        }
    } else {
        m_position = glm::vec2(rayEnd.x(), rayEnd.y());
    }
}

void Foot::onDraw(sf::RenderTarget &target) {
    sf::Color color(0, 0, 0);

    float s = 0.05f;

    // Draw lower leg
    sf::Vector2f lowerLegVec(m_anklePosition.x - m_position.x, m_anklePosition.y - m_position.y);
    if(lowerLegVec == sf::Vector2f()) lowerLegVec.y = 1;
    float lowerLegLength = thor::length(lowerLegVec);

    // Draw upper leg
    sf::Vector2f upperLegVec(m_player->position().x - m_anklePosition.x, m_player->position().y - m_anklePosition.y);
    if(upperLegVec == sf::Vector2f()) upperLegVec.y = 1;
    float upperLegLength = thor::length(upperLegVec);

    auto tex = Root().resources.getTexture("upper-leg");
    auto size = tex->getSize();
    sf::Sprite upperLeg(*tex.get());
    upperLeg.setOrigin(size.x / 2, size.y * 0.96);
    upperLeg.setScale(s / size.x * 2, upperLegLength / size.y);
    upperLeg.setPosition(m_anklePosition.x, m_anklePosition.y);
    upperLeg.setRotation(90+thor::polarAngle(upperLegVec));
    target.draw(upperLeg);

    tex = Root().resources.getTexture("lower-leg");
    size = tex->getSize();
    sf::Sprite lowerLeg(*tex.get());
    lowerLeg.setOrigin(size.x / 2, size.y);
    lowerLeg.setScale(s / size.x * 2, (lowerLegLength + 0.03) / size.y);
    lowerLeg.setPosition(m_position.x, m_position.y);
    lowerLeg.setRotation(90+thor::polarAngle(lowerLegVec));
    target.draw(lowerLeg);

    // sf::CircleShape ankle;
    // ankle.setPosition(m_anklePosition.x, m_anklePosition.y);
    // ankle.setRadius(1);
    // ankle.setOrigin(1, 1);
    // ankle.setScale(s/2, s/2);
    // ankle.setFillColor(color);
    // target.draw(ankle);

    // sf::CircleShape foot;
    // foot.setPosition(m_position.x, m_position.y);
    // foot.setRadius(1);
    // foot.setOrigin(1, 1);
    // foot.setScale(s/2, s/2);
    // foot.setFillColor(color);
    // target.draw(foot);

    if(m_player->m_state->m_debugDrawEnabled) {
        sf::CircleShape foot_debug;
        foot_debug.setPosition(m_position.x, m_position.y);
        foot_debug.setRadius(1);
        foot_debug.setOrigin(1, 1);
        foot_debug.setScale(0.05, 0.05);
        foot_debug.setFillColor(sf::Color::Magenta);
        target.draw(foot_debug);

        sf::CircleShape ankle_debug;
        ankle_debug.setPosition(m_anklePosition.x, m_anklePosition.y);
        ankle_debug.setRadius(1);
        ankle_debug.setOrigin(1, 1);
        ankle_debug.setScale(0.05, 0.05);
        ankle_debug.setFillColor(sf::Color::Yellow);
        target.draw(ankle_debug);
    }
}

void Foot::setDirection(int direction) {
    m_direction = direction;
}
