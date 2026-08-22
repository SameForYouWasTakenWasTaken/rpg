#pragma once
#include <SFML/Graphics.hpp>

#include "IEvent.hpp"

namespace ssg
{
class KeyRepeatedEvent : public IEvent
{
  public:
    sf::Keyboard::Key key;

    KeyRepeatedEvent(sf::Keyboard::Key k) : key(k) {}

    GENERATE_EVENT_TYPE()
};
} // namespace ssg