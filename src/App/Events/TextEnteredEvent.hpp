#pragma once

#include <cstdint>
#include "Events/IEvent.hpp"

namespace ssg {

class TextEnteredEvent : public IEvent {
public:
    uint32_t unicode;

    TextEnteredEvent(uint32_t u)
        : unicode(u) {
    }

    GENERATE_EVENT_TYPE();
};

} // namespace ssg
