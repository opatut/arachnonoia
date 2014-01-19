#include "Pair.hpp"

#include <iostream>

#include <Thor/Math.hpp>

#include "Root.hpp"

Pair::Pair()
{
    setGlyphNumber(1);
    m_sprite.setTexture(* Root().resources.getTexture("pair").get());

    m_mass = 0.f;
    m_physicsShape = new btSphereShape(0.5);
    // m_physicsShape->
}

void Pair::onAdd(State* state) {
    m_physicsBody->setCollisionFlags(m_physicsBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    setGlyphNumber(m_glyphNumber); // update sprite
}

void Pair::setMetadata(int data) {
    setGlyphNumber(data);
}

void Pair::onUpdate(double dt) {
    if(m_active) {
        m_activationTime += dt;
    }

    // TODO
    m_active = true;
    m_activationTime = 0;
}

void Pair::onDraw(sf::RenderTarget& target) {
    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(1/512.f, 1/512.f);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    m_sprite.setRotation(thor::toDegree(m_rotation));
    target.draw(m_sprite);

    if(m_active) {
        m_glyphSprite.setPosition(m_position.x, m_position.y);
        m_glyphSprite.setScale(0.5 * 1/128.f, 0.5 * 1/128.f);
        m_glyphSprite.setOrigin(m_glyphSprite.getTexture()->getSize().x / 2, m_glyphSprite.getTexture()->getSize().y / 2);
        m_glyphSprite.setRotation(thor::toDegree(m_rotation));
        target.draw(m_glyphSprite);
    }
}

void Pair::setGlyphNumber(int number) {
    auto texture = Root().resources.getTexture(getGlyphName(number)).get();
    if(texture) {
        m_glyphNumber = number;
        m_glyphSprite.setTexture(*texture);
    }
}

std::string Pair::getGlyphName(int number) {
    char buf[8];
    sprintf(buf, "glyph-%02d", number);
    return std::string(buf);
}
