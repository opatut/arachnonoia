#ifndef ENTITYMOTIONSTATE_HPP
#define ENTITYMOTIONSTATE_HPP

#include "Entity.hpp"

#include <memory>

#include <btBulletDynamicsCommon.h>

class EntityMotionState : public btMotionState
{

public:
    EntityMotionState(const btTransform &initialpos, std::shared_ptr<Entity> entity);
    virtual ~EntityMotionState();

    virtual void getWorldTransform(btTransform &worldTrans) const override;
    virtual void setWorldTransform(const btTransform &worldTrans) override;

protected:
    std::shared_ptr<Entity> m_entity;
    btTransform m_transform;
};

#endif // ENTITYMOTIONSTATE_HPP
