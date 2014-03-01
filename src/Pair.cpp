#include "Pair.hpp"

#include <iostream>

#include <Thor/Math.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtx/vector_angle.hpp>

#include "Root.hpp"

Pair::Pair()
{
    setGlyphNumber(1);
    m_sprite.setTexture(* Root().resources.getTexture("pair").get());

    m_mass = 0.f;
    m_physicsShape = new btSphereShape(0.01);
}

std::string Pair::getTypeName() {
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

void Pair::onDraw(sf::RenderTarget& target) {
    glm::vec2 root(0, 0.5);
    glm::rotate(root, m_rotation);
    root += m_position;

    for(int i = 0; i < 2; ++i) {
        sf::ConvexShape shape(3);
        shape.setPoint(0, sf::Vector2f(-0.02,  0));
        shape.setPoint(1, sf::Vector2f( 0.00, -1));
        shape.setPoint(2, sf::Vector2f( 0.02,  0));
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(root.x, root.y);
        shape.setRotation(m_rotation + (i-0.5) * 70);
        target.draw(shape);
    }

    glm::vec2 center = m_position;
    sf::Color lineColor(255, 255, 255, 120);
    sf::VertexArray lines;
    for(int i = 0; i < 6; ++i) {
        glm::vec2 out = glm::rotate(glm::vec2(1, 0), i * 2.f / 6.f * thor::Pi);
        glm::vec2 out2 = glm::rotate(glm::vec2(1, 0), (i+1) * 2.f / 6.f * thor::Pi);

        lines.append(sf::Vertex(sf::Vector2f(center.x, center.y), lineColor));
        lines.append(sf::Vertex(sf::Vector2f(center.x + out.x, center.y + out.y), lineColor));
        for(float d = 0.1; d < 0.8; d+=0.2) {
            lines.append(sf::Vertex(sf::Vector2f(center.x + d * out.x,  center.y + d * out.y ), lineColor));
            lines.append(sf::Vertex(sf::Vector2f(center.x + d * out2.x, center.y + d * out2.y), lineColor));
        }
    }
    lines.setPrimitiveType(sf::Lines);
    target.draw(lines);


    m_sprite.setPosition(m_position.x, m_position.y);
    m_sprite.setScale(1/512.f, 1/512.f);
    m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2, m_sprite.getTexture()->getSize().y / 2);
    m_sprite.setRotation(thor::toDegree(m_rotation));
    // target.draw(m_sprite);

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
