#pragma once
#include "IEvent.hpp"

namespace ssg
{
class WindowCloseEvent : public IEvent
{
  public:
    WindowCloseEvent() = default;

    GENERATE_EVENT_TYPE();
};
} // namespace ssg