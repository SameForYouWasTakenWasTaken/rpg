#pragma once

#include <entt/entt.hpp>

#include "Events/Gameplay/OnAttackRequest.hpp"
#include "Systems/ISystem.hpp"
#include "Systems/SpatialGrid.hpp"

namespace ssg
{
class CombatSystem : public ISystem
{
  public:
    CombatSystem(entt::registry& registry, EventBus& bus, SpatialGrid& spatial_grid);
    void Update(float /*dt*/) override;

  private:
    void OnAttackEvent(OnAttackRequest& event);
    void ResolveAttack(entt::entity attacker, entt::entity defender);

    SpatialGrid& m_SpatialGrid;
};
} // namespace ssg