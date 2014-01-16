#include "EntityMotionState.hpp"

#include <glm/glm.hpp>

EntityMotionState::EntityMotionState(const btTransform &initialpos, std::shared_ptr<Entity> entity) :
    m_entity(entity),
    m_transform(initialpos)
{}

EntityMotionState::~EntityMotionState() {}

void EntityMotionState::getWorldTransform(btTransform &worldTrans) const {
    worldTrans = m_transform;
}

void EntityMotionState::setWorldTransform(const btTransform &worldTrans) {
    btQuaternion rot = worldTrans.getRotation();
    m_entity->setRotation(rot.getAngle());
    btVector3 pos = worldTrans.getOrigin();
    m_entity->setPosition(glm::vec2(pos.x(), pos.y()));
    m_transform = worldTrans;
}
