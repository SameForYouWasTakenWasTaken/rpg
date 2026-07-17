#pragma once

#include "IEvent.hpp"
#include "Rendering/Window.hpp"
#include "SFML/Graphics/View.hpp"

namespace ssg
{
class Window;
class WindowSetViewEvent : public IEvent
{
  public:
    sf::View view;
    Window* window = nullptr;

    WindowSetViewEvent(sf::View newView, Window* window = nullptr) : view(newView), window(window)
    {
    }
};
} // namespace ssg