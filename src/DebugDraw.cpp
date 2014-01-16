#include "DebugDraw.hpp"

#include <iostream>

#include "Root.hpp"

DebugDraw::DebugDraw() {
    m_target = Root().window;
}

sf::Color DebugDraw::btToSfColor(const btVector3& color) {
	sf::Color result(sf::Uint8((color.x() * 255)), 
					 sf::Uint8((color.y() * 255)), 
					 sf::Uint8((color.z() * 255)));
	return result;
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(from.x(), from.y()), btToSfColor(color)),
        sf::Vertex(sf::Vector2f(to.x(), to.y()), btToSfColor(color))
    };
    m_target->draw(line, 2, sf::Lines);
}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& from_color, const btVector3& to_color) {
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(from.x(), from.y()), btToSfColor(from_color)),
        sf::Vertex(sf::Vector2f(to.x(), to.y()), btToSfColor(to_color))
    };
    m_target->draw(line, 2, sf::Lines);
}

void DebugDraw::drawSphere(btScalar radius, const btTransform& transform, const btVector3& color) {
    sf::CircleShape circle(radius);
    circle.setPosition(transform.getOrigin().x(), transform.getOrigin().y());
    circle.setOrigin(radius, radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(btToSfColor(color));
    circle.setOutlineThickness(0.01f);
    m_target->draw(circle);
}

void DebugDraw::drawSphere(const btVector3& p, btScalar radius, const btVector3& color) {
    sf::CircleShape circle(radius);
    circle.setPosition(p.x(), p.y());
    circle.setOrigin(radius, radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(btToSfColor(color));
    circle.setOutlineThickness(0.01f);
    m_target->draw(circle);
}

void DebugDraw::drawTriangle(const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar) {
    sf::Vertex triangle[] = {
        sf::Vertex(sf::Vector2f(v0.x(), v0.y()), btToSfColor(color)),
        sf::Vertex(sf::Vector2f(v1.x(), v1.y()), btToSfColor(color)),
        sf::Vertex(sf::Vector2f(v2.x(), v2.y()), btToSfColor(color))
    };
    m_target->draw(triangle, 3, sf::Triangles);
}

void DebugDraw::drawContactPoint(const btVector3& point_on_b, const btVector3& normal_on_b, btScalar distance, int life_time, const btVector3& color) {
	drawLine(point_on_b, point_on_b + normal_on_b * distance, color);
}

void DebugDraw::reportErrorWarning(const char* warning_string) {
    std::cerr << warning_string << std::endl;
}

void DebugDraw::draw3dText(const btVector3& location, const char* text_string) {
    sf::Text text;
    text.setString(text_string);
    text.setPosition(location.x(), location.y());
    m_target->draw(text);
}

void DebugDraw::setDebugMode(int mode) {
    m_debugMode = mode;
}

int DebugDraw::getDebugMode() const {
    return m_debugMode;
}

void DebugDraw::drawAabb(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
    sf::RectangleShape rect(sf::Vector2f(to.x() - from.x(), to.y() - from.y()));
    rect.setPosition(from.x(), from.y());
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(btToSfColor(color));
    rect.setOutlineThickness(1.f);
}
