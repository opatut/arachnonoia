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

std::string Pair::getTypeName() const {
    return "Pair";
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
}

void Pair::onDraw(State *state, sf::RenderTarget& target) {
    if(m_solved) return;

    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(1/512.f, 1/512.f);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    m_sprite.setRotation(thor::toDegree(m_rotation));
    target.draw(m_sprite);

    if(m_active) {
        m_glyphSprite.setColor(sf::Color(255, 255, 255, fmin(255, 255 * m_activationTime)));
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

std::vector<std::shared_ptr<Pair>> Pair::findMatchingPairs() {
    std::vector<std::shared_ptr<Pair>> r;
    for(auto entity : m_state->getEntities()) {
        if(entity->getTypeName() == "Pair") {
            auto p = std::static_pointer_cast<Pair>(entity);
            if(p->m_glyphNumber == m_glyphNumber) {
                r.push_back(p);
            }
        }
    }
    return r;
}

void Pair::activate() {
    if(!m_active) {
        m_active = true;
        m_activationTime = 0;

        unsigned int activeCount = 0;
        auto pairs = findMatchingPairs();
        for(auto p : pairs) {
            if(p->m_active) activeCount++;
        }
        if(activeCount == pairs.size()) {
            for(auto p : pairs) {
                p->solve();
            }
        }
    }
}

void Pair::solve() {
    if(!m_solved) {
        m_solved = true;
    }
}

bool Pair::isActive() const {
    return m_active;
}

bool Pair::isSolved() const {
    return m_solved;
}

std::string Pair::getGlyphName(int number) {
    char buf[8];
    sprintf(buf, "glyph-%02d", number);
    return std::string(buf);
}
