#include "CollisionShape.hpp"

#include "State.hpp"
#include "Root.hpp"

CollisionShape::CollisionShape() {
    m_zLevel = 500;
}

std::string CollisionShape::getTypeName() {
    return "CollisionShape";
}

void CollisionShape::onDraw(sf::RenderTarget& target) {
    if(m_state != &Root().editor_state) return;

    auto pix = m_state->getPixelSize();

    for(auto points : m_shapes) {
        sf::ConvexShape shape(points.size());
        for(unsigned int i = 0; i < points.size(); ++i) {
            shape.setPoint(i, sf::Vector2f(points[i].x * m_scale.x, points[i].y * m_scale.y));
        }
        shape.setPosition(m_position.x, m_position.y);
        shape.setOutlineThickness(2 * m_state->getPixelSize());
        shape.setOutlineColor(sf::Color::Cyan);
        shape.setFillColor(sf::Color::Transparent);
        target.draw(shape);

        int pointSize = 10;
        for(auto p : points) {
            sf::RectangleShape r(sf::Vector2f(pointSize*pix, pointSize*pix));
            r.setPosition(m_position.x + p.x - pointSize / 2 * pix, m_position.y + p.y - pointSize / 2 * pix);
            r.setFillColor(sf::Color::Magenta);
            target.draw(r);
        }
    }
}

void CollisionShape::onInitialize() {
    btCompoundShape* compound = new btCompoundShape();

    for(auto points : m_shapes) {
        btTriangleMesh* mesh = new btTriangleMesh();
        for(unsigned int i = 0; i < points.size(); ++i) {
            const glm::vec2& p = points[i] * m_scale;
            const glm::vec2& q = points[(i+1)%points.size()] * m_scale;
            const glm::vec2  m = (p+q)/2.f;

            mesh->addTriangle(btVector3(p.x, p.y, 0), btVector3(q.x, q.y, 0), btVector3(m.x, m.y, 1));
        }

        btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(mesh, true);
        compound->addChildShape(btTransform::getIdentity(), shape);
    }

    m_physicsShape = compound;
}

void CollisionShape::onAdd(State *state) {
}

std::vector<std::vector<glm::vec2>>& CollisionShape::shapes() {
    return m_shapes;
}
