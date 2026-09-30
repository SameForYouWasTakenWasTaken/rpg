#pragma once

#include <Entity.hpp>

#include "Events/IEvent.hpp"

namespace ssg
{
class OnEntityDeath : public IEvent
{
  public:
    Entity entity;

    OnEntityDeath(Entity e) : entity(e) {}
    GENERATE_EVENT_TYPE();
};
} // namespace ssg