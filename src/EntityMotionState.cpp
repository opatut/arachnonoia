#include "EntityMotionState.hpp"

#include <glm/glm.hpp>

EntityMotionState::EntityMotionState(const btTransform &initialpos, std::shared_ptr<Entity> entity) :
    m_entity(entity),
    m_pos1(initialpos)
{}

EntityMotionState::~EntityMotionState() {}

void EntityMotionState::getWorldTransform(btTransform &worldTrans) const {
    worldTrans = m_pos1;
}

void EntityMotionState::setWorldTransform(const btTransform &worldTrans) {
    btQuaternion rot = worldTrans.getRotation();
    m_entity->m_rotation = rot.getAngle();
    btVector3 pos = worldTrans.getOrigin();
    m_entity->m_position = glm::vec2(pos.x(), pos.y());
    m_pos1 = worldTrans;
}
