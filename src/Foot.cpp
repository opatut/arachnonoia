#include "Foot.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>
#include <Thor/Graphics.hpp>

Foot::Foot(Entity* player, int offset) :
    m_player(player), m_offset(offset)
{}

std::string Foot::getTypeName() const {
    return "Foot";
}

void Foot::onUpdate(float dt) {
    if(m_phase > 1.0f || m_phase < -1.0f)
        m_reverse = !m_reverse;

    float speed_factor = 10; // speed of leg movement

    if(m_offset % 2 == 1)
        m_direction *= -1;

    if(m_direction == -1) {
        if(m_reverse)
            m_phase -= dt * speed_factor;
        else
            m_phase += dt * speed_factor;
    } else if(m_direction == 1) {
        if(m_reverse)
            m_phase += dt * speed_factor;
        else
            m_phase -= dt * speed_factor;
    }

    float ankle_offset_factor = 0.2f; // distance between ankle
    float ankle_phase_factor = 0.1f; // distance of ankle move per phase

    // Offset from main body
    sf::Vector2f offsetAnkle(m_phase * ankle_phase_factor + (m_offset - 1.5) * ankle_offset_factor, -0.5f);
    sf::Vector2f relAnklePos = thor::rotatedVector(offsetAnkle, thor::toDegree(m_player->rotation()));
    m_anklePosition = glm::vec2(m_player->position().x - relAnklePos.x, m_player->position().y - relAnklePos.y);

    float leg_offset_factor = 0.4f; // distance between legs
    float leg_phase_factor = 0.2f; // distance of leg move per phase

    sf::Vector2f offsetFootRayEnd(m_phase * leg_phase_factor + (m_offset - 1.5) * leg_offset_factor, 0.5f);
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
    if(rayCallback.hasHit())
        hitPoint = rayCallback.m_hitPointWorld;

    m_position = glm::vec2(hitPoint.x(), hitPoint.y());
}

void Foot::onDraw(sf::RenderTarget &target) {
    sf::CircleShape foot;
    foot.setPosition(m_position.x, m_position.y);
    foot.setRadius(1);
    foot.setOrigin(0.5, 0.5);
    foot.setScale(0.05, 0.05);
    foot.setFillColor(sf::Color::Magenta);
    target.draw(foot);

    sf::CircleShape ankle;
    ankle.setPosition(m_anklePosition.x, m_anklePosition.y);
    ankle.setRadius(1);
    ankle.setOrigin(0.5, 0.5);
    ankle.setScale(0.05, 0.05);
    ankle.setFillColor(sf::Color::Yellow);
    target.draw(ankle);
}

void Foot::setDirection(int direction) {
    m_direction = direction;
}
