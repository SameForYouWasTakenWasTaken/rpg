#pragma once

#include <entt/entt.hpp>

#include "Events/IEvent.hpp"

namespace ssg
{
class OnAttackRequest : public IEvent
{
  public:
    entt::entity Attacker;

    OnAttackRequest(entt::entity attacker) : Attacker(attacker) {}
    GENERATE_EVENT_TYPE();
};
} // namespace ssg