#include "Camera.hpp"

#include "SFML/Graphics/View.hpp"

namespace ssg
{

Camera::Camera(const Vec2& center, const Vec2& size)
    : m_View(sf::Vector2f(center.x, center.y), sf::Vector2f(size.x, size.y))
{
}

void Camera::SetCenter(const Vec2& center) { m_View.setCenter(sf::Vector2f(center.x, center.y)); }

Vec2 Camera::GetCenter() const
{
    sf::Vector2f center = m_View.getCenter();
    return Vec2{center.x, center.y};
}

void Camera::SetSize(const Vec2& size) { m_View.setSize(sf::Vector2f(size.x, size.y)); }

Vec2 Camera::GetSize() const
{
    sf::Vector2f size = m_View.getSize();
    return Vec2{size.x, size.y};
}

void Camera::SetRotation(float degrees) { m_View.setRotation(sf::degrees(degrees)); }

float Camera::GetRotation() const { return m_View.getRotation().asDegrees(); }

void Camera::SetViewport(const sf::FloatRect& viewport) { m_View.setViewport(viewport); }

sf::FloatRect Camera::GetViewport() const { return m_View.getViewport(); }

void Camera::Move(const Vec2& offset) { m_View.move(sf::Vector2f(offset.x, offset.y)); }

void Camera::Rotate(float degrees) { m_View.rotate(sf::degrees(degrees)); }

void Camera::Zoom(float factor) { m_View.zoom(factor); }

void Camera::SetView(const Vec2& center, const Vec2& size)
{
    m_View.setCenter(sf::Vector2f(center.x, center.y));
    m_View.setSize(sf::Vector2f(size.x, size.y));
}

const sf::View& Camera::GetView() const { return m_View; }

} // namespace ssg
