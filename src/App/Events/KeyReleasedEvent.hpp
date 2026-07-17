#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "Events/IEvent.hpp"

namespace ssg {

class KeyReleasedEvent : public IEvent {
public:
    sf::Keyboard::Key key;
    bool alt;
    bool control;
    bool shift;
    bool system;

    KeyReleasedEvent(sf::Keyboard::Key k, bool a = false, bool c = false, bool s = false, bool sys = false)
        : key(k), alt(a), control(c), shift(s), system(sys) {
    }

    GENERATE_EVENT_TYPE();
};

} // namespace ssg
