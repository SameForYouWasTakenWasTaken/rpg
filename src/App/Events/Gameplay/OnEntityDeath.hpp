#pragma once

#include <entt/entt.hpp>

#include "Events/IEvent.hpp"

namespace ssg
{
class OnEntityDeath : public IEvent
{
  public:
    entt::entity entity;

    OnEntityDeath(entt::entity e) : entity(e) {}
    GENERATE_EVENT_TYPE();
};
} // namespace ssg