#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/View.hpp"
#include "Types.hpp"

namespace ssg
{

class Camera
{
  public:
    Camera() = default;
    explicit Camera(const Vec2& center, const Vec2& size);
    ~Camera() = default;

    Camera(const Camera&) = default;
    Camera& operator=(const Camera&) = default;
    Camera(Camera&&) = default;
    Camera& operator=(Camera&&) = default;

    void SetCenter(const Vec2& center);
    Vec2 GetCenter() const;

    void SetSize(const Vec2& size);
    Vec2 GetSize() const;

    void SetRotation(float degrees);
    float GetRotation() const;

    void SetViewport(const sf::FloatRect& viewport);
    sf::FloatRect GetViewport() const;

    void Move(const Vec2& offset);
    void Rotate(float degrees);
    void Zoom(float factor);

    void SetView(const Vec2& center, const Vec2& size);
    const sf::View& GetView() const;

  private:
    sf::View m_View;
};

} // namespace ssg
