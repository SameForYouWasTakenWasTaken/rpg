#pragma once

#include <Entity.hpp>

#include "Events/IEvent.hpp"

namespace ssg
{
class OnAttackRequest : public IEvent
{
  public:
    Entity Attacker;

    OnAttackRequest(Entity attacker) : Attacker(attacker) {}
    GENERATE_EVENT_TYPE();
};
} // namespace ssg